#include "sf-trt.h"
#include "Imgui-module.h"
#include "config-module.h"
#include "tensorrt-module.h"
#include "build-module.h"
#include "examine-module.h"
#include "dml-module.h"

static const char* theme_items_name[3] = { u8"深蓝",u8"暗紫",u8"蓝白" };
static const char* theme_items_english_name[3] = { u8"Blue",u8"Purple",u8"White" };
static const char* language_ui_name[2] = { u8"中文",u8"English" };

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
	ImGui::Text(GetLanguage() ? u8"性能:[循环:%-3d 截图:%-3d 推理:%-3d FPS:%-4d]" : "Performance:[Loop:%-3d Capture:%-3d Pred:%-3d FPS:%-4d]",
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

	if (IButton(u8"加载config", "Load Config"))
		SF::FileBrowser::Get().config_dialog.Open();
	ExplanationMake(u8"启动加载一次config文件", "Start Loading a Config File");

	if (!signl->KeepSave) {
		ImGui::SameLine();
		if (IButton(u8"保存参数", "Save Parame")) {
			ConfigModule::Get().SaveParame();
		}
		ExplanationMake(u8"保存参数到Config.ini", "Save Parameters To Config.ini");
	}
	else {
		ConfigModule::Get().SaveParame();
	}

	ImGui::SameLine();
	ICheckbox(u8"实时保存", "Keep Save", &signl->KeepSave);
	ExplanationMake(u8"cpu开销增大", "Increased Cpu Overhead");


	ImGui::SetNextItemWidth(150);
	ImGui::InputTextWithHint(GetLanguage() ? u8"Config文件名" : u8"Save Config Name", GetLanguage() ? u8"示例:Config.ini" : u8"Example:Config.ini", ConfigModule::Get().Config_name, 20);
	ExplanationMake(u8"保存到指定的ini文件，默认为Config.ini,ASCII编码", "Save to the specified ini file, the default is Config.ini,ASCII encoding");
	

	ImGui::SetNextItemWidth(80);
	if (GetLanguage())
		ImGui::Combo(u8"主题", &variables->themeitems, theme_items_name, IM_ARRAYSIZE(theme_items_name));
	else
		ImGui::Combo("Theme", &variables->themeitems, theme_items_english_name, IM_ARRAYSIZE(theme_items_english_name));
	switch (variables->themeitems) {
	case 0:ImGui::StyleColorsDark(); break;
	case 1:ImGui::StyleColorsClassic(); break;
	case 2:ImGui::StyleColorsLight(); break;
	}

	ImGui::SameLine();
	ImGui::SetNextItemWidth(85);
	ImGui::Combo(GetLanguage() ? u8"语言" : "Language", &variables->language, language_ui_name, IM_ARRAYSIZE(language_ui_name));

	// Tips
	ExplanationMake(u8"Tips: Ctrl + 左键可直接修改滑块数值", u8"·Ctrl + left can directly modify the slider value", FALSE);

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
	ImGui::Text(u8"※ ");
	ImGui::SameLine();
	ICheckbox(u8"推理设置", "Inference Settings", &variables->infer_ui);
	if (!variables->infer_ui) return;

	if (signl->ThreadReady) {
		// 后端
		ImGui::RadioButton(GetLanguage() ? u8"CUDA" : "CUDA", &variables->inference_backend, 0);
		ImGui::SameLine();
		ImGui::RadioButton(GetLanguage() ? u8"DML" : "DML", &variables->inference_backend, 1);
		if (variables->inference_backend == 0) {
			// 多GPU选择，最多支持4个gpu
			if (variables->gpu_number > 1) {
				ImGui::Text(GetLanguage() ? "指定GPU" : "Designated GPU");
				for (int i = 0; i < variables->gpu_number; ++i) {
					ImGui::SameLine();
					ImGui::RadioButton((char*)std::to_string(i).c_str(), &value->gpuidx, i);
				}
				ExplanationMake(u8"指定GPU参与计算,英伟达显卡索引,最多支持x4", "Designated GPU to participate in computing, Nvidia graphics card index,Max index x4");
			}
		}

		if (variables->inference_backend == 0) {
			ImGui::SetNextItemWidth(300);
			ImGui::InputText(u8"##engine_path", value->engine_path, MAX_PATH);	// 只作显示用途，无输入作用
			ImGui::SameLine();
			if (IButton(u8"浏览##engine", "Browse##engine"))
				SF::FileBrowser::Get().engine_dialog.Open();
		}
		else {
			ImGui::SetNextItemWidth(300);
			ImGui::InputText(u8"##onnx_path_cpu", (char*)StringToUTF8(value->cpu_onnx).c_str(), MAX_PATH);
			ImGui::SameLine();
			if (IButton(u8"浏览##onnx_cpu", "Browse##onnx_cpu"))
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
		ImGui::RadioButton(u8"易键鼠", &variables->move_way, 1);
		ImGui::SameLine();
		ImGui::RadioButton(u8"SendInput", &variables->move_way, 2);
		ImGui::SameLine();
		ImGui::RadioButton(GetLanguage() ? u8"自定义##移动方式" : u8"Customize", &variables->move_way, 3);

		if (IButton(u8"创建本地线程", "Create Native Thread")) {
			SFFramework();
		}

	}else{
		// show pred window
		ICheckbox(u8"显示推理窗口", "Show Forecast Window", &signl->ShowWindow);
		// show performance
		IText2(5, 1, 2, 200);
		if (IButton(u8"退出线程", "Exit Thread")) {
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
	ImGui::Text(u8"※ ");
	ImGui::SameLine();
	ICheckbox(u8"按键设置", "Set Key", &variables->key_ui);
	if (!variables->key_ui) return;

	if (signl->AimSwitch == TRUE) {
		IPushID(u8"状态:ON ", "State:ON ");
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(0.f / 255, 255.f / 255, 0.0f / 255));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(0.f / 255, 225.f / 255, 0.0f / 255));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(0.f / 255, 155.f / 255, 0.0f / 255));
		if (IButton(u8"状态:ON ", "State:ON "))
			signl->AimSwitch = FALSE;
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	} else {
		IPushID(u8"状态:OFF", "State:OFF");
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(255.f / 255, 0.0f / 255, 0.0f / 255));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(225.f / 255, 0.0f / 255, 0.0f / 255));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(155.f / 255, 0.0f / 255, 0.0f / 255));
		if (IButton(u8"状态:OFF", "State:OFF"))
			signl->AimSwitch = TRUE;
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}

	IRadioButton(u8"左", u8"Left", &value->lock_key, 0x01);
	IRadioButton(u8"右", u8"Right", &value->lock_key, 0x02);
	IRadioButton(u8"侧", u8"Side", &value->lock_key, 0x05);

	ImGui::SetNextItemWidth(30);	//设置宽度
	ImGui::SameLine();
	IInputScalar(u8"自定义##按键2", u8"Customize", &value->lock_key2);
	ExplanationMake(u8"自定义触发按键,对应虚拟键表的十进制数,0为不使用", "Custom trigger button, corresponding to the decimal number of the virtual key table, 0 means not used");

	ICheckbox(u8"扳机", "Auto Fire", &value->auto_fire);
	if (value->auto_fire == TRUE) {
		ImGui::SetNextItemWidth(40);	//设置宽度
		IInputScalar(u8"间隔", "Firing Interval", &value->auto_interval);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(40);	//设置宽度
		IInputScalar(u8"随机", "Random", &value->auto_random);

		IRadioButton(u8"方式1", "Model 1", &value->auto_model, 0);
		IRadioButton(u8"方式2", "Model 2", &value->auto_model, 1);
		ExplanationMake(u8"方式1:先移动至目标范围内再开枪，方式2:不移动,在目标范围内开枪", u8"Method 1: Move to the target range before shooting, Method 2: Do not move, shoot within the target range");
	}
}

