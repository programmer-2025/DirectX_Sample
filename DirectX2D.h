#pragma once
#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>

namespace DirectX2D {
	inline ID2D1Factory* factory = nullptr;
	inline IDWriteFactory* writeFactory = nullptr;
	inline IDWriteTextFormat* textFormat = nullptr;
	inline ID2D1RenderTarget* renderTarget = nullptr;
	inline ID2D1SolidColorBrush* brush = nullptr;

	void Init();
}