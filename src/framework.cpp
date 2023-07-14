#include "framework.h"
#include "sf-trt.h"
#include "tensorrt-module.h"
#include "SF_DXGICapture.h"


VOID YoloV5Per(cv::Mat& img,float* host_input_prt) {
	for (int c = 0; c < 3; ++c) {
		for (int h = 0; h < img.rows; ++h) {
			cv::Vec3b* p1 = img.ptr<cv::Vec3b>(h);
			for (int w = 0; w < img.cols; ++w) {
				host_input_prt[c * img.cols * img.rows + h * img.cols + w] = (p1[w][c]) / 255.0f;
			}
		}
	}
}

VOID YoloXPer(cv::Mat& img, float* host_input_prt) {

	for (int c = 0; c < 3; ++c) {
		for (int h = 0; h < img.rows; ++h) {
			cv::Vec3b* p1 = img.ptr<cv::Vec3b>(h);
			for (int w = 0; w < img.cols; ++w) {
				host_input_prt[c * img.cols * img.rows + h * img.cols + w] = (p1[w][c]);
			}
		}
	}

}

VOID YoloV8Per(cv::Mat& img, float* host_input_prt) {

	for (int c = 0; c < 3; ++c) {
		for (int h = 0; h < img.rows; ++h) {
			cv::Vec3b* p1 = img.ptr<cv::Vec3b>(h);
			for (int w = 0; w < img.cols; ++w) {
				host_input_prt[c * img.cols * img.rows + h * img.cols + w] = (p1[w][c]) / 255.0f;
			}
		}
	}

}

VOID YoloV5Post(float* output, int class_num, int size=0, int Anchorbox_num=0) {	// 输出数组，类别，输入大小，先验框

	Process* process = &Process::Get();
	float conf = SF::Value::Get().confidence;

	process->boxes.clear();
	process->confidences.clear();
	process->classes.clear();
	process->indices.clear();

	for (int i = 0; i < Anchorbox_num; i++) {	//global_data.Output_Dim = [1,29200,5+cls]

		float temp_conf = *std::max_element(&output[i * (5 + class_num) + 5],
			&output[(i + 1) * (5 + class_num)]);

		if (temp_conf < conf) 
			continue;
		
		cv::Rect temp;
		temp.x = ((float*)output)[i * (5 + class_num)];
		temp.y = ((float*)output)[i * (5 + class_num) + 1];
		temp.width = ((float*)output)[i * (5 + class_num) + 2];
		temp.height = ((float*)output)[i * (5 + class_num) + 3];
		int tempClass = std::max_element(&output[i * (5 + class_num) + 5],
			&output[(i + 1) * (5 + class_num)])
			- &output[i * (5 + class_num) + 5];

		process->boxes.push_back(temp);
		process->classes.push_back(tempClass);
		process->confidences.push_back(((float*)output)[i * (5 + class_num) + 4] * temp_conf);
	}

	cv::dnn::NMSBoxes(process->boxes, process->confidences, conf, IOU, process->indices);
}


static VOID generate_grids_and_stride(Process* process, int size) {

	for (auto stride : process->strides) {
		int num_grid_w = size / stride;
		int num_grid_h = size / stride;
		for (int g1 = 0; g1 < num_grid_w; g1++) {
			for (int g0 = 0; g0 < num_grid_h; g0++) {
				Process::GridAndStride gs{};
				gs.grid0 = g0;
				gs.grid1 = g1;
				gs.stride = stride;
				process->grid_strides.push_back(gs);
			}
		}
	}
}

VOID YoloXPost(float* output, int class_num,int size,int Anchorbox_num) {

	Process* process = &Process::Get();
	float conf = SF::Value::Get().confidence;

	process->boxes.clear();
	process->confidences.clear();
	process->classes.clear();
	process->indices.clear();
	process->grid_strides.clear();

	generate_grids_and_stride(process, size);

	for (int anchor_idx = 0; anchor_idx < process->grid_strides.size(); ++anchor_idx) {
		const int grid0 = process->grid_strides[anchor_idx].grid0;
		const int grid1 = process->grid_strides[anchor_idx].grid1;
		const int stride = process->grid_strides[anchor_idx].stride;
		const int basic_pos = anchor_idx * (class_num + 5);

		float x_center = (output[basic_pos + 0] + grid0) * stride;
		float y_center = (output[basic_pos + 1] + grid1) * stride;
		float w = exp(output[basic_pos + 2]) * stride;
		float h = exp(output[basic_pos + 3]) * stride;
		float x0 = x_center - w * 0.5f;
		float y0 = y_center - h * 0.5f;
		float box_objectness = output[basic_pos + 4];
		
		for (int class_idx = 0; class_idx < class_num; ++class_idx) {
			float box_cls_score = output[basic_pos + 5 + class_idx];
			float box_prob = box_objectness * box_cls_score;
			if (box_prob > conf) {
				cv::Rect rect;
				rect.x = x_center;
				rect.y = y_center;
				rect.width = w;
				rect.height = h;

				process->classes.push_back(class_idx);
				process->confidences.push_back((float)box_prob);
				process->boxes.push_back(rect);
			}
		}
	}
	cv::dnn::NMSBoxes(process->boxes, process->confidences, conf, IOU, process->indices);
}


VOID YoloV8Post(float* output, int class_num, int size, int Anchorbox_num) {

}