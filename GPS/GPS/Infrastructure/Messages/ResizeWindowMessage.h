#pragma once

#include "WinMessageBase.h"


class ResizeWindowMessage : public WinMessageBase {
public:
	DWORD   resizeTypeRequested;
	WORD    newWidth;
	WORD    newHeight;

	ResizeWindowMessage(HWND, UINT, WPARAM, LPARAM);
};