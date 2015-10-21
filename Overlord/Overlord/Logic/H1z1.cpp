#include "H1z1.h"


H1z1::H1z1() {
	this->status = H1z1Status::NOT_ATTACHED;
	this->h1z1Module = NULL;
	this->processHandle = NULL;
	this->baseAddress = 0;
}

H1z1::~H1z1() {
	this->resetValues();
}

void H1z1::AttachToProcess() {
	try {
		this->processHandle = this->getH1z1ProcessHandle();
	} catch (std::exception& e) {
		resetValues();
		throw e;
	}

	this->h1z1Module = this->findH1Z1Module(processHandle);

	if (this->h1z1Module == NULL) {
		resetValues();
		throw exception("H1Z1 Module not found!");
	}

	this->status = H1z1Status::ATTACHED;
	this->baseAddress = (DWORD64)this->h1z1Module;
}

void H1z1::GetCoordinates(PlayerCoords* coords) {
	if (this->status != H1z1Status::ATTACHED) {
		throw exception("Process is not attached!");
	}

	DWORD64 address = this->baseAddress + this->positionsOffset;

	BOOL result = ReadProcessMemory(
		this->processHandle,
		(LPCVOID)(address),
		(LPVOID)coords,
		sizeof(PlayerCoords),
		NULL
	);

	if (result == 0) {
		char message[128];
		DWORD errorCode = GetLastError();

		sprintf_s(message, sizeof(message), "Error(%d): could not read process memory", errorCode);
		this->resetValues();		

		throw exception(message);
	}
}

void H1z1::GetHeadingTo(float* headingTo) {
	if (this->status != H1z1Status::ATTACHED) {
		throw exception("Process is not attached!");
	}

	DWORD64 address = this->headingOffset;

	BOOL result = ReadProcessMemory(
		this->processHandle,
		(LPCVOID)(address),
		(LPVOID)headingTo,
		sizeof(float),
		NULL
	);

	if (result == 0) {
		char message[128];
		DWORD errorCode = GetLastError();

		sprintf_s(message, sizeof(message), "Error(%d): could not read process memory", errorCode);
		this->resetValues();

		throw exception(message);
	}
}

bool H1z1::IsAttached() {
	return this->status == H1z1Status::ATTACHED;
}

void H1z1::resetValues() {
	if (this->processHandle != NULL) {
		CloseHandle(processHandle);
	}

	this->processHandle = NULL;
	this->status = H1z1Status::NOT_ATTACHED;
	this->h1z1Module = NULL;
	this->baseAddress = 0;
}


BOOL CALLBACK H1z1::enumWindowsProc(HWND hwnd, LPARAM lParam) {
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


HWND H1z1::findH1z1WindowHandle() {
	HWND hwnd = NULL;

	EnumWindows(H1z1::enumWindowsProc, (LPARAM)&hwnd);

	return hwnd;
}

HANDLE H1z1::getH1z1ProcessHandle() {
	HANDLE  processHandle;
	HWND    windowHandle;
	DWORD   processId;

	windowHandle = this->findH1z1WindowHandle();

	if (windowHandle == NULL) {
		throw exception("Could not find window handle!");
	}

	GetWindowThreadProcessId(windowHandle, &processId);
	processHandle = OpenProcess(
		PROCESS_VM_READ | PROCESS_QUERY_INFORMATION,
		false,
		processId
	);

	if (processHandle == NULL) {
		throw exception("Failed to open process!");
	}

	return processHandle;
}

HMODULE H1z1::findH1Z1Module(HANDLE processHandle) {
	TCHAR   h1z1ExecutableName[] = TEXT("H1Z1.exe");
	HMODULE modules[1024];
	DWORD   cbNeeded;

	if (!EnumProcessModulesEx(processHandle, modules, sizeof(modules), &cbNeeded, LIST_MODULES_ALL)) {
		char message[256];
		DWORD errorCode = GetLastError();

		sprintf_s(message, sizeof(message), "Error(%d): Could not enumerate processes - cbNeeded (%d)", errorCode, cbNeeded);
		throw exception(message);
	}

	for (DWORD i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
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