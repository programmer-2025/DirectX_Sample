#include <Windows.h>
#include "DirectX3D.h"
#include "Input.h"
#define WINDOW_CLASS_NAME L"DirectX_Sample"

namespace {

}

using namespace DirectX3D;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
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

	MSG msg = {};
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			Input::update();
			float color[4] = { 0, 0, 0, 1.0f };

			d3d11Context_->OMSetRenderTargets(1, &renderTargetView_, nullptr);
			d3d11Context_->ClearRenderTargetView(renderTargetView_, color);
			swapChain_->Present(1, 0);

			if (Input::IsPushKey(DIK_0)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				break;
			}
		}
	}

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