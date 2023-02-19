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
#include <vector>

#include "DisplayWin32.h"
#include "WinInput.h"
#include "TriangleComponent.h"


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")



class Game
{
public:

	Game(LPCWSTR applicationName, HINSTANCE hInstance, int width, int height);

	~Game();
	
	/*HWND CreateWindow32(LPCWSTR applicationName, HINSTANCE hInstance, int width, int height);*/

	HWND GetWindow32HWND();

	//Сделать добавление фичер левела
	void SetFeatureLevel();

	//Не все параметры вынесены
	void SetSwapDesc(int buffCount = 2, int RefreshRateNum = 60, int RefreshRateDenominator = 1, bool isWindowed = true);

	void CreateDeviceAndSwapChain();

	void CompileFromFile(LPCWSTR fileName);

	void InitializeDirectX();

	void StartGameLoop();

	void AddGameComponent(GameComponent* gc);



	DisplayWin32* window;
	WinInput* wInput;

	DXGI_SWAP_CHAIN_DESC swapDesc;

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr <IDXGISwapChain> swapChain;
	ID3D11RenderTargetView* rtv;
	
	HRESULT res;
	bool isExitRequested = false;

	ID3D11Texture2D* backTex;
	ID3D11RasterizerState* rastState;
	

	std::chrono::time_point<std::chrono::steady_clock> PrevTime;
	float totalTime = 0;
	unsigned int frameCount = 0;


private:
	void GetInput();
	void Render();
	void Update();
	std::vector<GameComponent*> gameComponents;
};

