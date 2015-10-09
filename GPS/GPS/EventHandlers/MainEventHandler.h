#pragma once


#include <Windows.h>
#include <exception>
#include <CommCtrl.h>
#include <tchar.h>

#include "../Logic/H1z1.h"
#include "../Infrastructure/Messages/WindowMessages.h"
#include "../resource.h"

using namespace std;

// Manifest to use the latest CommCtl32 library which
// enables the newest visual styles for windows.
// ----------------------------------------------------
// ref: https://msdn.microsoft.com/en-us/library/windows/desktop/bb773175%28v=vs.85%29.aspx
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

class MainEventHandler {
private:
	PlayerCoords        coords;
	HINSTANCE           hInstance;
	HWND                hwnd;
	H1z1				h1z1;
	int                 charWidth;
	int                 charHeight;
	int                 timerId;

public:
	MainEventHandler();
	~MainEventHandler();

	// WM_CREATE: Called when the window is being created.
	LRESULT OnCreate(CreateWindowMessage*);
	// WM_PAINT: Called when the window needs to be redrawn.
	LRESULT OnPaint(PaintWindowMessage*);
	// WM_SIZE: Called when the window was resized;
	LRESULT OnResize(ResizeWindowMessage*);
	// WM_COMMAND: Called when a button, menu button or accelerator was triggered
	LRESULT OnCommand(CommandWindowMessage*);
	// WM_LBUTTONDOWN: Called when the left mouse button is clicked
	LRESULT OnLeftMouseButtonClick(LeftMouseButtonDownMessage*);
	// WM_TIMER: Called when a timer sends a WM to the hwnd
	LRESULT OnTimerCallback(TimerMessage*);

private:
	BOOL     initCommonVisualControls();
	void     calculateDefaultFontSizes(HDC);
	void     drawMap(HDC);
	void     drawPlayerPosition(HDC);
	void     attachToH1Z1Process();
};