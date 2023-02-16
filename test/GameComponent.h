#pragma once
#include "Shader.h"

class Game;

class GameComponent
{
public:
	virtual void Render() = 0;
	virtual void Update(float deltaSec) = 0;
	virtual void Initialize() = 0;
	virtual void InitializeShaders() = 0;

protected:
	Game* game;
	VertexShader* vertexShader;
	
};

