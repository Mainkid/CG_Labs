#pragma once

#include <windows.h>
#include <WinUser.h>
#include <wrl.h>
#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "GameComponent.h"

#include "GeometricPrimitive.h"
#include "../ConstantBufferTypes.h"
#include "../DirectXSDK/SimpleMath.h"
#include "WICTextureLoader.h"

#include <DirectXMath.h>
#include <DirectXCollision.h>






class Planet : public GameComponent
{
public:
	Planet(std::shared_ptr<Game> g,float radius,float _axisAngle,float _orbitAngle,float orbitR,LPCWSTR filepath);
	void Initialize() override;
	void Render() override;
	void Update(float deltaSec) override;
	void InitializeShaders() override;
	//void ChangeMousePos(const InputDevice::MouseMoveEventArgs&);
	DirectX::BoundingBox GetBox();
	CB_VS_vertexshader data;

	DirectX::XMMATRIX modelMat= DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX rotationOrbitMat=DirectX::XMMatrixRotationY(0.0f);
	DirectX::XMMATRIX rotationAxisMat = DirectX::XMMatrixRotationY(0.0f);

	float orbitAngle=1.0f;
	float axeAngle = 3.0f;
	float orbitR = 0.0f;
	float r=1.0f;
	LPCWSTR filePath;

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
	Microsoft::WRL::ComPtr<ID3DBlob> vertexBC; //m
	Microsoft::WRL::ComPtr<ID3DBlob> pixelBC;  //m
	UINT strides[1] = { 32 }; //m
	UINT offsets[1] = { 0 }; //m
	D3D11_VIEWPORT viewport = {};
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rastState;
	Microsoft::WRL::ComPtr<ID3DBlob> errorPixelCode = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorVertexCode = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> ib;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vb;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
	Microsoft::WRL::ComPtr <ID3D11SamplerState> samplerState;
	std::unique_ptr<DirectX::GeometricPrimitive> shape;


	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	
	CD3D11_RASTERIZER_DESC rastDesc = {};


	float offX;
	float offY;

	
	DirectX::BoundingBox boundingBox;

};

