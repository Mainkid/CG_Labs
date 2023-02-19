#pragma once
#include <windows.h>
#include <WinUser.h>
#include <wrl.h>
#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "GameComponent.h"
#include "ConstantBufferTypes.h"
#include "SimpleMath.h"
#include <DirectXMath.h>
#include <DirectXCollision.h>



#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")



class TriangleComponent : public GameComponent
{
public:
	TriangleComponent(Game* g);
	void Initialize() override;
	void Render() override;
	void Update(float deltaSec) override;
	void InitializeShaders() override;
	DirectX::BoundingBox GetBox();


private:
	ID3D11InputLayout* layout;
	ID3DBlob* vertexBC; //m
	ID3DBlob* pixelBC;  //m
	UINT strides[1] = { 32 }; //m
	UINT offsets[1] = { 0 }; //m
	D3D11_VIEWPORT viewport = {};
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11RasterizerState* rastState;
	ID3DBlob* errorPixelCode = nullptr;
	ID3DBlob* errorVertexCode = nullptr;
	ID3D11Buffer* ib;
	ID3D11Buffer* vb;
	ID3D11Buffer* constantBuffer;
	CD3D11_RASTERIZER_DESC rastDesc = {};


	CB_VS_vertexshader data;
	DirectX::BoundingBox boundingBox;

};

