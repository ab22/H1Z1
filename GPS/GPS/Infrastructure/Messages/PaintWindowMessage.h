#pragma once

#include "WinMessageBase.h"


class PaintWindowMessage : public WinMessageBase {
public:
	PaintWindowMessage(HWND, UINT, WPARAM, LPARAM);
};