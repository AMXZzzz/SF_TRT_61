#pragma once
#include <spdlog/sinks/basic_file_sink.h>
#include <imgui/imfilebrowser.h>
#include <windows.h>
#include <opencv2/opencv.hpp>

#define IOU 0.1
#define MESSAGEBOX_TITLE "SF"
#define SF_DEBUG 0
#define WINDOWS_NAME "bilibili: 随风而息《此为免费程序》"
#define asserthr(x) if(!x) return FALSE;

namespace SF {
	struct FileBrowser {
		static FileBrowser& Get() {
			static FileBrowser m_pInstance;
			return m_pInstance;
		}
		ImGui::FileBrowser build_onnx_dialog;
		ImGui::FileBrowser cpu_onnx_dialog;
		ImGui::FileBrowser config_dialog;
		ImGui::FileBrowser engine_dialog;
	};

	struct Value {
		// 直接调用值,用于后端资源
		static Value& Get() {
			static Value m_pInstance;
			return m_pInstance;
		}
		int gpuidx = 0;						// 选定的执行设备
		char engine_path[MAX_PATH];			// CUDA后端的engine模型 需要u8类型
		char cpu_onnx[MAX_PATH];			// dml需要的onnx
		char build_onnx[MAX_PATH];			// build的onnx

		float confidence = 0.35;			// 置信度
		float location = 0.75;				// 偏移
		int effectiverange = 200;			// 自瞄有效范围
		int max_pixels = 15;				// 限制移动距离
		int class_number = 1;				// 类别数量

		bool class0 = FALSE;
		bool class1 = FALSE;
		bool class2 = FALSE;
		bool class3 = FALSE;

		float P_x = 0.3;
		float I_x = 0.1;
		float D_x = 0.1;
		float P_y = 0.3;
		float I_y = 0.1;
		float D_y = 0.1;

		float hfov = 0;				// 横向fov
		float vfov = 0;				// 纵向fov
		int game_x_pixel = 0;		// 横向像素
		int game_y_pixel = 0;		// 横向像素

		int precision = 0;	// 精度

		int lock_key = 0x02;		// 触法按键
		int lock_key2 = 0;			// 自定义触法按键

		bool auto_fire = FALSE;		// 开启自动开火
		int auto_interval = 150;	// 间隔150ms
		int auto_random = 80;		// 加上的随机数
		int auto_model = 0;			// 扳机模式
	};

	struct Signl {
		// 全局通知信号
		static Signl& Get() {
			static Signl m_pInstance;
			return m_pInstance;
		}

		bool ImGuiWinStop = TRUE;	// 点击窗口的×写入FALSE
		bool KeepSave = FALSE;		// 实时保存
		bool ThreadReady = TRUE;	// AI线程就绪
		bool ShowWindow = TRUE;		// 显示推理窗口
		bool ThreadStopSignl = TRUE;	// 线程停止信号
		bool BezierCurve = FALSE;		// 贝塞尔曲线
		bool KalmanFilter = FALSE;		// 卡尔曼滤波
		bool ModelClasses = FALSE;		// 类别选择
		bool BuildThreadReady = TRUE;	// build engine的线程状态
		bool AimSwitch = TRUE;			// 自瞄状态
		bool ExamineState = FALSE;
	};

	struct Variables {
		// Imgui 中间变量，imgui内使用
		static Variables& Get() {
			static Variables m_pInstance;
			return m_pInstance;
		}
		int language = 0;			// 语言 0是 中文 1是english
		int themeitems = 0;			// 界面主题
		bool infer_ui = FALSE;		// 推理UI
		int inference_backend = 0;	// 运算后端
		int gpu_number = 1;			// GPU数量
		int process_frame = 0;		// 模型框架
		int move_way = 0;			// 0：Sendinput 1：自定义
		bool key_ui = TRUE;		// 按键UI
		bool parame_ui = FALSE;		// 显示参数UI
		int max_range = 640;			// 最大范围
		bool pid_ui = FALSE;			// PIDui
		bool fov_algorithm = FALSE;			// fov ui
		bool BuildEngineUI = FALSE;			// build engine ui
		bool EncryptionModule = FALSE;		// 加密的模型？
	};
}
VOID DrawBox(cv::Mat& img);
extern std::shared_ptr<spdlog::logger> g_logger;	// 全局对象
BOOL InitSpdLog();
std::string UTF8ToAnsi(const char* strSrc);
std::string StringToUTF8(const std::string& str);
std::string GetIniPath();
const wchar_t* GetWC(const char* c);
std::wstring String2WString(const std::string& s);