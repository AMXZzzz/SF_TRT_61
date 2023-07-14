#include "sf-trt.h"
#include "Imgui-module.h"
#include "config-module.h"
#include "tensorrt-module.h"
#include "build-module.h"
#include "examine-module.h"
#include "dml-module.h"

static const char* theme_items_name[3] = { u8"����",u8"����",u8"����" };
static const char* theme_items_english_name[3] = { u8"Blue",u8"Purple",u8"White" };
static const char* language_ui_name[2] = { u8"����",u8"English" };

static inline BOOL GetLanguage() {
	return SF::Variables::Get().language == 0;
}
static BOOL IButton(char* label, char* label_engine) {
	return ImGui::Button(GetLanguage() ? label : label_engine);
}
static VOID ICheckbox(char* label, char* label_engine, bool* v) {
	ImGui::Checkbox(GetLanguage() ? label : label_engine, v);
}
static VOID ISliderFloat(char* label, char* label_engine, float* v, float v_min, float v_max) {
	ImGui::SliderFloat(GetLanguage() ? label : label_engine, v, v_min, v_max);
}
static VOID ISliderInt(char* label, char* label_engine, int* v, int v_min, int v_max) {
	ImGui::SliderInt(GetLanguage() ? label : label_engine, v, v_min, v_max);
}
static VOID IInputScalar(char* label, char* label_engine, void* p_data) {
	ImGui::InputScalar(GetLanguage() ? label : label_engine, ImGuiDataType_S32, p_data, NULL, NULL, "%d", NULL);
}
static VOID IText(char* label, char* label_engine) {
	ImGui::Text(GetLanguage() ? label : label_engine);
}
static VOID IText2(int loop,int cap, int pred, int fps) {
	ImGui::Text(GetLanguage() ? u8"����:[ѭ��:%-3d ��ͼ:%-3d ����:%-3d FPS:%-4d]" : "Performance:[Loop:%-3d Capture:%-3d Pred:%-3d FPS:%-4d]",
		loop, cap, pred, fps);
}
static VOID IFovInput(char* label,float* val) {
	ImGui::SetNextItemWidth(80);
	ImGui::InputFloat(label,val);
}
static VOID IFovPixelInput(char* label, int* val) {
	ImGui::SameLine();
	ImGui::SetNextItemWidth(80);
	ImGui::InputScalar(label, ImGuiDataType_S32, val, NULL, NULL, "%d", NULL);
}
static VOID IRadioButton(char* label, char* label_engine,int* key,int val) {
	ImGui::SameLine();
	ImGui::RadioButton(GetLanguage() ? label : label_engine, key, val);
}
static VOID IPushID(char* label, char* label_engine) {
	ImGui::PushID(GetLanguage() ? label : label_engine);
}

static inline VOID ClassChoose(int& num, char* label, bool* v) {
	if (num) {
		ImGui::SameLine();
		ImGui::Checkbox(label, v);
		num--;
	}
}
static VOID ExplanationMake(char* strdata, char* strdata2, BOOL Same = TRUE) {
	if (Same)
		ImGui::SameLine();
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered()) {
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 25.f);
		ImGui::TextUnformatted(GetLanguage() ? strdata : strdata2);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

