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
#include "Input/WinInput.h"
#include "RenderComponents/Planet.h"
#include "Camera/Camera.h"
#include "Camera/CameraController.h"


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")




class Game
{
public:


	Game(LPCWSTR applicationName, HINSTANCE hInstance, int width, int height);

	~Game();

	HWND GetWindow32HWND();

	void CreateDeviceAndSwapChain();

	void InitializeDirectX();

	void StartGameLoop();

	void AddGameComponent(std::shared_ptr<GameComponent> gc);

	std::unique_ptr<DisplayWin32> window;
	std::unique_ptr<WinInput> wInput;


	Camera* camera;
	CameraController* cameraController;
	
	DXGI_SWAP_CHAIN_DESC swapDesc;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr <IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;
	Microsoft::WRL::ComPtr < ID3D11Texture2D> backTex;
	Microsoft::WRL::ComPtr < ID3D11RasterizerState> rastState;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr <ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilState> depthStencilState;
	
	
	CB_VS_vertexshader worldViewData;
	
	bool isExitRequested = false;

	
	std::vector<std::shared_ptr<GameComponent>> gameComponents;
	

	


private:
	void GetInput();
	void Render();
	void Update();
	

	std::chrono::time_point<std::chrono::steady_clock> PrevTime;
	float totalTime = 0;
	unsigned int frameCount = 0;
};

