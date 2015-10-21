#pragma once

#include "WinMessageBase.h"

class InitDialogMessage : public WinMessageBase {
public:
	InitDialogMessage(HWND, UINT, WPARAM, LPARAM);
};