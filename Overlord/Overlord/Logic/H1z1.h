#pragma once

#include <tchar.h>
#include <Windows.h>
#include <Psapi.h>
#include <stdio.h>
#include <exception>
#include <fstream>
#include <sstream>
#include "../vendor/rapidjson/document.h"
#include "../vendor/rapidjson/writer.h"
#include "../vendor/rapidjson/stringbuffer.h"

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
	DWORD64 positionsOffset = 0;
	DWORD64 headingOffset = 0;

	H1z1Status     status;
	HANDLE         processHandle;
	HMODULE        h1z1Module;
	DWORD64        baseAddress;

	void    resetValues();
	HANDLE  getH1z1ProcessHandle();
	HWND    findH1z1WindowHandle();
	HMODULE findH1Z1Module(HANDLE);
	char*   readConfigFile();
	void    readConfigVariables();

	static BOOL CALLBACK enumWindowsProc(HWND, LPARAM);
};