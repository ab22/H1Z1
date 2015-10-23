#pragma once

#include <Windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <d3d9.h>
#include <dxgi.h>
#include <DxErr.h>
#include <stdio.h>
#include "H1z1.h"

#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxerr.lib")

// Debugging Macros
#define ReleaseCom(x) if(x){ x->Release(); x = NULL; }

#if defined(_DEBUG) | defined(DEBUG)
	#ifndef TraceInvalidResult
		#define TraceInvalidResult(x) { HRESULT hr = (x); if( FAILED(hr) ){ DXTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true); } }
	#endif
#else
	#ifndef TraceInvalidResult
		#define TraceInvalidResult(x) (x)
	#endif
#endif


class D3DApp {
private:
	HWND                    hwnd;
	ID3D10Device*           device;
	IDXGISwapChain*         swapChain;
	ID3D10RenderTargetView* backBuffer;
	ID3DX10Font*            smallFont;
	H1z1*                   h1z1;
	PlayerCoords            playerCoords;
	float                   headingTo;

public:
	D3DApp();
	~D3DApp();

	void Initialize(HWND, H1z1*);
	void Render();
	void Clean();

private:
	bool attachToProcess();
	void drawCoords();
	int  getCompassString(char *, float);
};