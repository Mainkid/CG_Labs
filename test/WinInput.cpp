#include "WinInput.h"
#include "Game.h"

WinInput::WinInput(Game* _game)
{
	game = _game;
}

bool WinInput::IsKeyDown(int vKey)
{
	SHORT state = GetAsyncKeyState(vKey);
	if ((1 << 15) & state)
		return true;
	return false;
}

void WinInput::UpdateInput()
{
	

	POINT oldPosition = cursorPos;

	GetCursorPos(&cursorPos);
	
	mouseOffset.x = cursorPos.x - oldPosition.x;
	mouseOffset.y = cursorPos.y - oldPosition.y;
}

void WinInput::OnKeyDown()
{

}

void WinInput::OnMouseDown()
{

}

bool WinInput::ProcessMessages()
{
	UpdateInput();

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT)
	{
		if (IsWindow(game->GetWindow32HWND()))
		{
			//hWnd = NULL;
			return false;
		}
	}
	else if (msg.message == WM_KEYDOWN)
	{
		std::cout << "OK" << std::endl;
	}

	return true;
}

