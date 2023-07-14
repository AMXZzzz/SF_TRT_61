#include "dml-module.h"
#include "sf-trt.h"
#include "SF_DXGICapture.h"
#include "framework.h"
#include "lock-module.h"

#define CHECKORT(x,y) if (!CheckStatus(x,y)) return FALSE;

static HANDLE dml_thread = NULL;		
BOOL DMLModel::CheckStatus(OrtStatus * status, int line) {
	if (status != NULL) {
		std::cout << line << std::endl;
		const char* msg = g_ort->GetErrorMessage(status);
		std::cout << msg << std::endl;
		g_ort->ReleaseStatus(status);
		return FALSE;
	}
	return TRUE;
}

VOID DMLModel::LoadDMLResource() {

	switch (SF::Variables::Get().process_frame) {
	case 0:		// yolov5/7
		Normalized = 1 / 255.f;
		Postprocess = YoloV5Post;
		break;
	case 1:		// yolox
		Normalized = 1.0;
		Postprocess = YoloXPost;
		break;
	case 2:		//yolov8
		Normalized = 1 / 255.f;
		Postprocess = YoloV8Post;
		break;
	}
}

BOOL DMLModel::InitDML(const char* path) {

	LoadDMLResource();
	CHECKORT(g_ort->CreateEnv(ORT_LOGGING_LEVEL_WARNING, "SuperResolution", &env) , __LINE__);	
	CHECKORT(g_ort->CreateSessionOptions(&session_options), __LINE__);							
	CHECKORT(g_ort->SetSessionGraphOptimizationLevel(session_options, ORT_ENABLE_BASIC), __LINE__);	
	CHECKORT(g_ort->DisableMemPattern(session_options), __LINE__);			
	OrtSessionOptionsAppendExecutionProvider_DML(session_options, 0);	
	CHECKORT(g_ort->CreateSession(env, String2WString(path).c_str(), session_options, &session), __LINE__);	
	asserthr(GetModelInfo());
	CHECKORT(g_ort->CreateCpuMemoryInfo(OrtArenaAllocator, OrtMemTypeDefault, &memory_info), __LINE__);
	return TRUE;
}

BOOL DMLModel::GetLayerInfo(std::vector<int64_t>& dims,int idx, BOOL IsInput = TRUE) {
	size_t dims_size = 0;		
	OrtTypeInfo* typeinfo = nullptr;
	const OrtTensorTypeAndShapeInfo* info;

	if (IsInput) {
		CHECKORT(g_ort->SessionGetInputTypeInfo(session, idx, &typeinfo), __LINE__);
	}
	else {
		CHECKORT(g_ort->SessionGetOutputTypeInfo(session, idx, &typeinfo), __LINE__);
	}
	CHECKORT(g_ort->CastTypeInfoToTensorInfo(typeinfo, &info), __LINE__);
	CHECKORT(g_ort->GetDimensionsCount(info, &dims_size), __LINE__);

	dims.resize(dims_size);
	CHECKORT(g_ort->GetDimensions(info, dims.data(), dims_size), __LINE__);
	for (size_t i = 0; i < dims_size; i++)
		std::cout << "维度 " << i << " = " << dims[i] << std::endl;
	if (IsInput) {
		for (size_t i = 0; i < dims_size; i++)
			input_tensor_size *= input_dims[i];
	}
	return TRUE;
}

BOOL DMLModel::GetModelInfo() {
	size_t nodes_num = 0;
	char* temp_name;
	CHECKORT(g_ort->GetAllocatorWithDefaultOptions(&allocator), __LINE__);

	CHECKORT(g_ort->SessionGetInputCount(session, &nodes_num), __LINE__);
	for (size_t i = 0; i < nodes_num; i++) {
		CHECKORT(g_ort->SessionGetInputName(session, i, allocator, &temp_name), __LINE__);

		if (strcmp("images", temp_name) == 0) {											// 
			asserthr(GetLayerInfo(input_dims,i, TRUE));
			input_idx = i;
			SF::Variables::Get().max_range = Input_size = static_cast<int>(input_dims[2]);
		}
	}
	CHECKORT(g_ort->CreateTensorAsOrtValue(allocator, input_dims.data(), 4, ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT, &input_tensors), __LINE__);		

	CHECKORT(g_ort->SessionGetOutputCount(session, &nodes_num), __LINE__);
	for (size_t i = 0; i < nodes_num; i++) {
		g_ort->SessionGetOutputName(session, i, allocator, &temp_name);
		if (strcmp("output", temp_name) == 0) {
			asserthr(GetLayerInfo(output_dims,i, FALSE));
			output_idx = i;
			Anchorbox_num = static_cast<int>(output_dims[1]);
			SF::Value::Get().class_number = num_classes = static_cast<int>(output_dims[2] - 5);				
		}
	}
	CHECKORT(g_ort->CreateTensorAsOrtValue(allocator, output_dims.data(), 3, ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT, &output_tensors), __LINE__);
	return TRUE;
}

