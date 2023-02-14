#include "DisplayWin32.h"
#include "Game.h"





int main()
{
	LPCWSTR applicationName = L"My3DApp";
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	auto screenWidth = 800;
	auto screenHeight = 800;

#pragma region Window init
	Game myGame;
	myGame.CreateWindow32(applicationName, hInstance,screenWidth, screenHeight);
#pragma endregion Window init


	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

	myGame.SetSwapDesc();

	myGame.CreateDeviceAndSwapChain();

	myGame.CompileFromFile(L"MyVeryFirstShader.hlsl");

	myGame.InitializeDirectX();

	myGame.StartGameLoop();

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