static VOID BaseUI() {
	SF::Signl* signl = &SF::Signl::Get();
	SF::Variables* variables = &SF::Variables::Get();
	ImGui::Separator();
	ImGui::Separator();

	if (IButton(u8"����config", "Load Config"))
		SF::FileBrowser::Get().config_dialog.Open();
	ExplanationMake(u8"��������һ��config�ļ�", "Start Loading a Config File");

	if (!signl->KeepSave) {
		ImGui::SameLine();
		if (IButton(u8"�������", "Save Parame")) {
			ConfigModule::Get().SaveParame();
		}
		ExplanationMake(u8"���������Config.ini", "Save Parameters To Config.ini");
	}
	else {
		ConfigModule::Get().SaveParame();
	}

	ImGui::SameLine();
	ICheckbox(u8"ʵʱ����", "Keep Save", &signl->KeepSave);
	ExplanationMake(u8"cpu��������", "Increased Cpu Overhead");


	ImGui::SetNextItemWidth(150);
	ImGui::InputTextWithHint(GetLanguage() ? u8"Config�ļ���" : u8"Save Config Name", GetLanguage() ? u8"ʾ��:Config.ini" : u8"Example:Config.ini", ConfigModule::Get().Config_name, 20);
	ExplanationMake(u8"���浽ָ����ini�ļ���Ĭ��ΪConfig.ini,ASCII����", "Save to the specified ini file, the default is Config.ini,ASCII encoding");
	

	ImGui::SetNextItemWidth(80);
	if (GetLanguage())
		ImGui::Combo(u8"����", &variables->themeitems, theme_items_name, IM_ARRAYSIZE(theme_items_name));
	else
		ImGui::Combo("Theme", &variables->themeitems, theme_items_english_name, IM_ARRAYSIZE(theme_items_english_name));
	switch (variables->themeitems) {
	case 0:ImGui::StyleColorsDark(); break;
	case 1:ImGui::StyleColorsClassic(); break;
	case 2:ImGui::StyleColorsLight(); break;
	}

	ImGui::SameLine();
	ImGui::SetNextItemWidth(85);
	ImGui::Combo(GetLanguage() ? u8"����" : "Language", &variables->language, language_ui_name, IM_ARRAYSIZE(language_ui_name));

	// Tips
	ExplanationMake(u8"Tips: Ctrl + �����ֱ���޸Ļ�����ֵ", u8"��Ctrl + left can directly modify the slider value", FALSE);

}
static VOID SFFramework() {
	SF::Variables* variables = &SF::Variables::Get();

	switch (variables->inference_backend) {
	case 0:
		TensorRtFrame();
		break;
	case 1:
		DMLFrame();
		break;
	}
}


static VOID LaunchUI() {
	SF::Variables* variables = &SF::Variables::Get();
	SF::Value* value = &SF::Value::Get();
	SF::Signl* signl = &SF::Signl::Get();

	ImGui::Separator();
	ImGui::Separator();
	ImGui::Text(u8"�� ");
	ImGui::SameLine();
	ICheckbox(u8"��������", "Inference Settings", &variables->infer_ui);
	if (!variables->infer_ui) return;

	if (signl->ThreadReady) {
		// ���
		ImGui::RadioButton(GetLanguage() ? u8"CUDA" : "CUDA", &variables->inference_backend, 0);
		ImGui::SameLine();
		ImGui::RadioButton(GetLanguage() ? u8"DML" : "DML", &variables->inference_backend, 1);
		if (variables->inference_backend == 0) {
			// ��GPUѡ�����֧��4��gpu
			if (variables->gpu_number > 1) {
				ImGui::Text(GetLanguage() ? "ָ��GPU" : "Designated GPU");
				for (int i = 0; i < variables->gpu_number; ++i) {
					ImGui::SameLine();
					ImGui::RadioButton((char*)std::to_string(i).c_str(), &value->gpuidx, i);
				}
				ExplanationMake(u8"ָ��GPU�������,Ӣΰ���Կ�����,���֧��x4", "Designated GPU to participate in computing, Nvidia graphics card index,Max index x4");
			}
		}

		if (variables->inference_backend == 0) {
			ImGui::SetNextItemWidth(300);
			ImGui::InputText(u8"##engine_path", value->engine_path, MAX_PATH);	// ֻ����ʾ��;������������
			ImGui::SameLine();
			if (IButton(u8"���##engine", "Browse##engine"))
				SF::FileBrowser::Get().engine_dialog.Open();
		}
		else {
			ImGui::SetNextItemWidth(300);
			ImGui::InputText(u8"##onnx_path_cpu", (char*)StringToUTF8(value->cpu_onnx).c_str(), MAX_PATH);
			ImGui::SameLine();
			if (IButton(u8"���##onnx_cpu", "Browse##onnx_cpu"))
				SF::FileBrowser::Get().cpu_onnx_dialog.Open();
		}


		ImGui::RadioButton(u8"YOLOV5/7", &variables->process_frame, 0);
		ImGui::SameLine();
		ImGui::RadioButton(u8"YOLOX", &variables->process_frame, 1);
		ImGui::SameLine();
		ImGui::RadioButton(u8"YOLOV8", &variables->process_frame, 2);

		// way of moveing
		ImGui::RadioButton(u8"GHUB", &variables->move_way, 0);
		ImGui::SameLine();
		ImGui::RadioButton(u8"�׼���", &variables->move_way, 1);
		ImGui::SameLine();
		ImGui::RadioButton(u8"SendInput", &variables->move_way, 2);
		ImGui::SameLine();
		ImGui::RadioButton(GetLanguage() ? u8"�Զ���##�ƶ���ʽ" : u8"Customize", &variables->move_way, 3);

		if (IButton(u8"���������߳�", "Create Native Thread")) {
			SFFramework();
		}

	}else{
		// show pred window
		ICheckbox(u8"��ʾ������", "Show Forecast Window", &signl->ShowWindow);
		// show performance
		IText2(5, 1, 2, 200);
		if (IButton(u8"�˳��߳�", "Exit Thread")) {
			signl->ThreadStopSignl = FALSE;
		}
	}
}

