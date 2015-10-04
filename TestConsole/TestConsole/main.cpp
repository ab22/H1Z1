#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <exception>

using namespace std;

struct H1Z1Coords {
	float z;
	float x;
};

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	TCHAR h1z1Title[] = TEXT("H1Z1 v0.");
	TCHAR title[MAX_PATH];

	GetWindowText(hwnd, title, sizeof(title));

	if (_tcsstr(title, h1z1Title) == NULL) {
		return TRUE;
	}

	HWND* returnValue = (HWND*)lParam;
	*returnValue = hwnd;
	return FALSE;
}

HWND FindH1Z1WindowHandle() {
	HWND hwnd = NULL;

	EnumWindows(EnumWindowsProc, (LPARAM)&hwnd);

	return hwnd;
}

HANDLE GetH1Z1ProcessHandle() {
	HANDLE  processHandle;
	HWND    windowHandle;
	DWORD   processId;

	windowHandle = FindH1Z1WindowHandle();

	if (windowHandle == NULL) {
		throw exception("could not find window handle!");
	}

	GetWindowThreadProcessId(windowHandle, &processId);
	processHandle = OpenProcess(
		PROCESS_VM_READ | PROCESS_QUERY_INFORMATION,
		false,
		processId
		);

	if (processHandle == NULL) {
		throw exception("failed to open process!");
	}

	return processHandle;
}

HMODULE FindH1Z1Module(HANDLE processHandle) {
	TCHAR   h1z1ExecutableName[] = TEXT("H1Z1.exe");
	HMODULE modules[1024];
	DWORD   cbNeeded;

	if (!EnumProcessModulesEx(processHandle, modules, sizeof(modules), &cbNeeded, LIST_MODULES_ALL)) {
		char message[256];
		DWORD errorCode = GetLastError();

		sprintf(message, "could not enumerate processes: error code(%d) - cbNeeded (%d)", errorCode, cbNeeded);
		throw exception(message);
	}

	for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
		HMODULE module = modules[i];
		TCHAR   moduleName[MAX_PATH];

		if (!GetModuleFileNameEx(processHandle, module, moduleName, sizeof(moduleName) / sizeof(TCHAR))) {
			continue;
		}

		if (_tcsstr(moduleName, h1z1ExecutableName) == NULL) {
			continue;
		}

		return module;
	}

	return NULL;
}

int main(void) {
	H1Z1Coords coords;
	MODULEINFO moduleInfo;
	HANDLE     processHandle = NULL;
	HMODULE    h1z1Module = NULL;
	UINT       zPosOffset = 0x3A8B27C;

	try {
		processHandle = GetH1Z1ProcessHandle();
	}
	catch (exception& e) {
		printf("%s\n", e.what());
		system("pause");
		return 0;
	}

	h1z1Module = FindH1Z1Module(processHandle);

	if (h1z1Module == NULL) {
		_tprintf(TEXT("H1Z1 Module not found!\n"));
		system("pause");
		return 0;
	}

	if (GetModuleInformation(processHandle, h1z1Module, &moduleInfo, sizeof(MODULEINFO)) == 0) {
		TCHAR  message[] = TEXT("error retrieving module information: error code(%d)");
		DWORD errorCode = GetLastError();

		_tprintf(message, errorCode);
	}

	_tprintf(TEXT("--------------------------------------------\n"));
	_tprintf(TEXT("> Process information\n"));
	_tprintf(TEXT("--------------------------------------------\n"));
	_tprintf(TEXT(">   H1z1 Module: %016llX\n"), (DWORD64)h1z1Module);
	_tprintf(TEXT(">  Z Pos Offset: 0x%X\n"), (DWORD)h1z1Module + zPosOffset);
	_tprintf(TEXT(">   Base of DLL: 0x%X\n"), moduleInfo.lpBaseOfDll);
	_tprintf(TEXT("> Size of image: 0x%X\n"), moduleInfo.SizeOfImage);
	_tprintf(TEXT(">   Entry point: 0x%X\n"), moduleInfo.EntryPoint);
	_tprintf(TEXT(">       LPCVOID: %d\n"), sizeof(LPCVOID));
	_tprintf(TEXT(">           int: %d\n"), sizeof(int));
	_tprintf(TEXT(">       HMODULE: %d\n"), sizeof(HMODULE));
	_tprintf(TEXT("--------------------------------------------\n"));
	_tprintf(TEXT("> Priting in game coordinates\n"));
	_tprintf(TEXT("--------------------------------------------\n"));

	while (true) {
		if (ReadProcessMemory(processHandle, (LPCVOID)((DWORD64)h1z1Module + zPosOffset), (LPVOID)&coords, sizeof(coords), NULL) == 0){
			TCHAR  message[] = TEXT("error retrieving module information: error code(%d)");
			DWORD errorCode = GetLastError();

			_tprintf(message, errorCode);
			break;
		}

		_tprintf(TEXT("(x:%f, z:%f) \n"), coords.x, coords.z);
		Sleep(1000);
	}

	CloseHandle(processHandle);

	system("pause");
	return 0;
}
