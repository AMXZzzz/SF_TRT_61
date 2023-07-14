#pragma once
#include <windows.h>
#include <NvOnnxParser.h>
#include "logging.h"
#include <fstream>

struct BuildModule {

	static BuildModule& Get() {
		static BuildModule m_pInstance;
		return m_pInstance;
	};

	BuildModule() {};
	~BuildModule() {};

	BOOL CreateOnnxInterface(char* model_path, int flag);
	BOOL BuildEngineLaunch();
	VOID Destroy();

private:
	char model_path[MAX_PATH];

	nvinfer1::IBuilder* builder = nullptr;
	nvinfer1::INetworkDefinition* network = nullptr;
	nvonnxparser::IParser* parser = nullptr;
	nvinfer1::IBuilderConfig* config = nullptr;
	nvinfer1::ICudaEngine* engine = nullptr;
	nvinfer1::IHostMemory* modelstream = nullptr;

};


VOID BuildEngine();