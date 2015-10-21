#pragma once

#include <tchar.h>
#include <Windows.h>
#include <Psapi.h>
#include <stdio.h>
#include <exception>

using namespace std;

enum H1z1Status {
	NOT_ATTACHED = 0,
	ATTACHED = 1
};

struct PlayerCoords {
	float z;
	float x;
};


class H1z1 {
public:
	H1z1();
	~H1z1();

	void AttachToProcess();
	void GetCoordinates(PlayerCoords*);
	void GetHeadingTo(float*);
	bool IsAttached();
private:
	const DWORD64 positionsOffset = 0x3AAA35C;
	const DWORD64 headingOffset = 0x0C43C220;

	H1z1Status     status;
	HANDLE         processHandle;
	HMODULE        h1z1Module;
	DWORD64        baseAddress;

	void    resetValues();
	HANDLE  getH1z1ProcessHandle();
	HWND    findH1z1WindowHandle();
	HMODULE findH1Z1Module(HANDLE);

	static BOOL CALLBACK enumWindowsProc(HWND, LPARAM);
};