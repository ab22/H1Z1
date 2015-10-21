#include "D3DApp.h"

D3DApp::D3DApp() {
	this->device = NULL;
	this->swapChain = NULL;
	this->backBuffer = NULL;
	this->smallFont = NULL;
	this->h1z1 = NULL;
	this->hwnd = NULL;
	this->headingTo = 0.0f;
	
	ZeroMemory(&this->playerCoords, sizeof(PlayerCoords));
}

D3DApp::~D3DApp() {
	this->Clean();
}

void D3DApp::Initialize(HWND hwnd, H1z1* h1z1) {
	DXGI_SWAP_CHAIN_DESC scd;
	HRESULT              result = NULL;
	UINT                 devFlag = 0;

	this->hwnd = hwnd;
	this->h1z1 = h1z1;

#ifdef _DEBUG
	devFlag = D3D10_CREATE_DEVICE_DEBUG;
#endif

	ZeroMemory(&scd, sizeof(scd));
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = this->hwnd;
	scd.SampleDesc.Count = 4;
	scd.Windowed = TRUE;

	result = D3D10CreateDeviceAndSwapChain(
		NULL,
		D3D10_DRIVER_TYPE_HARDWARE,
		NULL,
		devFlag,
		D3D10_SDK_VERSION,
		&scd,
		&this->swapChain,
		&this->device
	);

	ID3D10Texture2D *pBackBuffer;
	result = this->swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);
	result = this->device->CreateRenderTargetView(pBackBuffer, NULL, &this->backBuffer);

	ReleaseCom(pBackBuffer);

	this->device->OMSetRenderTargets(1, &this->backBuffer, NULL);

	D3D10_VIEWPORT viewPort;

	ZeroMemory(&viewPort, sizeof(viewPort));
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = 800;
	viewPort.Height = 600;

	this->device->RSSetViewports(1, &viewPort);
	
	D3DX10CreateFont(
		this->device,
		18,
		0,
		0,
		2,
		false,
		DEFAULT_CHARSET,
		OUT_CHARACTER_PRECIS,
		ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		TEXT("Ubuntu"),
		&this->smallFont
	);
}

void D3DApp::Render() {
	this->device->ClearRenderTargetView(this->backBuffer, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

	this->h1z1->GetCoordinates(&this->playerCoords);
	this->h1z1->GetHeadingTo(&this->headingTo);
	this->drawCoords();

	this->swapChain->Present(0, 0);
}

void D3DApp::drawCoords() {
	RECT textPos;
	char playerCoords[64];
	int  textLength = 0;	

	ZeroMemory(&textPos, sizeof(RECT));
	textPos.left = 15;
	textPos.top = 15;

	textLength = sprintf(playerCoords, "LOCATION - (Z: %.2f, X: %.2f) - Heading: %.2f", this->playerCoords.z, this->playerCoords.x, this->headingTo);

	this->smallFont->DrawTextA(NULL, playerCoords, textLength, &textPos, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
}

void D3DApp::Clean() {
	ReleaseCom(this->device);
	//ReleaseCom(this->deviceContext);
	ReleaseCom(this->swapChain);
	ReleaseCom(this->backBuffer);
}