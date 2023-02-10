#include "WinInput.h"
#include "Game.h"
#include <string>

WinInput::WinInput(Game* _game)
{
	game = _game;
	keyboard = new Keyboard();
	mouse = new Mouse();
}

WinInput::~WinInput()
{
	delete keyboard;
	delete mouse;
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

   	switch (msg.message)
	{
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(msg.lParam);
		int y = HIWORD(msg.lParam);
		mouse->OnMouseMove(x, y);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(msg.lParam);
		int y = HIWORD(msg.lParam);
		mouse->OnLeftPressed(x, y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(msg.lParam);
		int y = HIWORD(msg.lParam);
		mouse->OnRightPressed(x, y);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(msg.lParam);
		int y = HIWORD(msg.lParam);
		mouse->OnMiddlePressed(x, y);
		break;
	}
	case WM_LBUTTONUP:
	{
		int x = LOWORD(msg.lParam);
		int y = HIWORD(msg.lParam);
		mouse->OnLeftReleased(x, y);
		break;
	}
	case WM_RBUTTONUP:
	{
		int x = LOWORD(msg.lParam);
		int y = HIWORD(msg.lParam);
		mouse->OnRightReleased(x, y);
		break;
	}
	case WM_MBUTTONUP:
	{
		int x = LOWORD(msg.lParam);
		int y = HIWORD(msg.lParam);
		mouse->OnMiddleReleased(x, y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(msg.lParam);
		int y = HIWORD(msg.lParam);
		if (GET_WHEEL_DELTA_WPARAM(msg.wParam) > 0)
		{
			mouse->OnWheelUp(x, y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(msg.wParam) < 0)
		{
			mouse->OnWheelDown(x, y);
		}
		break;
	}

// Keyboard messages
	case WM_KEYDOWN:
	{
		unsigned char ch = static_cast<unsigned char>(msg.wParam);

		if (keyboard->IsKeysAutoRepeat())
		{
			keyboard->OnKeyPressed(ch);
		}
		else
		{
			const bool wasPressed = msg.lParam & 0x40000000;
			if (!wasPressed)
			{
				keyboard->OnKeyPressed(ch);
			}
		}
		
		break;
	}
	case WM_KEYUP:
	{
		unsigned char ch = static_cast<unsigned char>(msg.wParam);
		keyboard->OnKeyReleased(ch);
		break;
	}
	case WM_QUIT:
	{
		if (IsWindow(game->GetWindow32HWND()))
		{
			//hWnd = NULL;
			return false;
		}
	}

	}

	return true;
}

void WinInput::GetInput()
{
	while (!keyboard->KeyBufferIsEmpty())
	{
		KeyboardEvent kbe = keyboard->ReadKey();
		unsigned char keycode = kbe.GetKeyCode();
		std::string outmsg = "Keycode: ";
		if (kbe.IsPress())
		{
			outmsg += "Key press: ";
		}
		else if (kbe.IsRelease())
		{
			outmsg += "Key release: ";
		}

		outmsg += keycode;
		outmsg += "\n";
		std::cout << outmsg;
	}

	while (!mouse->EventBufferIsEmpty())
	{
		MouseEvent me = mouse->ReadEvent();

		if (me.GetType() == MouseEvent::EventType::WheelUp)
		{
			std::cout << "MouseWheelUp";
		}
		else if (me.GetType() == MouseEvent::EventType::WheelDown)
		{
			std::cout << "MouseWheelDown";
		}
		else if (me.GetType() == MouseEvent::EventType::Move)
		{
			std::string outmsg = "X: ";
			outmsg += std::to_string(me.GetPosX());
			outmsg += " Y: ";
			outmsg += std::to_string(me.GetPosY());
			std::cout << outmsg << std::endl;
		}
		else if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
		{
			std::string outmsg = "RAW X: ";
			outmsg += std::to_string(me.GetPosX());
			outmsg += " Y: ";
			outmsg += std::to_string(me.GetPosY());
			std::cout << outmsg << std::endl;
		}
		else
		{
			std::cout << "Mouse click event" << std::endl;
		}

		

	}
}