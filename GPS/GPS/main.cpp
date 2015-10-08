#pragma once

#include <Windows.h>

// Make these definitions global for the other classes
// and headers in case we need them.
#define APP_CLASS_NAME  TEXT("H1Z1_GPS")
#define APP_WND_TITLE   TEXT("H1Z1 GPS")
#define APP_WIDTH       950
#define APP_HEIGHT      720

#include "EventHandlers\MainEventHandler.h"
#include "Infrastructure\Messages\WindowMessages.h"

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *lpCmdLine, int nShowCmd);

MainEventHandler *mainEventHandler = NULL;

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	WinMessageBase*  message = NULL;
	LRESULT          result = 0;

	switch (msg) {
	case WM_CREATE:
		message = new CreateWindowMessage(hwnd, msg, wParam, lParam);
		result = mainEventHandler->OnCreate((CreateWindowMessage *)message);
		break;
	case WM_COMMAND:
		message = new CommandWindowMessage(hwnd, msg, wParam, lParam);
		result = mainEventHandler->OnCommand((CommandWindowMessage *)message);
		break;
	case WM_LBUTTONDOWN:
		message = new LeftMouseButtonDownMessage(hwnd, msg, wParam, lParam);
		result = mainEventHandler->OnLeftMouseButtonClick((LeftMouseButtonDownMessage*)message);
		break;
	case WM_PAINT:
		message = new PaintWindowMessage(hwnd, msg, wParam, lParam);
		result = mainEventHandler->OnPaint((PaintWindowMessage *)message);
		break;
	case WM_SIZE:
		message = new ResizeWindowMessage(hwnd, msg, wParam, lParam);
		result = mainEventHandler->OnResize((ResizeWindowMessage *)message);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return result;
	case WM_CLOSE:
		result = MessageBox(hwnd, TEXT("Are you sure you want to exit?"), TEXT("Exit application"), MB_YESNOCANCEL | MB_ICONQUESTION);
		if (result == IDYES) {
			DestroyWindow(hwnd);
		}
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	if (message != NULL)
		delete message;

	return result;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *lpCmdLine, int nShowCmd) {
	mainEventHandler = new MainEventHandler();

	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = CreateSolidBrush(RGB(245, 245, 245));
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hIcon = LoadIcon(0, IDI_WINLOGO);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WinProc;
	wc.lpszClassName = APP_CLASS_NAME;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	RegisterClass(&wc);

	HWND hwnd = CreateWindow(
		APP_CLASS_NAME,
		APP_WND_TITLE,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		APP_WIDTH,
		APP_HEIGHT,
		0,
		0,
		hInstance,
		0
	);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	MSG msg;

	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	delete mainEventHandler;

	return msg.wParam;
}