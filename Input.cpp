#include <dinput.h>
#include "Input.h"
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

int Input::initialize(HINSTANCE hInstance, HWND hwnd) {
    HRESULT result = {};
    result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&input_, nullptr);

    result = input_->CreateDevice(GUID_SysKeyboard, &inputDevice_, nullptr);
    result = inputDevice_->SetDataFormat(&c_dfDIKeyboard);
    result = inputDevice_->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

    return 0;
}

void Input::update() {
    inputDevice_->Acquire();
    inputDevice_->GetDeviceState(sizeof(keys_), &keys_);
}

void Input::release() {
    input_->Release();
    inputDevice_->Release();
}

bool Input::IsPushKey(int key) {
    if (keys_[key] & 0x80) {
        return true;
    }
    return false;
}
