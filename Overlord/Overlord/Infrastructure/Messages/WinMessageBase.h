#pragma once
#include <Windows.h>

class WinMessageBase {
public:
	HWND    hwnd;
	UINT    msg;
	WPARAM  wParam;
	LPARAM  lParam;

	WinMessageBase(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};