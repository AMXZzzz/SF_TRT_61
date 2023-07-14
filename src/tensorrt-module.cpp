
#include "sf-trt.h"
#include "tensorrt-module.h"
#include "SF_DXGICapture.h"
#include "framework.h"
#include "lock-module.h"


#define CHECK_TRT(x) (x==NULL)

static HANDLE tensorrt_thread = NULL;		

#pragma region ���
static int volume(const nvinfer1::Dims & dims) {
	int size = 1;
	for (int i = 0; i < dims.nbDims; ++i)
		size *= dims.d[i];
	return size;
}


static int64_t GetMemorySize(const nvinfer1::Dims & dims, const nvinfer1::DataType & type) {
	return static_cast<unsigned long long>(1) * volume(dims) * sizeof(type);
}


static inline VOID PrintDimsToLog(const nvinfer1::Dims dims, const BOOL input) {
	char temp[MAX_PATH]{};
	if (input)
		_snprintf_s(temp, MAX_PATH, "����ά��:[ %d  %d  %d  %d ]", dims.d[0], dims.d[1], dims.d[2], dims.d[3]);
	else
		_snprintf_s(temp, MAX_PATH, "���ά��:[ %d  %d  %d ]", dims.d[0], dims.d[1], dims.d[2]);
	g_logger->info(temp);
}

static inline VOID CHECKCUDA_DEBUG(const cudaError err, const char* path, const int line) {
	if (err != cudaSuccess) {
		g_logger->warn("CUDA Error:������:{} �� {} �ļ�: ��{}��", std::to_string(err).c_str(), path, std::to_string(line).c_str());
		SF::Signl::Get().ThreadStopSignl = FALSE;		// CUDA���ִ��������˳�
	}
}

VOID DrawBox(cv::Mat &img) {
	Process* process = &Process::Get();
	if (!SF::Signl::Get().ShowWindow) {
		if (cv::getWindowProperty(WINDOWS_NAME, cv::WND_PROP_VISIBLE))
			cv::destroyWindow(WINDOWS_NAME);
	}else {
		for (int i = 0; i < process->indices.size(); ++i) {
			cv::rectangle(img,
				cv::Rect(
					process->boxes[process->indices[i]].x - process->boxes[process->indices[i]].width * 0.5f,
					process->boxes[process->indices[i]].y - process->boxes[process->indices[i]].height * 0.5f,
					process->boxes[process->indices[i]].width,
					process->boxes[process->indices[i]].height),
				cv::Scalar(0, 255, 0), 2, 8, 0);
		}
		cv::imshow(WINDOWS_NAME, img);
		cv::waitKey(1);
	}
}

#pragma endregion

VOID TensorRtModule::GetFrameResourse() {
	strcpy_s(engine_path_a, UTF8ToAnsi(SF::Value::Get().engine_path).c_str());

	switch (SF::Variables::Get().process_frame) {
	case 0:		// yolov5/7
		Pre = YoloV5Per;
		Post = YoloV5Post;
		break;
	case 1:		// yolox
		Pre = YoloXPer;
		Post = YoloXPost;
		break;
	case 2:		//yolov8
		Pre = YoloV8Per;
		Post = YoloV8Post;
		break;
	}
	cudaSetDevice(SF::Value::Get().gpuidx);
	g_logger->info("����GPU:{}", std::to_string(SF::Value::Get().gpuidx).c_str());
	g_logger->info("��ȡTensorRt��Դ    Done...");
}

BOOL TensorRtModule::LoadEngineModel() {
	std::ifstream file(engine_path_a, std::ios::binary);
	if (!file.good()) {
		g_logger->warn("��ȡengine����: {}", engine_path_a);
		MessageBoxA(NULL, "��ȡengine�ļ�����", MESSAGEBOX_TITLE, MB_OK);
		return FALSE;
	}
	file.seekg(0, file.end);
	char_stream_size = file.tellg();
	file.seekg(0, file.beg);
	char_stream = new char[char_stream_size];
	assert(char_stream);
	file.read(char_stream, char_stream_size);
	file.close();
	g_logger->info("����engine�ļ�    Done...");
	return TRUE;
}

