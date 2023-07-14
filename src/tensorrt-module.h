#pragma once
#include <windows.h>
#include <NvInfer.h>
#include <fstream>
#include "logging.h"
#include "opencv2/opencv.hpp"


struct TensorRtModule{

	static TensorRtModule& Get() {
		static TensorRtModule m_pInstance;
		return m_pInstance;
	}
	int Input_size = 640;				
	int Anchorbox_num = 0;				
	int Num_classes = 0;				

	float* host_input_ptr = nullptr;
	float* host_output_ptr = nullptr;

	BOOL InintTensorRtResourse();
	VOID Inference();
	VOID TensorRtFree();
	VOID(*Pre)(cv::Mat& , float* );
	VOID(*Post)(float*, int, int, int);
	VOID(*LockStart)();

private:
	TensorRtModule() {};
	~TensorRtModule() {};

	VOID GetFrameResourse();
	BOOL LoadEngineModel();
	BOOL CreateTensoRtInterface();
	VOID SetParameAndMalloc(const int& indices, const nvinfer1::Dims& dims, const nvinfer1::DataType& type, BOOL& hr, BOOL IsInput);
	VOID SpareCreateDeviceMemory(BOOL& hr_in, BOOL& hr_out);
	BOOL CreateDeviceMemory();


	Logger gLogger;						
	nvinfer1::IRuntime* runtime{};
	nvinfer1::ICudaEngine* engine{};
	nvinfer1::IExecutionContext* context{};
	cudaStream_t stream = nullptr;

	char engine_path_a[MAX_PATH] = "";		
	char* char_stream = nullptr;					
	int char_stream_size = 0;			

	int host_input_size = 0;			
	int host_output_size = 0;			

	int64_t input_memory_size = 0;		
	int64_t output_memory_size = 0;		

	void* gpu_memory_ptr[5] = { NULL, NULL ,NULL ,NULL,NULL };					
	int input_idx = 0;				
	int output_idx = 0;					


VOID TensorRtFrame();