static VOID KeyUI() {
	SF::Variables* variables = &SF::Variables::Get();
	SF::Value* value = &SF::Value::Get();
	SF::Signl* signl = &SF::Signl::Get();

	ImGui::Separator();
	ImGui::Separator();
	ImGui::Text(u8"�� ");
	ImGui::SameLine();
	ICheckbox(u8"��������", "Set Key", &variables->key_ui);
	if (!variables->key_ui) return;

	if (signl->AimSwitch == TRUE) {
		IPushID(u8"״̬:ON ", "State:ON ");
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(0.f / 255, 255.f / 255, 0.0f / 255));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(0.f / 255, 225.f / 255, 0.0f / 255));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(0.f / 255, 155.f / 255, 0.0f / 255));
		if (IButton(u8"״̬:ON ", "State:ON "))
			signl->AimSwitch = FALSE;
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	} else {
		IPushID(u8"״̬:OFF", "State:OFF");
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(255.f / 255, 0.0f / 255, 0.0f / 255));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(225.f / 255, 0.0f / 255, 0.0f / 255));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(155.f / 255, 0.0f / 255, 0.0f / 255));
		if (IButton(u8"״̬:OFF", "State:OFF"))
			signl->AimSwitch = TRUE;
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}

	IRadioButton(u8"��", u8"Left", &value->lock_key, 0x01);
	IRadioButton(u8"��", u8"Right", &value->lock_key, 0x02);
	IRadioButton(u8"��", u8"Side", &value->lock_key, 0x05);

	ImGui::SetNextItemWidth(30);	//���ÿ��
	ImGui::SameLine();
	IInputScalar(u8"�Զ���##����2", u8"Customize", &value->lock_key2);
	ExplanationMake(u8"�Զ��崥������,��Ӧ��������ʮ������,0Ϊ��ʹ��", "Custom trigger button, corresponding to the decimal number of the virtual key table, 0 means not used");

	ICheckbox(u8"���", "Auto Fire", &value->auto_fire);
	if (value->auto_fire == TRUE) {
		ImGui::SetNextItemWidth(40);	//���ÿ��
		IInputScalar(u8"���", "Firing Interval", &value->auto_interval);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(40);	//���ÿ��
		IInputScalar(u8"���", "Random", &value->auto_random);

		IRadioButton(u8"��ʽ1", "Model 1", &value->auto_model, 0);
		IRadioButton(u8"��ʽ2", "Model 2", &value->auto_model, 1);
		ExplanationMake(u8"��ʽ1:���ƶ���Ŀ�귶Χ���ٿ�ǹ����ʽ2:���ƶ�,��Ŀ�귶Χ�ڿ�ǹ", u8"Method 1: Move to the target range before shooting, Method 2: Do not move, shoot within the target range");
	}
}

