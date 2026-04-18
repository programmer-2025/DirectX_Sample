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

    result = input_->CreateDevice(GUID_SysMouse, &mouseInputDevice_, nullptr);
    result = mouseInputDevice_->SetDataFormat(&c_dfDIMouse);
    result = mouseInputDevice_->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

    return 0;
}

void Input::update() {
    inputDevice_->Acquire();
    inputDevice_->GetDeviceState(sizeof(keys_), &keys_);

    mouseInputDevice_->Acquire();
    mouseInputDevice_->GetDeviceState(sizeof(mouseState_), &mouseState_);
    mousePoint_.x += mouseState_.lX;
    mousePoint_.y += mouseState_.lY;
}

void Input::release() {
    input_->Release();
    inputDevice_->Release();
    mouseInputDevice_->Release();
}

bool Input::IsPushKey(int key) {
    if (keys_[key] & 0x80) {
        return true;
    }
    return false;
}

bool Input::IsPushMouse(int button) {
    if (mouseState_.rgbButtons[button] & 0x80) {
        return true;
    }
    return false;
}
