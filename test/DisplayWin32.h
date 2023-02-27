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




#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

class Game;

class DisplayWin32
{
public:
	DisplayWin32(Game* _game,LPCWSTR applicationName, HINSTANCE hInstance, int screenHeight = 800, int screenWidth = 800);

	~DisplayWin32();

	HWND GetHWND();

	int GetHeight();

	int GetWidth();



private:
	

	int clientHeight;
	int clientWidth;
	HINSTANCE hInstance;
	HWND hWnd;
	WNDCLASSEX wc;

	Game* game;

	

};

