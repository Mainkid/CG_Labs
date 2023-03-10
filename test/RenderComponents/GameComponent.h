#pragma once
#include <memory>


class Game;

class GameComponent
{
public:
	virtual void Render() = 0;
	virtual void Update(float deltaSec) = 0;
	virtual void Initialize() = 0;
	virtual void InitializeShaders() = 0;

protected:
	std::shared_ptr<Game> game;
	
};

