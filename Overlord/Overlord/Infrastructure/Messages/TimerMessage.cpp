#include "TimerMessage.h"

TimerMessage::TimerMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) : WinMessageBase(hwnd, msg, wParam, lParam) {
	this->timerId = (int)wParam;
}