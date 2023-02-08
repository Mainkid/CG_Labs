#pragma once
#include <windows.h>
#include <WinUser.h>
#include <wrl.h>
#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>

#include "DisplayWin32.h"
#include "WinInput.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")



class Game
{
public:

	Game();

	~Game();
	
	HWND CreateWindow32(LPCWSTR applicationName, HINSTANCE hInstance, int width, int height);

	HWND GetWindow32HWND();

	//Сделать добавление фичер левела
	void SetFeatureLevel();

	//Не все параметры вынесены
	void SetSwapDesc(int buffCount = 2, int RefreshRateNum = 60, int RefreshRateDenominator = 1, bool isWindowed = true);

	void CreateDeviceAndSwapChain();

	void CompileFromFile(LPCWSTR fileName);

	void Render();

	void Update();



	DisplayWin32* window;
	WinInput* wInput;

	DXGI_SWAP_CHAIN_DESC swapDesc;

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	ID3D11DeviceContext* context;
	IDXGISwapChain* swapChain;

	
	HRESULT res;
	bool isExitRequested = false;
	//Переделать
	ID3DBlob* vertexBC; //m
	ID3DBlob* pixelBC;  //m
	ID3D11Texture2D* backTex;
	ID3D11RasterizerState* rastState;
	ID3D11InputLayout* layout;
	ID3D11Buffer* ib;
	ID3D11Buffer* vb;
	UINT strides[1] = { 32 }; //m
	UINT offsets[1] = { 0 }; //m
	
	ID3DBlob* errorPixelCode;
	ID3DBlob* errorVertexCode = nullptr;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	std::chrono::time_point<std::chrono::steady_clock> PrevTime;
	ID3D11RenderTargetView* rtv;
	float totalTime = 0;
	unsigned int frameCount = 0;
	CD3D11_RASTERIZER_DESC rastDesc = {};
};

