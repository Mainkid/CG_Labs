#include "Game.h"


Game::Game()
{
	wInput = new WinInput(this);
}

Game::~Game()
{
	delete wInput;
	delete window;
	for (auto i:gameComponents)
	{
		delete i;
	}
}

HWND Game::CreateWindow32(LPCWSTR applicationName, HINSTANCE hInstance, int width, int height)
{
	window = new DisplayWin32(this,applicationName, hInstance, width, height);
	return window->GetHWND();
}

HWND Game::GetWindow32HWND()
{
	return window->GetHWND();
}

void Game::SetFeatureLevel()
{

}

void Game::SetSwapDesc(int buffCount, int RefreshRateNum, int RefreshRateDenominator, bool isWindowed)
{
	//Потом доделать
}

void Game::CreateDeviceAndSwapChain()
{
	swapDesc = {};
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = window->GetWidth();
	swapDesc.BufferDesc.Height = window->GetHeight();
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator =1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = window->GetHWND();
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	 

	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

	HRESULT res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapDesc,
		&swapChain,
		&device,
		nullptr,
		&context);

	if (FAILED(res))
	{
		std::cout << "Well, that was unexpected" << std::endl;
	}



	res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backTex);	// __uuidof(ID3D11Texture2D)
	res = device->CreateRenderTargetView(backTex, nullptr, &rtv);
}

void Game::CompileFromFile(LPCWSTR fileName)
{

}

void Game::InitializeDirectX()
{
	

	for (auto i : gameComponents)
		i->Initialize();
}

void Game::StartGameLoop()
{
	PrevTime = std::chrono::steady_clock::now();

	while (wInput->ProcessMessages()) {

		GetInput();
		Update();
		Render();
	}

	

}

void Game::AddGameComponent(GameComponent* gc)
{
	gameComponents.push_back(gc);
}

#pragma region GameLoopPattern

void Game::GetInput()
{
	wInput->GetInput();
}

void Game::Render()
{
	float bgColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	context->ClearRenderTargetView(rtv, bgColor);

	for (auto i : gameComponents)
	{
		i->Render();
	}

	swapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
}

void Game::Update()
{
	auto	curTime = std::chrono::steady_clock::now();
	float	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - PrevTime).count() / 1000000.0f;
	PrevTime = curTime;

	totalTime += deltaTime;
	frameCount++;

	if (totalTime > 1.0f) {
		float fps = frameCount / totalTime;

		totalTime -= 1.0f;

		WCHAR text[256];
		swprintf_s(text, TEXT("FPS: %f"), fps);
		SetWindowText(GetWindow32HWND(), text);

		frameCount = 0;
	}

	for (auto vec : gameComponents)
	{
		vec->Update(deltaTime);
	}
}

#pragma endregion