#include <Graphics\FullScreenQuad.hpp>

NS_BEGIN

FullScreenQuad::FullScreenQuad()
{}

FullScreenQuad::~FullScreenQuad()
{}

void FullScreenQuad::Initialize(ID3D11Device* device)
{
	float vertices[20] = {
		-1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 1.0f
	};

	uint16 indices[6] = {
		0, 1, 2,
		2, 1, 3
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.MiscFlags = 0;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA srd;
	srd.pSysMem = vertices;
	HRESULT hr = device->CreateBuffer(&bd, &srd, &vertexBuffer);

	bd.ByteWidth = sizeof(uint16) * 6;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	srd.pSysMem = indices;
	hr = device->CreateBuffer(&bd, &srd, &indexBuffer);
}

void FullScreenQuad::Render(ID3D11DeviceContext* deviceContext)
{
	uint stride = sizeof(float) * 5;
	uint offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	deviceContext->DrawIndexed(6, 0, 0);
}

NS_END