#include "Ball.h"
#include "Game.h"
#include <algorithm>
#include <math.h>


Ball::Ball(Game* game,GameComponent* racket, GameComponent* ai)
{
	this->game = game;
	this->racket = dynamic_cast<TriangleComponent*>(racket);
	this->ai = dynamic_cast<SimpleAI*>(ai);
	prevOffset.x = 0;
	prevOffset.y = 0;
}

void Ball::Initialize()
{
	vertexBC = nullptr;

	D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };

	HRESULT res = D3DCompileFromFile(L"./Shaders/Racket.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexBC,
		&errorVertexCode);

	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(game->window->GetHWND(), L"./Shaders/Racket.hlsl", L"Missing Shader File", MB_OK);
		}

	}




	res = D3DCompileFromFile(L"./Shaders/Racket.hlsl", Shader_Macros /*macros*/, nullptr /*include*/, "PSMain", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelBC, &errorPixelCode);


	res = game->device->CreateVertexShader(
		this->vertexBC->GetBufferPointer(),
		this->vertexBC->GetBufferSize(),
		nullptr, &this->vertexShader);

	res = game->device->CreatePixelShader(
		this->pixelBC->GetBufferPointer(),
		this->pixelBC->GetBufferSize(),
		nullptr, &this->pixelShader);

	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};


	res = game->device->CreateInputLayout(
		inputElements,
		2,
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		&layout);

	DirectX::XMFLOAT4 points[8] = {
		DirectX::XMFLOAT4(-0.02f, 0.02f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-0.02f, -0.02f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.02f, 0.02f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.02f, -0.02f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	};


	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * std::size(points);

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;


	res = game->device->CreateBuffer(&vertexBufDesc, &vertexData, &vb);

	int indeces[] = { 0,1,2, 1,2,3 };
	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * std::size(indeces);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indeces;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;


	res = game->device->CreateBuffer(&indexBufDesc, &indexData, &ib);

	D3D11_BUFFER_DESC constantBufDesc = {};
	constantBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufDesc.MiscFlags = 0;
	constantBufDesc.ByteWidth = sizeof(CB_VS_vertexshader) + (16 - sizeof(CB_VS_vertexshader)) % 16;
	constantBufDesc.StructureByteStride = 0;

	res = game->device->CreateBuffer(&constantBufDesc, 0, &constantBuffer);


	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	res = game->device->CreateRasterizerState(&rastDesc, &rastState);

	game->context->RSSetState(rastState);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(800);
	viewport.Height = static_cast<float>(800);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	game->context->RSSetViewports(1, &viewport);

	boundingSphere.Center = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	boundingSphere.Radius = 0.05f;

	topBorder.Center = DirectX::XMFLOAT3(0.0f, 1.1f, 0.0f);
	topBorder.Extents = DirectX::XMFLOAT3(1.0f, 0.05f, 0.1f);

	botBorder.Center = DirectX::XMFLOAT3(0.0f, -1.1f, 0.0f);
	botBorder.Extents = DirectX::XMFLOAT3(1.0f, 0.05f, 0.1f);

	direction.x = -1;
	direction.y = 0;
}

void Ball::Render()
{

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT res = game->context->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	CopyMemory(mappedResource.pData, &data, sizeof(CB_VS_vertexshader));
	game->context->Unmap(constantBuffer, 0);
	game->context->VSSetConstantBuffers(0, 1, &constantBuffer);


	game->context->IASetInputLayout(layout);
	game->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	game->context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
	game->context->IASetVertexBuffers(0, 1, &vb, strides, offsets);
	game->context->VSSetShader(this->vertexShader, nullptr, 0);
	game->context->PSSetShader(this->pixelShader, nullptr, 0);


	game->context->OMSetRenderTargets(1, &game->rtv, nullptr);

	game->context->DrawIndexed(6, 0, 0);

	game->context->OMSetRenderTargets(0, nullptr, nullptr);



}

