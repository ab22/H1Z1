#pragma once

#include "WinMessageBase.h"


class TimerMessage : public WinMessageBase {
public:
	int timerId;

	TimerMessage(HWND, UINT, WPARAM, LPARAM);
};