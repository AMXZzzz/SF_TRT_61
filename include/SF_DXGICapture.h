#pragma once
#include<opencv2/opencv.hpp>
#include<windows.h>

#define SF_API  __declspec(dllexport)
#define CHECK(x)	(x!=DXGI_NOERROR)

// ��ʼ�������
typedef enum SF_DXGI_ERROR {
	DXGI_NOERROR = 0,	/* ִ�гɹ� */
	DXGI_DEVICE_ERROR = 1,	/* ��ʼ��d3ddeviceʧ�� */
	DXGI_DEVICE2_ERROR = 2,	/* ��ʼ��d3ddevice2ʧ�� */
	DXGI_ADAPTER_ERROR = 3,	/* ��ʼ��d3d������ʧ�� */
	DXGI_OUTPUT_ERROR = 4,	/* ��ʼ��d3d����ӿ�ʧ�� */
	DXGI_OUTPUT1_ERROR = 5,	/* ��ʼ��d3d����ӿ�2ʧ�� */
	DXGI_DUPLICATE_ERROR = 6,	/* ��ʼ��d3d��ʾ����Ϣʧ�� */

}SF_DXGI_ERROR;

struct SFPoint {
	int origin_x = 0;
	int origin_y = 0;
	int center_x = 0;
	int center_y = 0;
	int CapWidth = 0;
	int CapHeight = 0;
};

struct SF_API IInterface {
	static IInterface& Get() {
		static IInterface m_pInstance;
		return m_pInstance;
	}

	struct SFPoint point;

	SF_DXGI_ERROR SFInitCaptureResource(
		_In_ int x,
		_In_ int y,
		_In_ int Width,
		_In_ int Height);

	SF_DXGI_ERROR SFInitCaptureResourceEx(
		_In_ int Width,
		_In_ int Height);

	BOOL SFGetDesktopBitmapToMat(
		_Out_ cv::Mat* img);

	VOID Release();
};