BOOL TensorRtModule::CreateTensoRtInterface() {
	runtime = nvinfer1::createInferRuntime(gLogger);
	if (CHECK_TRT(runtime)) {
		g_logger->warn("���� runtime ʧ��");
		MessageBoxA(NULL, "����runtimeʧ��", MESSAGEBOX_TITLE, MB_OK);
		return FALSE;
	}

	engine = runtime->deserializeCudaEngine(char_stream, char_stream_size);
	delete[] char_stream;
	char_stream = nullptr;
	g_logger->info("new���ֽ����ͷ�    Done...");
	if (CHECK_TRT(engine)) {
		g_logger->warn("���� engine ʧ��");
		MessageBoxA(NULL, "����engineʧ��", MESSAGEBOX_TITLE, MB_OK);
		return FALSE;
	}

	context = engine->createExecutionContext();
	if (CHECK_TRT(context)) {
		g_logger->warn("���� context ʧ��");
		MessageBoxA(NULL, "����contextʧ��", MESSAGEBOX_TITLE, MB_OK);
		return FALSE;
	}


	g_logger->info("��ʼ��TensorRT�ӿ�    Done...");
	return TRUE;
}

VOID TensorRtModule::SetParameAndMalloc(const int& indices, const nvinfer1::Dims& dims,
	const nvinfer1::DataType& type, BOOL& hr, BOOL IsInput = TRUE) {
	if (IsInput) {
		input_idx = indices;
		Input_size = SF::Variables::Get().max_range = dims.d[2];
		host_input_size = volume(dims);
		input_memory_size = GetMemorySize(dims, type);	//2076672
		PrintDimsToLog(dims, IsInput);
		CHECKCUDA_DEBUG(cudaMalloc(&gpu_memory_ptr[indices], input_memory_size), __FILE__, __LINE__);
	}
	else {
		output_idx = indices;
		Anchorbox_num = dims.d[1];
		host_output_size = volume(dims);
		SF::Value::Get().class_number = Num_classes = dims.d[2] - 5;
		output_memory_size = GetMemorySize(dims, type);	// 298116
		PrintDimsToLog(dims, IsInput);
		CHECKCUDA_DEBUG(cudaMalloc(&gpu_memory_ptr[indices], output_memory_size), __FILE__, __LINE__);
	}
	hr = TRUE;
}

VOID TensorRtModule::SpareCreateDeviceMemory(BOOL& hr_in, BOOL& hr_out) {
	for (int i = 0; i < engine->getNbBindings(); ++i) {
		nvinfer1::Dims dims = engine->getBindingDimensions(i);
		nvinfer1::DataType type = engine->getBindingDataType(i);
		if (!hr_in) {
			if ((dims.nbDims == 4) && (dims.d[1] == 3)) {	// ����ά��Ϊ4��ά�ȵĵ�1����Ϊ3ͨ��
				if (dims.d[2] != dims.d[3]) {
					g_logger->warn("CreateVariousMemory: ģ�����벻�Գ�");
					MessageBoxA(NULL, "ģ�����벻�Գ�", MESSAGEBOX_TITLE, MB_OK);
					return;
				}
				SetParameAndMalloc(i, dims, type, hr_in, TRUE);
			}
		}
		if (!hr_out) {
			if (dims.nbDims == 3) {
				if ((dims.d[0] < dims.d[1]) && (dims.d[0] < dims.d[2]) && (dims.d[1] > dims.d[2])) {	// ���������ά��
					SetParameAndMalloc(i, dims, type, hr_out, FALSE);
				}
			}
		}
	}
}

BOOL TensorRtModule::CreateDeviceMemory() {
	BOOL hr_in = FALSE;
	BOOL hr_out = FALSE;
	for (size_t i = 0; i < engine->getNbBindings(); i++) {
		nvinfer1::Dims dims = engine->getBindingDimensions(i);
		nvinfer1::DataType type = engine->getBindingDataType(i);
		if (strcmp(engine->getBindingName(i), "images") == 0) {
			if (dims.d[2] != dims.d[3]) {
				g_logger->warn("ģ������㲻�Գ�");
				MessageBoxA(NULL, "ģ������㲻�Գ�", MESSAGEBOX_TITLE, MB_OK);
				return FALSE;
			}
			SetParameAndMalloc(i, dims, type, hr_out, TRUE);
		}
		else if (strcmp(engine->getBindingName(i), "output") == 0) {
			SetParameAndMalloc(i, dims, type, hr_out, FALSE);
		}
		else {
			// �����ڵ�,���Բ��ã����Ǳ�������
			CHECKCUDA_DEBUG(cudaMalloc(&gpu_memory_ptr[i], GetMemorySize(dims, type)), __FILE__, __LINE__);
		}
	}
	cudaStreamCreate(&stream);

	if (hr_in && hr_out) return TRUE;
	g_logger->warn("CreateDeviceMemory: ��һ�β��ҽڵ�ʧ��,input:{},output:{}", std::to_string(hr_in).c_str(), std::to_string(hr_out).c_str());

	SpareCreateDeviceMemory(hr_in, hr_out);
	if (hr_in && hr_out) return TRUE;

	if (!hr_in) g_logger->warn("��������ڵ��2��ʧ��");
	if (!hr_out) g_logger->warn("��������ڵ��2��ʧ��");

	return FALSE;
}

