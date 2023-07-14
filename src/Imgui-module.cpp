#include "Imgui-module.h"
#include "sf-trt.h"


LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return TRUE;
	switch (msg) {
	case WM_SIZE:

		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // ���� ALT Ӧ�ó���˵�
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

VOID ImguiModule::CreateRanderBuffer() {
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	if (pBackBuffer != NULL)
		g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

VOID ImguiModule::CleanupRenderBuffer() {
	if (g_mainRenderTargetView) {
		g_mainRenderTargetView->Release();
		g_mainRenderTargetView = NULL;
	}
}

VOID ImguiModule::CloseD3D() {
	ReleaseCom(g_pd3dDevice);
	ReleaseCom(g_pd3dDeviceContext);
	ReleaseCom(g_pSwapChain);
	ReleaseCom(g_mainRenderTargetView);
	DestroyWindow(hwnd);
	g_logger->info("imgui�ͷ� PASS...");
}

BOOL ImguiModule::CreateImguiWinClass() {
	// ע�ᴰ����		Create winClass
	WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"Temp Example", NULL };
	RegisterClassExW(&wc);
	hwnd = CreateWindow(wc.lpszClassName, L"SF_TRT 6.1", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

	//��ʼ��d3d		Init D3D
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	HRESULT res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
		res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_WARP, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res != S_OK) {
		CleanupRenderBuffer();
		if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
		if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
		if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
		g_logger->warn("D3D11CreateDeviceAndSwapChainʧ��");
		return FALSE;
	}
	return TRUE;
}

BOOL ImguiModule::InitImguiMdule() {
	if (!CreateImguiWinClass()) return FALSE;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// ����imgui��IO		Set the IO of imgui
	io = &ImGui::GetIO(); 
	(void*)io;						
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;		
	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;		
	io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;			
	io->IniFilename = NULL;
	io->ConfigViewportsNoAutoMerge = TRUE;

	ImFont* font = io->Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\simhei.ttf", 16.f, NULL, io->Fonts->GetGlyphRangesChineseFull());
	ImGuiStyle& style = ImGui::GetStyle();

	if (io->ConfigFlags == ImGuiConfigFlags_ViewportsEnable) {
		style.Colors[ImGuiCol_WindowBg].w = 10.0f;	
		style.WindowRounding = 10.0f;				
	}

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);


	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);			
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.5);			
	ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 10.f);				
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5.f);			

	SF::FileBrowser* filebrowser = &SF::FileBrowser::Get();
	filebrowser->build_onnx_dialog.SetTypeFilters({ ".onnx",".sftrt" });
	filebrowser->build_onnx_dialog.SetTitle("onnx##ONNX�ļ������");

	filebrowser->cpu_onnx_dialog.SetTypeFilters({ ".onnx" });
	filebrowser->cpu_onnx_dialog.SetTitle("onnx##cpuONNX�ļ������");

	filebrowser->engine_dialog.SetTypeFilters({ ".engine" ,"trt" });
	filebrowser->engine_dialog.SetTitle("engien##engien�ļ������");

	filebrowser->config_dialog.SetTypeFilters({ ".ini" });
	filebrowser->config_dialog.SetTitle("config##config�ļ������");

	return TRUE;
}

VOID ImguiModule::SetNextFrame() {
	MSG msg{};
	while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT) {
			g_logger->info("imgui�����ڹر�...");
		}
	}


	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

VOID ImguiModule::PresentFrame() {
	ImGui::Render();
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);	//��ȡ����
	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);	// ��ʼ������
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());	// ��ȡImGui�Ŀؼ�Buff
	if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();	//�󶨵�����
	}

	g_pSwapChain->Present(1, 0); 
}

VOID ImguiModule::RenderImguiLayout(VOID (*fun)()) {
	SetNextFrame();
	ImGui::Begin(u8"SF_TRT 6.1 Bate", &SF::Signl::Get().ImGuiWinStop, ImGuiWindowFlags_NoCollapse + ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoSavedSettings);
	fun();
	ImGui::End();
	PresentFrame();
}

