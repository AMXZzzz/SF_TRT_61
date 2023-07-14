#pragma once
#include <onnxruntime_cxx_api.h>
#include <onnxruntime_c_api.h>
#include <dml_provider_factory.h>
#include <opencv2/opencv.hpp>
#include <windows.h>

struct DMLModel {
	DMLModel() {};
	~DMLModel() {};
	static DMLModel& Get() {
		static DMLModel m_pInstance;
		return m_pInstance;
	}
	int Input_size ;
	int Anchorbox_num = 0;				
	int num_classes = 0;					
	float* floatarr = nullptr;				

	std::vector<int64_t> input_dims;
	std::vector<int64_t> output_dims;

	BOOL InitDML(const char*);
	VOID Preprocess(cv::Mat&);
	VOID Detect();
	VOID(*Postprocess)(float*, int, int, int);
	VOID(*LockStart)();
	VOID Release();
private:
	VOID LoadDMLResource();
	BOOL GetLayerInfo( std::vector<int64_t>&,int, BOOL);
	BOOL GetModelInfo();
	BOOL CheckStatus(OrtStatus* status, int);

	const OrtApi* g_ort = OrtGetApiBase()->GetApi(ORT_API_VERSION);


	OrtEnv* env;
	OrtSessionOptions* session_options;
	OrtSession* session = nullptr;
	OrtMemoryInfo* memory_info;
	OrtAllocator* allocator;


	float Normalized = 1.0;

	size_t input_tensor_size = 1;			

	OrtValue* input_tensors = nullptr;		
	OrtValue* output_tensors = nullptr;		

	int input_idx = 0;						
	int output_idx = 0;						

	std::vector<const char*> input_names = { "images" };
	std::vector<const char*> output_names = { "output" };

};

VOID DMLFrame();