void Ball::Update(float deltaSec)
{
	

	//Обработка коллизий

	DirectX::BoundingBox player1BB= racket->GetBox();
	DirectX::BoundingBox aiBB = ai->GetBox();

	boundingSphere.Center.x += direction.x * speed * deltaSec;
	boundingSphere.Center.y += direction.y * speed * deltaSec;

	prevOffset.x = data.xOffset;
	prevOffset.y = data.yOffset;

	data.xOffset += direction.x * speed * deltaSec;
	data.yOffset += direction.y * speed * deltaSec;
	

	if (data.xOffset <= -1)
	{
		float y = (prevOffset.y - data.yOffset) * (-1 - data.xOffset) / (prevOffset.x - data.xOffset) + data.yOffset;
		if (y < player1BB.Center.y + 0.2f && y > player1BB.Center.y - 0.2f)
		{
			std::cout << "GG";
			data.xOffset = prevOffset.x;
			data.yOffset = prevOffset.y;
			boundingSphere.Center.x = prevOffset.x;
			boundingSphere.Center.y = prevOffset.y;

			DirectX::SimpleMath::Vector2 a(0.2f, 0.f);
			DirectX::SimpleMath::Vector2 bv(0, boundingSphere.Center.y - player1BB.Center.y);
			DirectX::SimpleMath::Vector2 c = (a + bv);
			c.Normalize();
			direction = c;
			speed *= 1.1f;
		}
	}
	else if (data.xOffset >= 1)
	{
		float y = (prevOffset.y - data.yOffset) * (1 - data.xOffset) / (prevOffset.x - data.xOffset) + data.yOffset;
		if (y < aiBB.Center.y + 0.2f && y > aiBB.Center.y - 0.2f)
		{
			std::cout << "GG";
			data.xOffset = prevOffset.x;
			data.yOffset = prevOffset.y;
			boundingSphere.Center.x = prevOffset.x;
			boundingSphere.Center.y = prevOffset.y;

			DirectX::SimpleMath::Vector2 a(-0.2f, 0.f);
			DirectX::SimpleMath::Vector2 bv(0, boundingSphere.Center.y - aiBB.Center.y);
			DirectX::SimpleMath::Vector2 c = (a + bv);
			c.Normalize();
			direction = c;
			speed *= 1.1f;
		}


	}
	//else if (data.yOffset >= 1)
	//{


	//}
	else
	{
		if (boundingSphere.Intersects(player1BB))
		{
			DirectX::SimpleMath::Vector2 a(0.2f, 0.f);
			DirectX::SimpleMath::Vector2 bv(0, boundingSphere.Center.y - player1BB.Center.y);
			DirectX::SimpleMath::Vector2 c = (a + bv);
			c.Normalize();
			direction = c;
			speed *= 1.1f;

			data.xOffset = boundingSphere.Center.x;
			data.yOffset = boundingSphere.Center.y;

		}
		else if (boundingSphere.Intersects(aiBB))
		{
			DirectX::SimpleMath::Vector2 a(-0.2f, 0.f);
			DirectX::SimpleMath::Vector2 bv(0, boundingSphere.Center.y - aiBB.Center.y);
			DirectX::SimpleMath::Vector2 c = (a + bv);
			c.Normalize();
			direction = c;
			speed *= 1.1f;

			data.xOffset = boundingSphere.Center.x;
			data.yOffset = boundingSphere.Center.y;

		}
		else if (boundingSphere.Intersects(topBorder) || boundingSphere.Intersects(botBorder))
		{
			direction.y *= -1;


		}
	
	}
	
	
	

	


	// Забит гол
	if (abs( data.xOffset) > 2 || abs(data.yOffset) > 2)
	{
		std::cout << prevOffset.x << std::endl;
		std::cout << data.xOffset << std::endl;
		
		data.xOffset = 0;
		data.yOffset = 0;
		direction.x = -1;
		direction.y = 0;
		boundingSphere.Center.x = 0;
		boundingSphere.Center.y = 0;
		boundingSphere.Center.z = 0;
		speed = 1.0f;
	}

	


	//Управление AI

	if (ai->data.yOffset > data.yOffset&& ai->data.yOffset >= -0.8f ||
		ai->data.yOffset < data.yOffset && ai->data.yOffset <= 0.8f)
	{

		if (abs(ai->data.yOffset - data.yOffset) <= deltaSec * 1.5f)
		{
			ai->data.yOffset = data.yOffset;
			ai->boundingBox2.Center.y = data.yOffset;
		}
		else
		{
			if (ai->data.yOffset > data.yOffset)
			{
				ai->data.yOffset -= deltaSec * 1.5f;
				ai->boundingBox2.Center.y -= deltaSec * 1.5f;
			}
			else
			{
				ai->data.yOffset += deltaSec * 1.5f;
				ai->boundingBox2.Center.y += deltaSec * 1.5f;
			}
		}
		
		ai->data.yOffset = (std::max) (-0.8f, (std::min)(ai->data.yOffset, 0.8f));
		ai->boundingBox2.Center.y = ai->data.yOffset;
	}
	
	speed = min(speed, 4.0f);

}

void Ball::InitializeShaders()
{


}
