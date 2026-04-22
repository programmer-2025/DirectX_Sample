#pragma once
#include <Windows.h>
#include <string>

namespace Logger {

	inline void debug(std::wstring message) {
		message += L"\n";
		OutputDebugString(message.c_str());
	}
}