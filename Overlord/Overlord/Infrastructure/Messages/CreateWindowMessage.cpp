#include "CreateWindowMessage.h"


CreateWindowMessage::CreateWindowMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) : WinMessageBase(hwnd, msg, wParam, lParam) {
	this->createStruct = (CREATESTRUCT *)lParam;
}