BOOL TensorRtModule::InintTensorRtResourse() {
	BOOL hr = TRUE;

	GetFrameResourse();

	hr = LoadEngineModel();
	asserthr(hr);

	hr = CreateTensoRtInterface();
	asserthr(hr);

	hr = CreateDeviceMemory();
	asserthr(hr);

	host_input_ptr = new float[host_input_size];
	host_output_ptr = new float[host_output_size];

	g_logger->info("����Gpu�ڴ��Host�ڴ�    Done...");
	return TRUE;
}

VOID TensorRtModule::TensorRtFree() {
	for (int i = 0; i < 5; i++) {
		if (gpu_memory_ptr[i]) {
			cudaFree(gpu_memory_ptr[i]);
		}
	}

	if (context) context->destroy();
	if (engine) engine->destroy();
	if (runtime) runtime->destroy();

	if (host_input_ptr != NULL) delete[]host_input_ptr;
	if (host_output_ptr != NULL) delete[]host_output_ptr;

	g_logger->info("�ͷ�TensorRT�����Դ���");
}

VOID TensorRtModule::Inference() {
	CHECKCUDA_DEBUG(cudaMemcpyAsync(gpu_memory_ptr[input_idx], host_input_ptr, input_memory_size, cudaMemcpyHostToDevice, stream), __FILE__, __LINE__);
	context->executeV2(gpu_memory_ptr);		
	CHECKCUDA_DEBUG(cudaMemcpyAsync(host_output_ptr, gpu_memory_ptr[output_idx], output_memory_size, cudaMemcpyDeviceToHost, stream), __FILE__, __LINE__);
}

static DWORD WINAPI TensorrtThread() {

	SF::Signl* signl = &SF::Signl::Get();
	TensorRtModule* trt = &TensorRtModule::Get();
	IInterface* sf = &IInterface::Get();
	Process* process = &Process::Get();
	LockMode* lock = &LockMode::Get();
	cv::Mat img;

	if (!trt->InintTensorRtResourse()) {
		goto Free;
	}

	SF_DXGI_ERROR dxerr = sf->SFInitCaptureResourceEx(trt->Input_size, trt->Input_size);
	if (dxerr != DXGI_NOERROR) {
		g_logger->warn("DXGI��ͼ��ʼ��ʧ�ܣ�ʧ���룺{},Ϊ6������GPUֱ��", std::to_string(dxerr).c_str());
		MessageBoxA(NULL, "DXGI��ʼ��ʧ��,������鿴��־", MESSAGEBOX_TITLE, MB_OK);
		goto Free;
	}


	if (!lock->InitLock(SF::Variables::Get().move_way, &(trt->LockStart))) {
		g_logger->warn("����ģ���ʼ��ʧ��");
		goto Free;
	}

	g_logger->info("TensorrtThread: while��ʼ����...");
	while (signl->ThreadStopSignl && signl->ImGuiWinStop) {
		// ��ͼ
		sf->SFGetDesktopBitmapToMat(&img);
		// Ԥ���� ָ���޸�Done
		trt->Pre(img, trt->host_input_ptr);
		// ����
		trt->Inference();
		// ����
		trt->Post(trt->host_output_ptr, trt->Num_classes,0,trt->Anchorbox_num);
		// �ƶ�
		trt->LockStart();
		// ��ʾ
		DrawBox(img); 
	}
	if (cv::getWindowProperty(WINDOWS_NAME, cv::WND_PROP_VISIBLE))
		cv::destroyWindow(WINDOWS_NAME);
	sf->Release();
	g_logger->info("TensorrtThread: while����...");

Free:
	trt->TensorRtFree();
	g_logger->info("TensorrtThread: �ͷ����...");

	// ��λ�ź�
	signl->ThreadReady = TRUE;
	signl->ThreadStopSignl = TRUE;
	tensorrt_thread = NULL;
	g_logger->info("TensorrtThread: ��λ���...");
	g_logger->info("-------------------- TensorRT��ܽ��� --------------------");
	return 0;
}

VOID TensorRtFrame() {
	SF::Signl::Get().ThreadReady = FALSE;

	if (tensorrt_thread == NULL) {
		tensorrt_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TensorrtThread, 0, 0, 0);
		if (tensorrt_thread == NULL) {
			SF::Signl::Get().ThreadReady = TRUE;
			g_logger->warn("����TRT�����߳�ʧ��");
			MessageBoxA(NULL, "����TRT�����߳�ʧ��", MESSAGEBOX_TITLE, MB_OK);
			return;
		}
	}
	g_logger->info("-------------------- ����TRT�����߳�PASS --------------------");
}