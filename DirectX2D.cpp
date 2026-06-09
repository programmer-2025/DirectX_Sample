#include "DirectX2D.h"
#include "DirectX3D.h"

#pragma comment(lib, "d2d1.lib") 
#pragma comment(lib, "dwrite.lib")

namespace DirectX2D {

	void Init() {
		const WCHAR* fontName = L"メイリオ";
		const int fontSize = 30;

		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(writeFactory), reinterpret_cast<IUnknown**>(&writeFactory));
		hr = writeFactory->CreateTextFormat(
			fontName,
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			fontSize,
			L"", 
			&textFormat
		);

		hr = textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		hr = textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		IDXGISurface* surface = nullptr;
		hr = DirectX3D::swapChain_->GetBuffer(0, IID_PPV_ARGS(&surface));
		D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
		);

		hr = factory->CreateDxgiSurfaceRenderTarget(surface, &props, &renderTarget); //TODO: E_INVALIDARGになるので修正
		renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);
		surface->Release();
	}
}