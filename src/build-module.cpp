#include "build-module.h"
#include "sf-trt.h"


BOOL BuildModule::CreateOnnxInterface(char* onnx_path,int flag) {
	Logger n_logger;
	//strcpy(model_path, onnx_path);	
	builder = nvinfer1::createInferBuilder(n_logger);
	network = builder->createNetworkV2(1U << static_cast<uint32_t>(nvinfer1::NetworkDefinitionCreationFlag::kEXPLICIT_BATCH));
	parser = nvonnxparser::createParser(*network, n_logger);
	parser->parseFromFile(onnx_path, (int)Logger::Severity::kERROR);
	config = builder->createBuilderConfig();

	config->setMaxAuxStreams(static_cast<size_t>(16) * static_cast<unsigned long long>(1) << 20);
	if (builder->platformHasFastFp16() && flag == 0) {
		config->setFlag(nvinfer1::BuilderFlag::kFP16);
	}

	return TRUE;
}

BOOL BuildModule::BuildEngineLaunch() {

	g_logger->info("开始 build engine......");
	engine = builder->buildEngineWithConfig(*network, *config);
	modelstream = engine->serialize();
	g_logger->info("build engine done....");

	// 保存
	char save_engine_path[MAX_PATH];
	std::string path = (char*)model_path;
	path = path.substr(0, path.find_last_of(".")) + ".engine"; // or .trt
	strcpy_s(save_engine_path, MAX_PATH, path.c_str());
	//strcpy(save_engine_path, path.c_str());
	std::ofstream p(save_engine_path, std::ios::binary);
	if (p) {
		p.write(reinterpret_cast<const char*>(modelstream->data()), modelstream->size());
	}
	g_logger->info("保存engine    Done....");
	return TRUE;
}

VOID BuildModule::Destroy() {
	if (engine != nullptr) engine->destroy();
	if (config != nullptr) config->destroy();
	if (network != nullptr) network->destroy();
	if (builder != nullptr) builder->destroy();
	if (modelstream != nullptr) modelstream->destroy();
	g_logger->info("Build engine 释放    done...");
}

static DWORD WINAPI OnnxToEngine() {
	BuildModule* buildmodule = &BuildModule::Get();
	buildmodule->CreateOnnxInterface(SF::Value::Get().build_onnx, SF::Value::Get().precision);
	buildmodule->BuildEngineLaunch();
	buildmodule->Destroy();

	// 复位
	SF::Signl::Get().BuildThreadReady = TRUE;
	return 0;
}

VOID BuildEngine() {
	SF::Signl::Get().BuildThreadReady = FALSE;

	HANDLE build_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)OnnxToEngine, 0, 0, 0);
	if (build_thread == NULL) {
		SF::Signl::Get().BuildThreadReady = TRUE;
		g_logger->warn("创建build线程失败");
		MessageBoxA(NULL, "创建build线程失败", MESSAGEBOX_TITLE, MB_OK);
		return;
	}
}