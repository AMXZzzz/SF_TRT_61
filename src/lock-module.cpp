#include "lock-module.h"
#include <stdio.h>
#include "sf-trt.h"
#include <iostream>
#include "tensorrt-module.h"

#include "move-way.h"
#include "framework.h"
#include <SF_DXGICapture.h>
#include "control-algorithm.h"

BOOL LockMode::CreateLockEvent() {
	char new_name[MAX_PATH]{};
	new_name[MAX_PATH-1] = 0;
	_snprintf_s(new_name, 64, "%s%lu", "SF_TRT", (long)GetCurrentProcessId());
	lock_event = CreateEventA(NULL, FALSE, FALSE, new_name);
	if (lock_event == NULL) {
		MessageBoxA(NULL, "�����¼�ʧ��,������鿴��־", MESSAGEBOX_TITLE, MB_OK);
		g_logger->warn("�����¼�ʧ�ܣ�������{}", std::to_string(GetLastError()).c_str());
		return FALSE;
	}
	return TRUE;
}

static VOID PushIndices(std::vector<float>& EuclideanDistance, 
	std::vector<int>& indices, 
	Process* process,
	const int& idx ,
	const bool& classes){
	SFPoint* point = &IInterface::Get().point;
	if (classes) {
		indices.push_back(process->indices[idx]);
		EuclideanDistance.push_back(
			pow((point->origin_x + process->boxes[process->indices[idx]].x) - point->center_x, 2) +
			pow((point->origin_y + process->boxes[process->indices[idx]].y) - point->center_y, 2)
		);
	}
}

static inline BOOL IGetAsyncKeyState(SF::Value* value) {
	if (value->lock_key2 == 0)
		return GetAsyncKeyState(value->lock_key);
	else
		return GetAsyncKeyState(value->lock_key) || GetAsyncKeyState(value->lock_key2);
}

static std::chrono::system_clock::time_point auto_start = std::chrono::system_clock::now();
static inline BOOL AutoComplete(SF::Value* value) {
	int random_time = 0;
	if (value->auto_random != 0) {
		random_time = (int)value->auto_random * rand() / (RAND_MAX + 1);
	}
	std::chrono::system_clock::time_point auto_end = std::chrono::system_clock::now();
	return std::chrono::duration_cast <std::chrono::milliseconds> (auto_end - auto_start).count() >= (value->auto_interval + random_time) * 1.0;
}

