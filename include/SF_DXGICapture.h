#pragma once
#include<opencv2/opencv.hpp>
#include<windows.h>

#define SF_API  __declspec(dllexport)
#define CHECK(x)	(x!=DXGI_NOERROR)

// 初始化错误表
typedef enum SF_DXGI_ERROR {
	DXGI_NOERROR = 0,	/* 执行成功 */
	DXGI_DEVICE_ERROR = 1,	/* 初始化d3ddevice失败 */
	DXGI_DEVICE2_ERROR = 2,	/* 初始化d3ddevice2失败 */
	DXGI_ADAPTER_ERROR = 3,	/* 初始化d3d适配器失败 */
	DXGI_OUTPUT_ERROR = 4,	/* 初始化d3d输出接口失败 */
	DXGI_OUTPUT1_ERROR = 5,	/* 初始化d3d输出接口2失败 */
	DXGI_DUPLICATE_ERROR = 6,	/* 初始化d3d显示器信息失败 */

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
