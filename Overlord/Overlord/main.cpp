#pragma once

#include <Windows.h>

// Make these definitions global for the other classes
// and headers in case we need them.
#define APP_CLASS_NAME  TEXT("H1Z1_OVERLORD")
#define APP_WND_TITLE   TEXT("H1Z1 OVERLORD")

#include "EventHandlers\MainEventHandler.h"
#include "Infrastructure\Messages\WindowMessages.h"
#include "Logic\H1z1.h"
#include "Logic\D3DApp.h"

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *lpCmdLine, int nShowCmd);

MainEventHandler mainEventHandler;

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	WinMessageBase*  message = NULL;
	LRESULT          result = 0;

	switch (msg) {
	case WM_CREATE:
		message = new CreateWindowMessage(hwnd, msg, wParam, lParam);
		result = mainEventHandler.OnCreate((CreateWindowMessage *)message);
		break;
	case WM_PAINT:
		message = new PaintWindowMessage(hwnd, msg, wParam, lParam);
		result = mainEventHandler.OnPaint((PaintWindowMessage *)message);
		break;
	case WM_SIZE:
		message = new ResizeWindowMessage(hwnd, msg, wParam, lParam);
		result = mainEventHandler.OnResize((ResizeWindowMessage *)message);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return result;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	if (message != NULL)
		delete message;

	return result;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *lpCmdLine, int nShowCmd) {
	WNDCLASSEX wc;
	D3DApp     d3dApp;
	H1z1       h1z1;

	// Try to attach to the H1Z1 process. If it fails, then close our app.
	try {
		h1z1.AttachToProcess();
		mainEventHandler.SetH1z1Client(&h1z1);
	}
	catch (exception &e) {
		char errorMessage[128];

		sprintf(errorMessage, "Error attaching to the H1Z1 process: %s", e.what());

		MessageBoxA(NULL, errorMessage, "Error!", MB_OK);
		return 0;
	}

	wc.cbClsExtra = 0;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = 0;
	wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(0, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WinProc;
	wc.lpszClassName = APP_CLASS_NAME;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	RegisterClassEx(&wc);
	
	HWND hwnd = CreateWindowEx(
		WS_EX_LAYERED  | WS_EX_TOPMOST | WS_EX_TRANSPARENT,
		APP_CLASS_NAME,
		APP_WND_TITLE,
		WS_POPUP,
		1,
		1,
		800,
		600,
		0,
		0,
		0,
		0
	);	

	SetLayeredWindowAttributes(hwnd, 0, 0, LWA_ALPHA);
	SetLayeredWindowAttributes(hwnd, 0, 0, LWA_COLORKEY);	

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	MSG msg;
	d3dApp.Initialize(hwnd, &h1z1);
	
	while (TRUE){
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) {
				break;
			}
		}
		else {
			try {
				d3dApp.Render();
			} catch (exception &e) {
				char errorMessage[128];

				sprintf(errorMessage, "Error rendering: %s", e.what());

				MessageBoxA(NULL, errorMessage, "Error!", MB_OK);
				break;
			}			
			Sleep(100);
		}
	}

	d3dApp.Clean();

	return (int)msg.wParam;
}