static inline VOID LeftButtonClick() {
	INPUT input{};
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;   //MOUSEEVENTF_LEFTDOWN �������
	input.mi.time = 0;
	input.mi.dwExtraInfo = 0;
	SendInput(1, &input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;   // MOUSEEVENTF_LEFTUP  ����ɿ�
	input.mi.time = 0;
	input.mi.dwExtraInfo = 0;
	SendInput(1, &input, sizeof(INPUT));
	auto_start = std::chrono::system_clock::now();
}


DWORD WINAPI LockThread() {
	SF::Signl* signl = &SF::Signl::Get();
	SF::Value* value = &SF::Value::Get();
	SF::Variables* variables = &SF::Variables::Get();
	LockMode* lock = &LockMode::Get();
	MoveWay* moveway = &MoveWay::Get();
	Process* process = &Process::Get();
	SFPoint* point = &IInterface::Get().point;

	Algorithm algorithm_x ;
	Algorithm algorithm_y ;


	std::vector<float> EuclideanDistance;
	std::vector<int> indices;		

	while (signl->ThreadStopSignl && signl->ImGuiWinStop) {

		WaitForSingleObject(lock->lock_event, INFINITE);
		if (!process->indices.size()) continue;

		cv::Rect target_xywh;
		bool category = signl->ModelClasses;
		EuclideanDistance.clear();
		indices.clear();

		for (int i = 0; i < process->indices.size(); i++) {
			if (category) {
				switch (process->classes[process->indices[i]]) {
				case 0:
					PushIndices(EuclideanDistance, indices, process, i, value->class0);
					break;
				case 1:
					PushIndices(EuclideanDistance, indices, process, i, value->class1);
					break;
				case 2:
					PushIndices(EuclideanDistance, indices, process, i, value->class2);
					break;
				case 3:
					PushIndices(EuclideanDistance, indices, process, i, value->class3);
					break;
				}
			}else {
				EuclideanDistance.push_back(
					pow((point->origin_x + process->boxes[process->indices[i]].x) - point->center_x, 2) +
					pow((point->origin_y + process->boxes[process->indices[i]].y) - point->center_y, 2)
				);
			}
		}

		// ���Ŀ������
		int idx = 0;
		if (category) {
			if (!indices.size())
				continue;
			idx = indices[std::distance(std::begin(EuclideanDistance), std::min_element(std::begin(EuclideanDistance), std::end(EuclideanDistance)))];
		}
		else {
			idx = process->indices[std::distance(std::begin(EuclideanDistance), std::min_element(std::begin(EuclideanDistance), std::end(EuclideanDistance)))];
		}

		target_xywh.x = process->boxes[idx].x;
		target_xywh.y = process->boxes[idx].y;
		target_xywh.width = process->boxes[idx].width;
		target_xywh.height = process->boxes[idx].height;

		// ƫ��λ��
		if (value->location)
			target_xywh.y = (target_xywh.y + (target_xywh.height * 0.5)) - (target_xywh.height * value->location);

		// ��Ծ���
		float x = (point->origin_x + target_xywh.x) - point->center_x;
		float y = (point->origin_y + target_xywh.y) - point->center_y;

		// ��Χ������ 
		if (abs(x) > value->effectiverange * 0.5 || abs(y) > value->effectiverange * 0.5) continue;

		// FOV���Ƽ��� 
		if (variables->fov_algorithm) {
			algorithm_x.FOVControl(&x, value->hfov, value->game_x_pixel, point->CapWidth, 360);	// point->CapWidth
			algorithm_x.FOVControl(&y, value->vfov, value->game_y_pixel, point->CapHeight, 180);
		}

		// PID���� �����ڴ渴��
		algorithm_x.PidControl(&x, value->P_x, value->I_x, value->D_x);
		algorithm_y.PidControl(&y, value->P_y, value->I_y, value->D_y);

		// ����ƶ�
		algorithm_y.MaxMovePixel(&x, value->max_pixels);
		algorithm_y.MaxMovePixel(&y, value->max_pixels);

		// �ƶ�
		//if (GetAsyncKeyState(0x02)) {
		//	moveway->MoveR(int(x * 0.1), int(y * 0.1));
		//}
		if (value->auto_fire) {
			if (IGetAsyncKeyState(value) && signl->AimSwitch) {
				if (value->auto_model == 0) {
					// ���ƶ���ǹ
					moveway->MoveR((int)x, int(y));
					if ((target_xywh.width * 0.4f) > abs(x) && (target_xywh.height * 0.4f) > abs(y) && AutoComplete(value)) {
						// ������²�������ȴ
						LeftButtonClick();
					}
				}else {
					if ((target_xywh.width * 0.4f) > abs(x) && (target_xywh.height * 0.4f) > abs(y) && AutoComplete(value)) {
						// ������²�������ȴ
						LeftButtonClick();
					}
				}
			}
			continue;
		}

		// ���ƶ�
		if (IGetAsyncKeyState(value) && signl->AimSwitch) {
			moveway->MoveR((int)x, int(y));
		}
	}
	return 0;
}


VOID SetLockEvent() {
	//std::cout << "test" << std::endl;
	SetEvent(LockMode::Get().lock_event);
}

BOOL LockMode::InitLock(int way,VOID (**f)()) {	// ����ָ���޸ģ�����ֱ���޸�
	// ���߳�Lock
	if (way == 3) {		// ����ʽLock

	}
	else {
		// ��ʼ���¼�
		if (!CreateLockEvent()) {
			return FALSE;
		}
		// ��ʼ���ƶ�
		if (!MoveWay::Get().InitMove(way)) return FALSE;

		// �����߳�
		lock_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LockThread, 0, 0, 0);
		if (lock_thread == NULL) {
			MessageBoxA(NULL, "����Lock�߳�ʧ�ܣ�������鿴��־", MESSAGEBOX_TITLE, MB_OK);
			g_logger->warn("����Lock�߳�ʧ�ܣ�������{}", std::to_string(GetLastError()).c_str());
			return FALSE;
		}
		*f = SetLockEvent;
	}
	return TRUE;
}