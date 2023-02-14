#pragma once

class Game;

class GameComponent
{
public:
	virtual void Render() = 0;
	virtual void Update(float deltaSec) = 0;
	virtual void Initialize() = 0;

protected:
	Game* game;
	
};

