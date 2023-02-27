#include "Planet.h"
#include "../Game.h"


Planet::Planet(std::shared_ptr<Game> game,float _r,float _axisAngle=3.0f,float _orbitAngle=0.5f,float _orbitR=0.0f,LPCWSTR _filepath=L"")
{
	this->game = game;
	this->axeAngle=_axisAngle;
	this->orbitAngle=_orbitAngle;
	this->filePath=_filepath;
	this->orbitR = _orbitR;
	this->r=_r;
}

void Planet::Initialize()
{
	
	D3D11_BUFFER_DESC constantBufDesc = {};
	constantBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufDesc.MiscFlags = 0;
	constantBufDesc.ByteWidth = sizeof(CB_VS_vertexshader) + (16 - sizeof(CB_VS_vertexshader)) % 16;
	constantBufDesc.StructureByteStride = 0;
	
	game->device->CreateBuffer(&constantBufDesc, 0, constantBuffer.GetAddressOf());


	HRESULT res=DirectX::CreateWICTextureFromFile(game->device.Get(),filePath,nullptr,texture.GetAddressOf());

	shape = DirectX::GeometricPrimitive::CreateSphere(game->context.Get(),r,32,false,true);


}

void Planet::Render()
{
	//!!!! Setting const buffer
	//data.worldViewMat=
	
	
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT res= game->context->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	CopyMemory(mappedResource.pData, &game->worldViewData, sizeof(CB_VS_vertexshader));
	game->context->Unmap(constantBuffer.Get(), 0);
	game->context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

	
	



	game->context->OMSetRenderTargets(1, game->rtv.GetAddressOf(), game->depthStencilView.Get());

	D3D11_DEPTH_STENCIL_DESC depthstencildesc;
	ZeroMemory(&depthstencildesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthstencildesc.DepthEnable = true;
	depthstencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthstencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	HRESULT hr = game->device->CreateDepthStencilState(&depthstencildesc, game->depthStencilState.GetAddressOf());

	//game->context->DrawIndexed(6, 0, 0);
	auto a=game->camera->GetViewMatrix();
	shape->SetDepthBufferMode(false);

	
	
	shape->Draw( modelMat*rotationAxisMat*DirectX::XMMatrixTranslation(orbitR,0.0f,0.0f)*rotationOrbitMat,
		game->camera->GetViewMatrix(),game->camera->GetProjectionMatrix(),DirectX::Colors::White,texture.Get(),false);
	game->context->OMSetRenderTargets(0, nullptr, nullptr);

	

}



void Planet::Update(float deltaSec)
{
	rotationAxisMat*=DirectX::XMMatrixRotationY(axeAngle*0.01f);
	rotationOrbitMat*=DirectX::XMMatrixRotationY(orbitAngle*0.01f);
}

void Planet::InitializeShaders()
{
	

}



DirectX::BoundingBox Planet::GetBox()
{
	return boundingBox;
}
