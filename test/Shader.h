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


class VertexShader
{
public:
	bool Initialize(ID3D11Device& device, std::wstring shaderPath,Game* game,D3D11_INPUT_ELEMENT_DESC * desc, UINT numElements);
	ID3D11VertexShader* GetShader();
	ID3DBlob* GetBuffer();
	ID3D11InputLayout* GetInputLayout();
private:
	ID3D11VertexShader* vertexShader = nullptr;
	ID3DBlob* shaderBuffer = nullptr;
	ID3D11InputLayout* inputLayout;
	Game* game = nullptr;

};

class PixelShader
{
public:
	bool Initialize(ID3D11Device& device, std::wstring shaderpath);
	ID3D11PixelShader* GetShader();
	ID3D10Blob* GetBuffer();

private:
	ID3D11PixelShader* pixelShader = nullptr;
	ID3DBlob* shaderBuffer = nullptr;
	Game* game = nullptr;
};

