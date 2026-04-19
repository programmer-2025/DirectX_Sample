#include <Windows.h>
#include "DirectX3D.h"
#include "Input.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_win32.h"
#include "ImGUI/imgui_impl_dx11.h"
#include <d3dcompiler.h>
#define WINDOW_CLASS_NAME L"DirectX_Sample"

#pragma comment(lib, "dxgi.lib")

namespace {
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D11InputLayout* inputLayout = nullptr;
	ID3D11Buffer* vertexBuffer = nullptr;

	struct Vertex {
		float x, y, z;   
		float r, g, b, a; 
	};
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

using namespace DirectX3D;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam))
		return true;

	switch (message) {
		case WM_CREATE: {
			break;
		}
		case WM_COMMAND: {
			break;
		}
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

int initializeImGUI(HWND hwnd) {
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hwnd);
	ID3D11Device* device = (ID3D11Device*)DirectX3D::d3d11Device_;
	ID3D11DeviceContext* deviceContext = (ID3D11DeviceContext*)DirectX3D::d3d11Context_;
	ImGui_ImplDX11_Init(device, deviceContext);

	return 0;
}

void initializeShader() {
	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;
	HRESULT result = {};

	result = D3DCompileFromFile(
		L"TestPixelShader.hlsl", 
		nullptr, 
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", 
		"ps_5_0", 
		D3DCOMPILE_ENABLE_STRICTNESS,
		0, 
		&psBlob, 
		NULL
	);

	result = D3DCompileFromFile(
		L"TestVertexShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"vs_5_0",
		D3DCOMPILE_ENABLE_STRICTNESS,
		0,
		&vsBlob,
		NULL
	);

	result = DirectX3D::d3d11Device_->CreateVertexShader(
		vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
		NULL, 
		&vertexShader);

	result = DirectX3D::d3d11Device_->CreatePixelShader(
		psBlob->GetBufferPointer(), psBlob->GetBufferSize(),
		NULL,
		&pixelShader);

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                          D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, UINT(3 * sizeof(float)),    D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	result = DirectX3D::d3d11Device_->CreateInputLayout(
		layout,
		ARRAYSIZE(layout),
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&inputLayout
	);
	assert(SUCCEEDED(result));

	Vertex vertices[3] = {
		{  0.0f,  0.5f, 0.0f, 1, 0, 0, 1 }, // 上：赤
		{  0.5f, -0.5f, 0.0f, 0, 1, 0, 1 }, // 右下：緑
		{ -0.5f, -0.5f, 0.0f, 0, 0, 1, 1 }, // 左下：青
	};

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;

	result = d3d11Device_->CreateBuffer(&bd, &initData, &vertexBuffer);
}

int initializeWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	WNDCLASSEXW wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEXW);
	wndClass.hInstance = hInstance;
	wndClass.lpszClassName = WINDOW_CLASS_NAME;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);	
	wndClass.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);	
	wndClass.lpszMenuName = nullptr;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	RegisterClassEx(&wndClass);

	HWND hwnd = CreateWindow(
		WINDOW_CLASS_NAME,
		L"Game",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, //X座標＆Y座標
		CW_USEDEFAULT, CW_USEDEFAULT, //幅＆高さ
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hwnd) {
		return -1;
	}

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	if (DirectX3D::initializeDevice(hwnd) == -1) {
		MessageBox(hwnd, L"DirectXの初期化に失敗しました", L"エラー", MB_OK);
		return -1;
	}
	if (Input::initialize(hInstance, hwnd) == -1) {
		MessageBox(hwnd, L"入力装置の初期化に失敗しました。", L"エラー", MB_OK);
		return -1;
	}
	if (initializeImGUI(hwnd) == -1) {
		MessageBox(hwnd, L"ImGUIの初期化に失敗しました。", L"エラー", MB_OK);
		return -1;
	}
	initializeShader();

	MSG msg = {};
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			Input::update();
			float color[4] = { 1, 1, 1, 1.0f };
			DirectX3D::d3d11Context_->OMSetRenderTargets(1, &renderTargetView_, nullptr);
			DirectX3D::d3d11Context_->ClearRenderTargetView(renderTargetView_, color);

			UINT stride = sizeof(Vertex);
			UINT offset = 0;

			DirectX3D::d3d11Context_->IASetInputLayout(inputLayout);
			DirectX3D::d3d11Context_->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
			DirectX3D::d3d11Context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			DirectX3D::d3d11Context_->VSSetShader(vertexShader, nullptr, 0);
			DirectX3D::d3d11Context_->PSSetShader(pixelShader, nullptr, 0);

			DirectX3D::d3d11Context_->Draw(3, 0);

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Game");
			ImGui::Text("mouse-left: %d, mouse-center, %d, mouse-right: %d", Input::IsPushMouse(0), Input::IsPushMouse(2), Input::IsPushMouse(1));
			ImGui::End();

			ImGui::EndFrame();
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
			}


			DirectX3D::swapChain_->Present(1, 0);

			if (Input::IsPushKey(DIK_0)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				break;
			}
		}
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	DirectX3D::release();
	Input::release();
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	if (initializeWindow(hInstance, hPrevInstance, lpCmdLine, nShowCmd) == -1) {
		MessageBox(NULL, L"ウインドウの初期化に失敗しました", L"エラー", MB_OK);
		return -1;
	}
	return 0;
}