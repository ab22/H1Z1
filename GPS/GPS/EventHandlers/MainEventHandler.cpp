#include "MainEventHandler.h"


MainEventHandler::MainEventHandler() {
	this->hwnd = NULL;
	this->hInstance = NULL;
	this->charHeight = 0;
	this->charWidth = 0;
	this->timerId = 1;

	this->coords.z = 0;
	this->coords.x = 0;
}

MainEventHandler::~MainEventHandler() {

}

LRESULT MainEventHandler::OnCreate(CreateWindowMessage *msg) {
	HDC hdc;

	hdc = GetDC(msg->hwnd);
	this->hwnd = msg->hwnd;
	this->hInstance = msg->createStruct->hInstance;
	this->calculateDefaultFontSizes(hdc);
	this->initCommonVisualControls();
	ReleaseDC(msg->hwnd, hdc);

	return DefWindowProc(msg->hwnd, msg->msg, msg->wParam, msg->lParam);
}

LRESULT MainEventHandler::OnPaint(PaintWindowMessage *msg) {
	PAINTSTRUCT ps;
	HDC         hdc;

	hdc = BeginPaint(msg->hwnd, &ps);

	this->drawMap(hdc);
	this->drawPlayerPosition(hdc);

	EndPaint(msg->hwnd, &ps);

	return 0;
}

LRESULT MainEventHandler::OnResize(ResizeWindowMessage *msg) {
	return 0;
}

LRESULT MainEventHandler::OnCommand(CommandWindowMessage *msg) {
	if (msg->buttonIdentifier == ID_H1Z1_ATTACHTOPROCESS) {
		this->attachToH1Z1Process();
	}

	return TRUE;
}

LRESULT MainEventHandler::OnLeftMouseButtonClick(LeftMouseButtonDownMessage* msg) {
	return 0;
}

LRESULT MainEventHandler::OnTimerCallback(TimerMessage* msg) {
	if (msg->timerId == this->timerId) {
		try {
			this->h1z1.GetCoordinates(&this->coords);
			InvalidateRect(this->hwnd, NULL, TRUE);
		} catch (exception& e){
			KillTimer(this->hwnd, this->timerId);
			MessageBoxA(this->hwnd, e.what(), "Error", MB_OK);
		}		
	}

	return 0;
}

void MainEventHandler::calculateDefaultFontSizes(HDC hdc) {
	TEXTMETRIC tm;

	GetTextMetrics(hdc, &tm);
	this->charWidth = tm.tmAveCharWidth;
	this->charHeight = tm.tmHeight + tm.tmInternalLeading;
}

BOOL MainEventHandler::initCommonVisualControls() {
	INITCOMMONCONTROLSEX	commControls;

	commControls.dwSize = sizeof(INITCOMMONCONTROLSEX);
	commControls.dwICC = ICC_STANDARD_CLASSES | ICC_BAR_CLASSES;

	return InitCommonControlsEx(&commControls);
}

void MainEventHandler::drawMap(HDC hdc) {
	HBITMAP hBitmap = NULL;
	BITMAP  h1z1Bitmap;
	HDC     hdcMem;

	hBitmap = (HBITMAP)LoadImage(this->hInstance, MAKEINTRESOURCE(IDB_BITMAP1), 0, 0, 0, 0);

	hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, hBitmap);

	GetObject(hBitmap, sizeof(h1z1Bitmap), &h1z1Bitmap);
	BitBlt(hdc, 0, 0, h1z1Bitmap.bmWidth, h1z1Bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

	DeleteDC(hdcMem);
}

void MainEventHandler::drawPlayerPosition(HDC hdc) {
	if (!this->h1z1.IsAttached()) {
		return;
	}

	TCHAR  title[128];
	HBRUSH brush = CreateSolidBrush(RGB(255, 255, 0));
	SelectObject(hdc, brush);

	int imageSize = 720;
	float mappedPosZ = this->coords.z <= -4000 ? 0 : this->coords.z + 4000;
	float mappedPosX = this->coords.x <= -4000 ? 0 : this->coords.x + 4000;

	int z = (int)((mappedPosZ / 8000) * imageSize);
	int x = (int)(720 - (((mappedPosX / 8000) * imageSize)));
	
	Ellipse(
		hdc,
		z,
		x,
		z + 10,
		x + 10
	);

	_stprintf_s(title, _countof(title), TEXT("H1Z1 GPS - (Z: %.2f, X: %.2f)"), this->coords.z, this->coords.x);
	SetWindowText(this->hwnd, title);
}

void MainEventHandler::attachToH1Z1Process() {
	try {
		h1z1.AttachToProcess();
	}
	catch (exception &e) {
		MessageBoxA(this->hwnd, e.what(), "Error", MB_OK);
		return;
	}

	InvalidateRect(this->hwnd, NULL, TRUE);
	SetTimer(this->hwnd, this->timerId, 1000, NULL);
}