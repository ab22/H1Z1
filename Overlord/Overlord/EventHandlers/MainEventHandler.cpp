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

void MainEventHandler::SetH1z1Client(H1z1* h1z1) {
	this->h1z1 = h1z1;
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
	MARGINS     margin = {0, 0, 800, 600};

	DwmExtendFrameIntoClientArea(hwnd, &margin);

	return DefWindowProc(msg->hwnd, msg->msg, msg->wParam, msg->lParam);
}

LRESULT MainEventHandler::OnResize(ResizeWindowMessage *msg) {
	return 0;
}

LRESULT MainEventHandler::OnTimerCallback(TimerMessage* msg) {
	if (msg->timerId == this->timerId) {
		try {
			this->h1z1->GetCoordinates(&this->coords);
			InvalidateRect(this->hwnd, NULL, FALSE);
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

void MainEventHandler::attachToH1Z1Process() {
	try {
		h1z1->AttachToProcess();
	}
	catch (exception &e) {
		MessageBoxA(this->hwnd, e.what(), "Error", MB_OK);
		return;
	}

	InvalidateRect(this->hwnd, NULL, FALSE);
	SetTimer(this->hwnd, this->timerId, 1000, NULL);
}
