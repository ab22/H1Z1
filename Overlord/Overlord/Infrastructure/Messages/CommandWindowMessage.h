#pragma once

#include "WinMessageBase.h"


class CommandWindowMessage : public WinMessageBase {
public:
	bool    isFromMenu;
	bool    isFromAccelerator;
	DWORD   buttonIdentifier;
	HANDLE  hSender;

	CommandWindowMessage(HWND, UINT, WPARAM, LPARAM);
};