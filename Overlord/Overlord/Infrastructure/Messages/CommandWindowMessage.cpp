#include "CommandWindowMessage.h"


CommandWindowMessage::CommandWindowMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) : WinMessageBase(hwnd, msg, wParam, lParam) {
	this->isFromMenu = HIWORD(this->wParam) == 0;
	this->isFromAccelerator = !this->isFromMenu;
	this->buttonIdentifier = LOWORD(wParam);
	this->hSender = (HANDLE)lParam;
}