static VOID ParameUI() {
	SF::Variables* variables = &SF::Variables::Get();
	SF::Value* value = &SF::Value::Get();
	SF::Signl* signl = &SF::Signl::Get();

	ImGui::Separator();
	ImGui::Separator();
	ImGui::Text(u8"※ ");
	ImGui::SameLine();
	ICheckbox(u8"参数设置", "Set Parame", &variables->parame_ui);
	if (!variables->parame_ui) return;

	ISliderFloat(u8"置信度", "Confidence", &value->confidence, 0.1f, 1.f);
	ISliderFloat(u8"位置", "Location", &value->location, 0.f, 1.f);
	ISliderInt(u8"最大范围", "Max Range", &value->effectiverange, 0, variables->max_range);
	ImGui::SetNextItemWidth(40);
	IInputScalar(u8"最大像素距离", "Max Pixel", &value->max_pixels);

	// 类别选择	
	ICheckbox(u8"模型类别", "Model Class", &signl->ModelClasses);
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
	ImGui::Text(u8"※ ");
	ImGui::SameLine();
	ICheckbox(u8"PID控制", "PID Control", &variables->pid_ui);
	if (!variables->pid_ui)	return;

	IText(u8"·	X轴", u8"·	X Axis");
	ImGui::SliderFloat(u8"P##P_X", &value->P_x, 0.f, 1.f);
	ImGui::SliderFloat(u8"I##I_X", &value->I_x, 0.f, 1.f);
	ImGui::SliderFloat(u8"P##D_X", &value->D_x, 0.f, 1.f);

	IText(u8"·	Y轴", u8"·	Y Axis");
	ImGui::SliderFloat(u8"P##P_Y", &value->P_y, 0.f, 1.f);
	ImGui::SliderFloat(u8"I##I_Y", &value->I_y, 0.f, 1.f);
	ImGui::SliderFloat(u8"P##D_Y", &value->D_y, 0.f, 1.f);
}

