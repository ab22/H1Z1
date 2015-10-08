#include "WinMessageBase.h"


WinMessageBase::WinMessageBase(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	this->hwnd = hwnd;
	this->msg = msg;
	this->wParam = wParam;
	this->lParam = lParam;
}