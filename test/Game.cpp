#include "Game.h"


Game::Game(LPCWSTR applicationName, HINSTANCE hInstance, int width, int height)
{
	
	window = std::make_unique<DisplayWin32> (this, applicationName, hInstance, width, height);
	wInput = std::make_unique<WinInput> (this);
	
}

Game::~Game()
{
}


HWND Game::GetWindow32HWND()
{
	return window->GetHWND();
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



	res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backTex.GetAddressOf());	// __uuidof(ID3D11Texture2D)
	res = device->CreateRenderTargetView(backTex.Get(), nullptr, rtv.GetAddressOf());
}



void Game::InitializeDirectX()
{
	
	CreateDeviceAndSwapChain();

	camera=new Camera();
	camera->SetPosition(0.0f,0.0f,-2.0f);
	camera->SetProjectionValues(90.0f,window->GetWidth()/window->GetHeight(), 0.1f, 1000.0f);

	cameraController=new CameraController(camera,this);

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = window->GetWidth();
	depthStencilDesc.Height = window->GetHeight();
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HRESULT hr = this->device->CreateTexture2D(&depthStencilDesc, NULL, this->depthStencilBuffer.GetAddressOf());
	

	hr = this->device->CreateDepthStencilView(this->depthStencilBuffer.Get(), NULL, this->depthStencilView.GetAddressOf());

	//context->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(800);
	viewport.Height = static_cast<float>(800);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	context->RSSetViewports(1, &viewport);

	for (auto component = gameComponents.begin(); component != gameComponents.end(); component++)
		component->get()->Initialize();
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

void Game::AddGameComponent(std::shared_ptr<GameComponent> gc)
{
	gameComponents.push_back(std::move(gc));
}


#pragma region GameLoopPattern

void Game::GetInput()
{
	wInput->GetInput();
	
	
}

void Game::Render()
{

	float bgColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	context->ClearRenderTargetView(rtv.Get(), bgColor);
	context->ClearDepthStencilView(depthStencilView.Get(),D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL,1.0f,0);
	context->OMSetRenderTargets(1, rtv.GetAddressOf(), depthStencilView.Get());

	DirectX::XMMATRIX world= DirectX::XMMatrixIdentity();
	//camera->AdjustPosition(0.0f,0.01f,0.0f);
	//camera->SetLookAtPos(DirectX::XMFLOAT3(0.0f,0.0f,0.0f));
	worldViewData.projectMat= camera->GetProjectionMatrix();
	worldViewData.viewMat=world*camera->GetViewMatrix();
	
	
	

	for (auto component = gameComponents.begin(); component != gameComponents.end(); component++)
		component->get()->Render();

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

	cameraController->CameraMovement(deltaTime);
	
	for (auto component = gameComponents.begin(); component != gameComponents.end(); component++)
		component->get()->Update(deltaTime);
}

#pragma endregion