static VOID ParameUI() {
	SF::Variables* variables = &SF::Variables::Get();
	SF::Value* value = &SF::Value::Get();
	SF::Signl* signl = &SF::Signl::Get();

	ImGui::Separator();
	ImGui::Separator();
	ImGui::Text(u8"�� ");
	ImGui::SameLine();
	ICheckbox(u8"��������", "Set Parame", &variables->parame_ui);
	if (!variables->parame_ui) return;

	ISliderFloat(u8"���Ŷ�", "Confidence", &value->confidence, 0.1f, 1.f);
	ISliderFloat(u8"λ��", "Location", &value->location, 0.f, 1.f);
	ISliderInt(u8"���Χ", "Max Range", &value->effectiverange, 0, variables->max_range);
	ImGui::SetNextItemWidth(40);
	IInputScalar(u8"������ؾ���", "Max Pixel", &value->max_pixels);

	// ���ѡ��	
	ICheckbox(u8"ģ�����", "Model Class", &signl->ModelClasses);
	if (signl->ModelClasses) {
		int class_num_temp = value->class_number;
		ClassChoose(class_num_temp, u8"0", &value->class0);
		ClassChoose(class_num_temp, u8"1", &value->class1);
		ClassChoose(class_num_temp, u8"2", &value->class2);
		ClassChoose(class_num_temp, u8"3", &value->class3);
	}
}

static VOID PidUI() {
	SF::Variables* variables = &SF::Variables::Get();
	SF::Value* value = &SF::Value::Get();

	ImGui::Separator();
	ImGui::Separator();
	ImGui::Text(u8"�� ");
	ImGui::SameLine();
	ICheckbox(u8"PID����", "PID Control", &variables->pid_ui);
	if (!variables->pid_ui)	return;

	IText(u8"��	X��", u8"��	X Axis");
	ImGui::SliderFloat(u8"P##P_X", &value->P_x, 0.f, 1.f);
	ImGui::SliderFloat(u8"I##I_X", &value->I_x, 0.f, 1.f);
	ImGui::SliderFloat(u8"P##D_X", &value->D_x, 0.f, 1.f);

	IText(u8"��	Y��", u8"��	Y Axis");
	ImGui::SliderFloat(u8"P##P_Y", &value->P_y, 0.f, 1.f);
	ImGui::SliderFloat(u8"I##I_Y", &value->I_y, 0.f, 1.f);
	ImGui::SliderFloat(u8"P##D_Y", &value->D_y, 0.f, 1.f);
}

static VOID FovUI() {
	SF::Variables* variables = &SF::Variables::Get();
	SF::Value* value = &SF::Value::Get();

	ImGui::Separator();
	ImGui::Separator();
	ImGui::Text(u8"�� ");
	ImGui::SameLine();
	ICheckbox(u8"FOV�㷨", "FOV Algorithm", &variables->fov_algorithm);
	if (!variables->fov_algorithm) return;

	// &&&ռλ��
	if (variables->fov_algorithm) {
		IFovInput(u8"HFOV", &value->hfov);
		IFovPixelInput(u8"X������", &value->game_x_pixel);

		IFovInput(u8"VFOV", &value->vfov);
		IFovPixelInput(u8"Y������", &value->game_y_pixel);
		ImGui::SameLine();
		if (IButton(u8"�Ƶ�", u8"Derive")) {
			//value->game_y_pixel = (value->hfov * value->game_x_pixel) / value->vfov;
			value->game_y_pixel = (value->vfov / value->hfov) * value->game_x_pixel;
		}
	}
}

static VOID BuildEngineUI() {
	SF::Variables* variables = &SF::Variables::Get();
	SF::Value* value = &SF::Value::Get();
	SF::Signl* signl = &SF::Signl::Get();

	ImGui::Separator();
	ImGui::Separator();
	ImGui::Text(u8"�� ");
	ImGui::SameLine();
	ICheckbox(u8"����engine", "Build Engine", &variables->BuildEngineUI);
	if (!variables->BuildEngineUI) return;

	ImGui::RadioButton("FP16", &value->precision, 0);
	ImGui::SameLine();
	ImGui::RadioButton("FP32", &value->precision, 1);

	ICheckbox(u8"ģ���Ǽ��ܣ�", u8"Module Is Encryption?", &variables->EncryptionModule);

	ImGui::InputText(u8"##onnx_path", value->build_onnx, MAX_PATH);
	ImGui::SameLine();
	if (IButton(u8"���##onnx", "Browse##onnx"))
		SF::FileBrowser::Get().build_onnx_dialog.Open();

	if (signl->BuildThreadReady) {
		if (IButton(u8"����", "Build")) {
			BuildEngine();
		}
	}
	else {
		ImGui::Text("Building Engine .... ");
	}
}

