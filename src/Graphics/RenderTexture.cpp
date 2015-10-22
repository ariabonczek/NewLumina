#include <Graphics\RenderTexture.hpp>

NS_BEGIN

RenderTexture::RenderTexture(uint32 width, uint32 height, LGUID guid, ID3D11Device* device, bool usesDepth, uint32 arraySize):
	Texture(guid)
{
	sampler = Sampler::ClampLinear;

	ID3D11Texture2D* texture;
	// Create the texture

	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));

	td.Width = width;
	td.Height = height;
	td.ArraySize = 1;
	td.SampleDesc.Count = 4;
	td.SampleDesc.Quality = 0;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.MipLevels = 1;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

	device->CreateTexture2D(&td, 0, &texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(srvd));

	srvd.Format = td.Format;
	srvd.Texture2D.MipLevels = 1;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	HRESULT hr = device->CreateShaderResourceView(texture, 0, &srv);

	hr = device->CreateRenderTargetView(texture, 0, &rtv);

	if (usesDepth)
	{
		D3D11_TEXTURE2D_DESC dsd;
		ZeroMemory(&dsd, sizeof(D3D11_TEXTURE2D_DESC));
		dsd.Width = width;
		dsd.Height = height;
		dsd.MipLevels = 1;
		dsd.ArraySize = 1;
		dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsd.Usage = D3D11_USAGE_DEFAULT;
		dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		dsd.CPUAccessFlags = NULL;
		dsd.MiscFlags = NULL;
		dsd.SampleDesc.Count = 4;

		HRESULT hr = device->CreateTexture2D(&dsd, NULL, &depthBuffer);
		hr = device->CreateDepthStencilView(depthBuffer, NULL, &dsv);
	}

	DELETECOM(texture);
}

RenderTexture::~RenderTexture()
{
	DELETECOM(srv);
	DELETECOM(rtv);
	DELETECOM(dsv);
}

ID3D11RenderTargetView* RenderTexture::GetRenderTargetView()const
{
	return rtv;
}

ID3D11DepthStencilView* RenderTexture::GetDepthStencilView()const
{
	return dsv;
}

NS_END