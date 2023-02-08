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

class Game;

class WinInput
{

public:
	WinInput(Game* _game);

	bool IsKeyDown(int vKey);

	void UpdateInput();

	//Доделать
	void OnKeyDown();

	void OnMouseDown();

	bool ProcessMessages();

private:
	POINT cursorPos;
	POINT mouseOffset;
	Game* game;
};