static VOID OpenFileManager() {
	SF::FileBrowser* filebrowser = &SF::FileBrowser::Get();
	SF::Value* value = &SF::Value::Get();

	filebrowser->config_dialog.Display();
	if (filebrowser->config_dialog.HasSelected()) {
		ConfigModule::Get().GetParame((char*)filebrowser->config_dialog.GetSelected().string().c_str());
		filebrowser->config_dialog.ClearSelected();
	}

	filebrowser->build_onnx_dialog.Display();
	if (filebrowser->build_onnx_dialog.HasSelected()) {
		//strcpy_s(value->build_onnx, StringToUTF8(filebrowser->build_onnx_dialog.GetSelected().string()).c_str());
		strcpy_s(value->build_onnx, (filebrowser->build_onnx_dialog.GetSelected().string()).c_str());
		filebrowser->build_onnx_dialog.ClearSelected();
	}

	filebrowser->cpu_onnx_dialog.Display();
	if (filebrowser->cpu_onnx_dialog.HasSelected()) {
		strcpy_s(value->cpu_onnx, (filebrowser->cpu_onnx_dialog.GetSelected().string()).c_str());
		filebrowser->cpu_onnx_dialog.ClearSelected();
	}

	filebrowser->engine_dialog.Display();
	if (filebrowser->engine_dialog.HasSelected()) {
		strcpy_s(value->engine_path, StringToUTF8(filebrowser->engine_dialog.GetSelected().string()).c_str());
		filebrowser->engine_dialog.ClearSelected();
	}
}
static VOID ExamineUI() {
	Examine* examine = &Examine::Get();
	SF::Signl* signl = &SF::Signl::Get();
	SF::Variables* variables = &SF::Variables::Get();

	ImGui::Text(u8"> ��������Ϊ��ѳ��򣡣���");
	ImGui::Text(u8"> ������ֻ�ڽ���Ⱥ�ڷ���");
	ImGui::Text(u8"> �ҽ���һ������Ⱥ:883453442");
	ImGui::Text(u8"> ��֪,�����շѻ��,���Ա�թƭ");
	ImGui::Text(u8"> �������������ѧϰ,�������ڷǷ���;");
	ImGui::Text(u8"> �Ҳ���ʹ���в����ĺ������");
	ImGui::Text(u8"> bilibili:����Ϣ");
	ImGui::Text(u8"> UID:120366874");

	ImGui::SetNextItemWidth(180);	//���ÿ��
	ImGui::InputTextWithHint(u8"##verify", u8"��¼��֤", examine->examine_key, MAX_PATH);
	ImGui::SameLine();	//������

	if (ImGui::Button(u8"������������")) {
		if (examine->GetExamine()) {		
			signl->ExamineState = TRUE;
		}
	}
}

static VOID ImguiLayout() {
	BaseUI();
	LaunchUI();
	KeyUI();
	ParameUI();
	PidUI();
	FovUI();
	BuildEngineUI();
	OpenFileManager();
}

static VOID start_main() {
	ImguiModule* imgui_module = &ImguiModule::Get();
	SF::Signl* signl = &SF::Signl::Get();
	Examine* examine = &Examine::Get();
	ConfigModule* config = &ConfigModule::Get();

	// imgui ��ʼ��
	if (!imgui_module->InitImguiMdule()) return;

	// �Զ�����һ�β���
	config->GetParame((char*)GetIniPath().c_str());

	// run
	while (signl->ImGuiWinStop) {
		if (signl->ExamineState)
			imgui_module->RenderImguiLayout(ImguiLayout);
		else 
			imgui_module->RenderImguiLayout(ExamineUI);
	}
	imgui_module->CloseD3D();
}

int main() {

	InitSpdLog();
	start_main();
	return 0;
}