VOID DMLModel::Preprocess(cv::Mat &img) {
	static cv::Mat blob;
	blob = cv::dnn::blobFromImage(img, Normalized, cv::Size(input_dims[2], input_dims[3]), NULL, true, false);
	CheckStatus(g_ort->CreateTensorWithDataAsOrtValue(memory_info, blob.ptr<float>(), input_tensor_size * sizeof(float), input_dims.data(), input_dims.size(), ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT, &input_tensors)
	,__LINE__);
}

VOID DMLModel::Detect() {
	CheckStatus(g_ort->Run(session, NULL, input_names.data(), &input_tensors, 1, output_names.data(), 1, &output_tensors), __LINE__);
	CheckStatus(g_ort->GetTensorMutableData(output_tensors, (void**)&floatarr), __LINE__);

}

VOID DMLModel::Release() {

	if (env) g_ort->ReleaseEnv(env);
	if (memory_info) g_ort->ReleaseMemoryInfo(memory_info);
	if (session) g_ort->ReleaseSession(session);
	if (session_options) g_ort->ReleaseSessionOptions(session_options);
	if (input_tensors) g_ort->ReleaseValue(input_tensors);
	if (output_tensors) g_ort->ReleaseValue(output_tensors);
	//if (allocator) g_ort->ReleaseAllocator(allocator);
	//if (floatarr)  delete[] floatarr;
}

static DWORD WINAPI DMLThread() {
	SF::Signl* signl = &SF::Signl::Get();
	DMLModel* dml = &DMLModel::Get();
	IInterface* sf = &IInterface::Get();
	Process* process = &Process::Get();
	LockMode* lock = &LockMode::Get();
	cv::Mat img;

	if (!dml->InitDML(SF::Value::Get().cpu_onnx)) {
		goto Free;
	}
	SF_DXGI_ERROR dxerr = sf->SFInitCaptureResourceEx(dml->Input_size , dml->Input_size);
	if (dxerr != DXGI_NOERROR) {
		g_logger->warn("DXGI截图初始化失败，失败码：{}",std::to_string(dxerr).c_str());
		MessageBoxA(NULL, "DXGI初始化失败,错误码查看日志", MESSAGEBOX_TITLE, MB_OK);
		goto Free;
	}

	if (!lock->InitLock(SF::Variables::Get().move_way, &dml->LockStart)) {
		g_logger->warn("自瞄模块初始化失败");
		goto Free;
	}

	// run
	g_logger->info("DMLThread: while开始运行...");
	while (signl->ThreadStopSignl && signl->ImGuiWinStop) {
		sf->SFGetDesktopBitmapToMat(&img);
		dml->Preprocess(img);
		dml->Detect();
		// POST
		dml->Postprocess(dml->floatarr,dml->num_classes,dml->Input_size, dml->Anchorbox_num);
		dml->LockStart();
		DrawBox(img);
	}

	if (cv::getWindowProperty(WINDOWS_NAME, cv::WND_PROP_VISIBLE))
		cv::destroyWindow(WINDOWS_NAME);
	sf->Release();
	g_logger->info("DMLThread: while结束...");
Free:
	//释放
	dml->Release();	

	// 复位
	signl->ThreadReady = TRUE;
	signl->ThreadStopSignl = TRUE;
	dml_thread = NULL;
	g_logger->info("DMLThread: 复位完成...");
	g_logger->info("-------------------- DML框架结束 --------------------");
	return 0;
}

VOID DMLFrame(){
	SF::Signl::Get().ThreadReady = FALSE;
	if (dml_thread == NULL) {
		dml_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DMLThread, 0, 0, 0);
		if (dml_thread == NULL) {
			SF::Signl::Get().ThreadReady = TRUE;
			g_logger->warn("创建DML推理线程失败");
			MessageBoxA(NULL, "创建DML推理线程失败", MESSAGEBOX_TITLE, MB_OK);
			return;
		}
	}
	g_logger->info("-------------------- 创建DML推理线程PASS --------------------");

}

