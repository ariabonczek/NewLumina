#include <Graphics\RenderTexture.hpp>

NS_BEGIN

RenderTexture::RenderTexture(uint32 width, uint32 height, LGUID guid, ID3D11Device* device, uint32 arraySize):
	Texture(guid)
{
	ID3D11Texture2D* texture;
	// Create the texture

	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));

	td.Width = width;
	td.Height = height;
	td.ArraySize = arraySize;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.MipLevels = 1;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

	device->CreateTexture2D(&td, 0, &texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(srvd));

	srvd.Format = td.Format;
	if (arraySize == 1)
	{
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	}
	else
	{
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	}

	device->CreateShaderResourceView(texture, &srvd, &srv);

	D3D11_RENDER_TARGET_VIEW_DESC rtvd;
	ZeroMemory(&rtvd, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	rtvd.Format = td.Format;
	if (arraySize == 1)
	{
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	}
	else
	{
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	}
	rtvd.Texture2D.MipSlice = 0;

	device->CreateRenderTargetView(texture, &rtvd, &rtv);

	DELETECOM(texture);
}

RenderTexture::~RenderTexture()
{
	DELETECOM(srv);
	DELETECOM(rtv);
}


NS_END