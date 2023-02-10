#include "Mouse.h"

Mouse::Mouse()
{
	/*if (raw_input_initialized == false)
	{
		RAWINPUTDEVICE rid;
		rid.usUsagePage = 0x01;
		rid.usUsage = 0x02;
		rid.dwFlags = 0;
		rid.hwndTarget = NULL;

		if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
		{
			std::cout << "Failed to register device" << std::endl;
			exit(-1);
		}
	}
	raw_input_initialized = true;*/
}


void Mouse::OnLeftPressed(int x, int y)
{
	this->leftIsDown = true;
	MouseEvent me(MouseEvent::EventType::LPress, x, y);
	this->eventBuffer.push(me);
}

void Mouse::OnLeftReleased(int x, int y)
{
	this->leftIsDown = false;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::LRelease, x, y));


}

void Mouse::OnRightPressed(int x, int y)
{
	this->rightIsDown = true;
	MouseEvent me(MouseEvent::EventType::RPress, x, y);
	this->eventBuffer.push(me);
}

void Mouse::OnRightReleased(int x, int y)
{
	this->rightIsDown = false;
	MouseEvent me(MouseEvent::EventType::RRelease, x, y);
	this->eventBuffer.push(me);
}

void Mouse::OnMiddlePressed(int x, int y)
{
	this->mbuttonDown = true;
	MouseEvent me(MouseEvent::EventType::MPress, x, y);
	this->eventBuffer.push(me);
}

void Mouse::OnMiddleReleased(int x, int y)
{
	this->mbuttonDown = false;
	MouseEvent me(MouseEvent::EventType::MRelease, x, y);
	this->eventBuffer.push(me);
}

void Mouse::OnWheelUp(int x, int y)
{
	MouseEvent me(MouseEvent::EventType::WheelUp, x, y);
	this->eventBuffer.push(me);
}

void Mouse::OnWheelDown(int x, int y)
{
	MouseEvent me(MouseEvent::EventType::WheelDown, x, y);
	this->eventBuffer.push(me);
}

void Mouse::OnMouseMove(int x, int y)
{
	this->prevX = this->x;
	this->prevY = this->y;
	this->x = x;
	this->y = y;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::Move, x, y));
	OnMouseMoveRaw(x,y);

}

void Mouse::OnMouseMoveRaw(int x, int y)
{

	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::RAW_MOVE, this->x-this->prevX, this->y-this->prevY));
}

bool Mouse::IsLeftDown()
{
	return this->leftIsDown;
}

bool Mouse::IsRightDown()
{
	return this->rightIsDown;
}

bool Mouse::IsMiddleDown()
{
	return this->mbuttonDown;
}

int Mouse::GetPosX()
{
	return this->x;
}

int Mouse::GetPosY()
{
	return this->y;
}

MousePoint Mouse::GetPos()
{
	return { this->x,this->y };
}

bool Mouse::EventBufferIsEmpty()
{
	return this->eventBuffer.empty();
}

MouseEvent Mouse::ReadEvent()
{
	if (this->eventBuffer.empty())
	{
		return MouseEvent();
	}
	else
	{
		MouseEvent me = this->eventBuffer.front();
		this->eventBuffer.pop();
		return me;
	}
}