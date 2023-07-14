#pragma once
#include <windows.h>
#include "opencv2/opencv.hpp"

struct Process {
	static Process& Get() {
		static Process m_pInstance;
		return m_pInstance;
	}

	std::vector<int> indices;			
	std::vector<cv::Rect> boxes;		
	std::vector<int> classes;		
	std::vector<float> confidences;		

	//yolox
	struct GridAndStride {				
		int grid0;
		int grid1;
		int stride;
	};
	std::vector<int> strides = { 8, 16, 32 };	
	std::vector<GridAndStride> grid_strides;	
};



VOID YoloV5Per(cv::Mat& img, float* host_input_prt);
VOID YoloXPer(cv::Mat& img, float* host_input_prt);
VOID YoloV8Per(cv::Mat& img, float* host_input_prt);

VOID YoloV5Post(float* , int ,int,int);
VOID YoloXPost(float*, int, int, int);
VOID YoloV8Post(float*, int, int, int);