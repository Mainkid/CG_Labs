#include "Shader.h"
#include "Game.h"

bool VertexShader::Initialize(ID3D11Device& device, std::wstring shaderpath, Game* game, D3D11_INPUT_ELEMENT_DESC* desc, UINT numElements)
{
	HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), &shaderBuffer);
	this->game = game;

	hr = game->device->CreateVertexShader(this->shaderBuffer->GetBufferPointer(), this->shaderBuffer->GetBufferSize(), NULL, &vertexShader);
	hr = game->device->CreateInputLayout(desc, numElements,shaderBuffer->GetBufferPointer(), this->shaderBuffer->GetBufferSize(), &inputLayout);
	return true;
}

ID3D11VertexShader* VertexShader::GetShader()
{
	return vertexShader;
}

ID3DBlob* VertexShader::GetBuffer()
{
	return shaderBuffer;
}

ID3D11InputLayout* VertexShader::GetInputLayout()
{
	return this->inputLayout;
}

bool PixelShader::Initialize(ID3D11Device& device, std::wstring shaderpath)
{
	HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), &shaderBuffer);

	hr = game->device->CreatePixelShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &pixelShader);

	return true;
}

ID3D11PixelShader* PixelShader::GetShader()
{
	return pixelShader;
}

ID3D10Blob* PixelShader::GetBuffer()
{
	return shaderBuffer;
}