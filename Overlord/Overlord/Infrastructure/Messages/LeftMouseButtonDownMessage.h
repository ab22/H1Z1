#pragma once

#include "WinMessageBase.h"


class LeftMouseButtonDownMessage : public WinMessageBase {
public:
	int   xCoordinate;
	int   yCoordinate;

	LeftMouseButtonDownMessage(HWND, UINT, WPARAM, LPARAM);
};