static VOID FovUI() {
	SF::Variables* variables = &SF::Variables::Get();
	SF::Value* value = &SF::Value::Get();

	ImGui::Separator();
	ImGui::Separator();
	ImGui::Text(u8"※ ");
	ImGui::SameLine();
	ICheckbox(u8"FOV算法", "FOV Algorithm", &variables->fov_algorithm);
	if (!variables->fov_algorithm) return;

	// &&&占位符
	if (variables->fov_algorithm) {
		IFovInput(u8"HFOV", &value->hfov);
		IFovPixelInput(u8"X轴像素", &value->game_x_pixel);

		IFovInput(u8"VFOV", &value->vfov);
		IFovPixelInput(u8"Y轴像素", &value->game_y_pixel);
		ImGui::SameLine();
		if (IButton(u8"推导", u8"Derive")) {
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
	ImGui::Text(u8"※ ");
	ImGui::SameLine();
	ICheckbox(u8"构建engine", "Build Engine", &variables->BuildEngineUI);
	if (!variables->BuildEngineUI) return;

	ImGui::RadioButton("FP16", &value->precision, 0);
	ImGui::SameLine();
	ImGui::RadioButton("FP32", &value->precision, 1);

	ICheckbox(u8"模型是加密？", u8"Module Is Encryption?", &variables->EncryptionModule);

	ImGui::InputText(u8"##onnx_path", value->build_onnx, MAX_PATH);
	ImGui::SameLine();
	if (IButton(u8"浏览##onnx", "Browse##onnx"))
		SF::FileBrowser::Get().build_onnx_dialog.Open();

	if (signl->BuildThreadReady) {
		if (IButton(u8"生成", "Build")) {
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

	ImGui::Text(u8"> ！！！此为免费程序！！！");
	ImGui::Text(u8"> 本程序只在交流群内发布");
	ImGui::Text(u8"> 且仅有一个交流群:883453442");
	ImGui::Text(u8"> 须知,若因收费获得,你以被诈骗");
	ImGui::Text(u8"> 本程序仅做交流学习,请勿用于非法用途");
	ImGui::Text(u8"> 且不对使用中产生的后果负责");
	ImGui::Text(u8"> bilibili:随风而息");
	ImGui::Text(u8"> UID:120366874");

	ImGui::SetNextItemWidth(180);	//设置宽度
	ImGui::InputTextWithHint(u8"##verify", u8"登录验证", examine->examine_key, MAX_PATH);
	ImGui::SameLine();	//不换行

	if (ImGui::Button(u8"接受以上声明")) {
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

	// imgui 初始化
	if (!imgui_module->InitImguiMdule()) return;

	// 自动加载一次参数
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
