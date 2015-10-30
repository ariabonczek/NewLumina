#include <Graphics\Cubemap.hpp>

#include <Graphics\Texture2D.hpp>
#include <Graphics\RenderTexture.hpp>

NS_BEGIN

Cubemap::Cubemap(LGUID guid, bool baked):
	Texture::Texture(guid), isBaked(false)
{
	sampler = Sampler::WrapLinear;
}

Cubemap::~Cubemap()
{} 

void Cubemap::SetTextures(Image* images, ID3D11Device* device)
{
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));

	td.Width = images[0].width;
	td.Height = images[0].height;
	td.ArraySize = 6;
	td.MipLevels = 1;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.MipLevels = 1;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	td.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	srvd.Format = td.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvd.TextureCube.MipLevels = td.MipLevels;
	srvd.TextureCube.MostDetailedMip = 0;

	D3D11_SUBRESOURCE_DATA srd[6];

	UINT rowpitch = 4 * images[0].width;
	UINT imagesize = rowpitch * images[0].height;

	// Copy the memory
	uint8* temp = new uint8[imagesize * 6];

	for (uint i = 0; i < 6; ++i)
	{
		CopyPixels(images[i], temp + imagesize * i, imagesize);

		srd[i].pSysMem = temp + imagesize * i;
		srd[i].SysMemPitch = rowpitch;
		srd[i].SysMemSlicePitch = 0;
	}
	ID3D11Texture2D* tex;
	HRESULT hr = device->CreateTexture2D(&td, &srd[0], &tex);
#if _DEBUG
	assert(hr == S_OK);
#endif
	hr = device->CreateShaderResourceView(tex, &srvd, &srv);
	
#if _DEBUG
	assert(hr == S_OK);
#endif

	DELETECOM(tex);
	delete[] temp;
}

NS_END