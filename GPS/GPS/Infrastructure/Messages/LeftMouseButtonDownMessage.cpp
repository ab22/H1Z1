#include "LeftMouseButtonDownMessage.h"


LeftMouseButtonDownMessage::LeftMouseButtonDownMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) : WinMessageBase(hwnd, msg, wParam, lParam) {
	this->xCoordinate = LOWORD(lParam);
	this->yCoordinate = HIWORD(lParam);
}