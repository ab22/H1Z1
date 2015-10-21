#pragma once

#include "WinMessageBase.h"


class CreateWindowMessage : public WinMessageBase {
public:
	CREATESTRUCT* createStruct;

	CreateWindowMessage(HWND, UINT, WPARAM, LPARAM);
};