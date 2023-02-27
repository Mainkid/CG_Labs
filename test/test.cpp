#include "DisplayWin32.h"
#include "Game.h"
#include "GeometricPrimitive.h"


int main()
{
	LPCWSTR applicationName = L"My3DApp";
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	auto screenWidth = 800;
	auto screenHeight = 800;



	std::shared_ptr<Game> myGame = std::make_shared<Game>(applicationName, hInstance, screenWidth, screenHeight);


	
	std::shared_ptr<GameComponent> sun =std::make_shared<Planet>(myGame,1.0f,0.2f,0.0f,0.0f,L"Textures\\sun.png");
	std::shared_ptr<GameComponent> mercury =std::make_shared<Planet>(myGame,0.2f,3.0f,1.2f,1.5f,L"Textures\\mercury.png");
	std::shared_ptr<GameComponent> venus =std::make_shared<Planet>(myGame,0.4f,2.5f,1.1f,3.0f,L"Textures\\venus.jpg");
	std::shared_ptr<GameComponent> earth =std::make_shared<Planet>(myGame,0.3f,2.0f,1.0f,4.5f,L"Textures\\earth.jpg");
	std::shared_ptr<GameComponent> mars =std::make_shared<Planet>(myGame,0.3f,1.9f,0.8f,5.5f,L"Textures\\mars.jpg");
	std::shared_ptr<GameComponent> jupyter =std::make_shared<Planet>(myGame,0.8f,1.6f,0.6f,6.5f,L"Textures\\jupyter.jpg");
	std::shared_ptr<GameComponent> saturn =std::make_shared<Planet>(myGame,0.7f,1.4f,0.5f,7.5f,L"Textures\\saturn.jpg");
	std::shared_ptr<GameComponent> uranus =std::make_shared<Planet>(myGame,0.6f,1.0f,0.4f,9.0f,L"Textures\\uranus.png");
	std::shared_ptr<GameComponent> neptune =std::make_shared<Planet>(myGame,0.7f,0.7f,0.3f,10.0f,L"Textures\\neptune.jpg");
	std::shared_ptr<GameComponent> pluto =std::make_shared<Planet>(myGame,0.1f,1.7f,0.1f,10.0f,L"Textures\\sun.png");

	
	myGame->AddGameComponent(sun);
	myGame->AddGameComponent(mercury);
	myGame->AddGameComponent(venus);
	myGame->AddGameComponent(earth);
	myGame->AddGameComponent(mars);
	myGame->AddGameComponent(jupyter);
	myGame->AddGameComponent(saturn);
	myGame->AddGameComponent(uranus);
	myGame->AddGameComponent(neptune);
	myGame->AddGameComponent(pluto);
	
	
	myGame->InitializeDirectX();

	myGame->StartGameLoop();

}

