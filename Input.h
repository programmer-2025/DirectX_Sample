#pragma once
#include <dinput.h>

namespace Input {
	inline LPDIRECTINPUT8 input_;
	inline LPDIRECTINPUTDEVICE8 inputDevice_;
	inline BYTE keys_[256] = {};

	inline LPDIRECTINPUTDEVICE8 mouseInputDevice_;
	inline DIMOUSESTATE mouseState_;
	inline POINT mousePoint_;

	int initialize(HINSTANCE hInstance, HWND hwnd);
	void update();
	void release();
	bool IsPushKey(int key);
	bool IsPushMouse(int button);
	
}