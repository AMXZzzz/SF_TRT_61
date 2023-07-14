#include "config-module.h"
#include "sf-trt.h"
#include <math.h>
#include <fstream>
#include "examine-module.h"
#include <iostream>

static char* Node = "Imgui";

#pragma region С��� - ������

static inline VOID GetConfigPath(const char* config_name, char** config_path) {
	char path[MAX_PATH]{};
	GetModuleFileNameA(NULL, path, MAX_PATH);
	static std::string root_path = path;
	root_path = root_path.substr(0, root_path.find_last_of("\\") + 1) + config_name;
	*config_path = (char*)root_path.c_str();
}

static inline VOID CreateConfigFile(const char* file_path) {
	std::ifstream file(file_path);
	if (!file.good()) {
		std::ofstream p(file_path, std::ios_base::out);
		p.close();
	}
}

static inline BOOL CheckConfigFile(const char* file_path) {
	std::ifstream file(file_path);
	return file.good();
}

#pragma endregion 

#pragma region д��Config - ������

static inline VOID WriteConfig(const char* key, char* val, char* path) {
	WritePrivateProfileStringA(Node, key, val, path);
}
static inline VOID WriteConfig(const char* key, float val, char* path) {
	WritePrivateProfileStringA(Node, key, std::to_string(val).c_str(), path);
}
static inline VOID WriteConfig(const char* key, int val, char* path) {
	WritePrivateProfileStringA(Node, key, std::to_string(val).c_str(), path);
}

#pragma endregion 

#pragma region ��ȡConfig - ������

static inline VOID LoadParame(char* val, char(**parame)[MAX_PATH], char* config_path) {
	char temp[MAX_PATH]{};
	GetPrivateProfileStringA(Node, val, NULL, temp, MAX_PATH, config_path);
	strcpy_s(**parame, temp);							// **parame Ϊutf8�� cmd����,��Ӱ��ʹ��
	//strcpy(**parame, UTF8ToAnsi(temp).c_str());	// **parame ΪAnsi ,imgui���� ��Ӱ��۸�
}

static inline VOID LoadParame(char* val, bool* parame, char* config_path) {
	*parame = GetPrivateProfileIntA(Node, val, NULL, config_path);
}

static inline VOID LoadParame(char* val, int* parame, char* config_path) {
	*parame = GetPrivateProfileIntA(Node, val, NULL, config_path);
}
static inline VOID LoadParame(char* val, float* parame, char* config_path) {
	char str_temp[MAX_PATH]{};
	GetPrivateProfileStringA(Node, val, NULL, str_temp, MAX_PATH, config_path);
	*parame = std::strtof(str_temp, NULL);
}
#pragma endregion 

#pragma region ����Config - ������

static VOID ReadAndWrite(char* key, char(*val)[MAX_PATH], char* path, CONFIG_IO row) {
	if (row == CONFIG_READ)
		LoadParame(key, &val, path);
	else
		WriteConfig(key, *val, path);
}

static VOID ReadAndWrite(char* key, bool* val, char* path, CONFIG_IO row) {
	if (row == CONFIG_READ)
		LoadParame(key, val, path);
	else
		WriteConfig(key, *val, path);
}

static VOID ReadAndWrite(char* key, int* val, char* path, CONFIG_IO row) {
	if (row == CONFIG_READ)
		LoadParame(key, val, path);
	else
		WriteConfig(key, *val, path);
}

static VOID ReadAndWrite(char* key, float* val, char* path, CONFIG_IO row) {
	if (row == CONFIG_READ)
		LoadParame(key, val, path);
	else
		WriteConfig(key, *val, path);
}
#pragma endregion 

VOID OperationParame(CONFIG_IO row, char* path) {
	SF::Variables* variables = &SF::Variables::Get();
	SF::Value* value = &SF::Value::Get();
	Examine* examine = &Examine::Get();
	SF::Signl* signl = &SF::Signl::Get();

	ReadAndWrite("examine", &examine->examine_key, path, row);

	ReadAndWrite("KeepSave", &signl->KeepSave, path, row);
	ReadAndWrite("themeitems", &variables->themeitems, path, row);
	ReadAndWrite("language", &variables->language, path, row);

	ReadAndWrite("infer_ui", &variables->infer_ui, path, row);
	ReadAndWrite("inference_backend", &variables->inference_backend, path, row);
	ReadAndWrite("engine_path", &value->engine_path, path, row);
	ReadAndWrite("cpu_onnx", &value->cpu_onnx, path, row);
	ReadAndWrite("process_frame", &variables->process_frame, path, row);
	ReadAndWrite("move_way", &variables->move_way, path, row);
	ReadAndWrite("ShowWindow", &signl->ShowWindow, path, row);

	ReadAndWrite("key_ui", &variables->key_ui, path, row);
	ReadAndWrite("lock_key", &value->lock_key, path, row);
	ReadAndWrite("lock_key2", &value->lock_key2, path, row);
	ReadAndWrite("auto_fire", &value->auto_fire, path, row);
	ReadAndWrite("auto_interval", &value->auto_interval, path, row);
	ReadAndWrite("auto_random", &value->auto_random, path, row);
	ReadAndWrite("auto_model", &value->auto_model, path, row);

	ReadAndWrite("parame_ui", &variables->parame_ui, path, row);
	ReadAndWrite("confidence", &value->confidence, path, row);
	ReadAndWrite("location", &value->location, path, row);
	ReadAndWrite("effectiverange", &value->effectiverange, path, row);
	ReadAndWrite("max_range", &variables->max_range, path, row);
	ReadAndWrite("max_pixels", &value->max_pixels, path, row);
	ReadAndWrite("ModelClasses", &signl->ModelClasses, path, row);
	ReadAndWrite("class0", &value->class0, path, row);
	ReadAndWrite("class1", &value->class1, path, row);
	ReadAndWrite("class2", &value->class2, path, row);
	ReadAndWrite("class3", &value->class3, path, row);

	ReadAndWrite("pid_ui", &variables->pid_ui, path, row);
	ReadAndWrite("P_x", &value->P_x, path, row);
	ReadAndWrite("I_x", &value->I_x, path, row);
	ReadAndWrite("D_x", &value->D_x, path, row);
	ReadAndWrite("P_y", &value->P_y, path, row);
	ReadAndWrite("I_y", &value->I_y, path, row);
	ReadAndWrite("D_y", &value->D_y, path, row);

	ReadAndWrite("fov_algorithm", &variables->fov_algorithm, path, row);
	ReadAndWrite("game_x_pixel", &value->game_x_pixel, path, row);
	ReadAndWrite("game_y_pixel", &value->game_y_pixel, path, row);
	ReadAndWrite("hfov", &value->hfov, path, row);
	ReadAndWrite("vfov", &value->vfov, path, row);

	ReadAndWrite("BuildEngineUI", &variables->BuildEngineUI, path, row);
	ReadAndWrite("build_onnx", &value->build_onnx, path, row);
}

VOID ConfigModule::GetParame(char* path) {
	g_logger->info("��ȡ{}�Ĳ���", path);
	if (!CheckConfigFile(path)) return;		// û��config�ļ�������������conf����0����
	OperationParame(CONFIG_READ,path);
}

VOID ConfigModule::SaveParame() {
	char* path;
	GetConfigPath(Config_name, &path);
	CreateConfigFile(path);
	OperationParame(CONFIG_WRITE, path);
}