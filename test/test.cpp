#include "DisplayWin32.h"
#include "Game.h"

#include "AnotherComponent.h"
#include "DotedLine.h"
#include "Ball.h"
#include "SimpleAI.h"

int main()
{
	LPCWSTR applicationName = L"My3DApp";
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	auto screenWidth = 800;
	auto screenHeight = 800;

#pragma region Window init
	Game* myGame = new Game(applicationName, hInstance, screenWidth, screenHeight);

#pragma endregion Window init


	
	GameComponent* playerRacket = new TriangleComponent(myGame); //!!! УБРАТЬ
	GameComponent* dotedLine = new DotedLine(myGame);
	GameComponent* simpleAI = new SimpleAI(myGame);
	GameComponent* ball = new Ball(myGame,playerRacket,simpleAI);
	

	myGame->AddGameComponent(ball);
	myGame->AddGameComponent(playerRacket);
	myGame->AddGameComponent(dotedLine);
	myGame->AddGameComponent(simpleAI);

	myGame->InitializeDirectX();

	myGame->StartGameLoop();

}

