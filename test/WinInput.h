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
#include "Keyboard.h"
#include "Mouse.h"

class Game;

class WinInput
{

public:
	WinInput(Game* _game);
	~WinInput();

	bool IsKeyDown(int vKey);

	void UpdateInput();

	//Доделать
	void OnKeyDown();

	void OnMouseDown();

	bool ProcessMessages();

	void GetInput();

private:
	POINT cursorPos;
	POINT mouseOffset;
	Game* game;
	Keyboard* keyboard;
	Mouse* mouse;
};

