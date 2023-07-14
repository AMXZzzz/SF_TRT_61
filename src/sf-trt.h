#pragma once
#include <spdlog/sinks/basic_file_sink.h>
#include <imgui/imfilebrowser.h>
#include <windows.h>
#include <opencv2/opencv.hpp>

#define IOU 0.1
#define MESSAGEBOX_TITLE "SF"
#define SF_DEBUG 0
#define WINDOWS_NAME "bilibili: ����Ϣ����Ϊ��ѳ���"
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
		// ֱ�ӵ���ֵ,���ں����Դ
		static Value& Get() {
			static Value m_pInstance;
			return m_pInstance;
		}
		int gpuidx = 0;						// ѡ����ִ���豸
		char engine_path[MAX_PATH];			// CUDA��˵�engineģ�� ��Ҫu8����
		char cpu_onnx[MAX_PATH];			// dml��Ҫ��onnx
		char build_onnx[MAX_PATH];			// build��onnx

		float confidence = 0.35;			// ���Ŷ�
		float location = 0.75;				// ƫ��
		int effectiverange = 200;			// ������Ч��Χ
		int max_pixels = 15;				// �����ƶ�����
		int class_number = 1;				// �������

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

		float hfov = 0;				// ����fov
		float vfov = 0;				// ����fov
		int game_x_pixel = 0;		// ��������
		int game_y_pixel = 0;		// ��������

		int precision = 0;	// ����

		int lock_key = 0x02;		// ��������
		int lock_key2 = 0;			// �Զ��崥������

		bool auto_fire = FALSE;		// �����Զ�����
		int auto_interval = 150;	// ���150ms
		int auto_random = 80;		// ���ϵ������
		int auto_model = 0;			// ���ģʽ
	};

	struct Signl {
		// ȫ��֪ͨ�ź�
		static Signl& Get() {
			static Signl m_pInstance;
			return m_pInstance;
		}

		bool ImGuiWinStop = TRUE;	// ������ڵġ�д��FALSE
		bool KeepSave = FALSE;		// ʵʱ����
		bool ThreadReady = TRUE;	// AI�߳̾���
		bool ShowWindow = TRUE;		// ��ʾ������
		bool ThreadStopSignl = TRUE;	// �߳�ֹͣ�ź�
		bool BezierCurve = FALSE;		// ����������
		bool KalmanFilter = FALSE;		// �������˲�
		bool ModelClasses = FALSE;		// ���ѡ��
		bool BuildThreadReady = TRUE;	// build engine���߳�״̬
		bool AimSwitch = TRUE;			// ����״̬
		bool ExamineState = FALSE;
	};

	struct Variables {
		// Imgui �м������imgui��ʹ��
		static Variables& Get() {
			static Variables m_pInstance;
			return m_pInstance;
		}
		int language = 0;			// ���� 0�� ���� 1��english
		int themeitems = 0;			// ��������
		bool infer_ui = FALSE;		// ����UI
		int inference_backend = 0;	// ������
		int gpu_number = 1;			// GPU����
		int process_frame = 0;		// ģ�Ϳ��
		int move_way = 0;			// 0��Sendinput 1���Զ���
		bool key_ui = TRUE;		// ����UI
		bool parame_ui = FALSE;		// ��ʾ����UI
		int max_range = 640;			// ���Χ
		bool pid_ui = FALSE;			// PIDui
		bool fov_algorithm = FALSE;			// fov ui
		bool BuildEngineUI = FALSE;			// build engine ui
		bool EncryptionModule = FALSE;		// ���ܵ�ģ�ͣ�
	};
}
VOID DrawBox(cv::Mat& img);
extern std::shared_ptr<spdlog::logger> g_logger;	// ȫ�ֶ���
BOOL InitSpdLog();
std::string UTF8ToAnsi(const char* strSrc);
std::string StringToUTF8(const std::string& str);
std::string GetIniPath();
const wchar_t* GetWC(const char* c);
std::wstring String2WString(const std::string& s);