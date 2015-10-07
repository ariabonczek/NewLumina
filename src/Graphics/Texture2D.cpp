#include <Graphics\Texture2D.hpp>

NS_BEGIN

#if DX11
	Texture2D::Texture2D(Image& imageData, GUID guid, ID3D11Device* device)
#elif DX12
	Texture2D::Texture2D(Image& imageData, GUID guid, ID3D12Device* device)
#elif GL43
	Texture2D::Texture2D(Image& imageData, GUID guid)
#endif
	: Resource(guid)
{
#if DX11
	ID3D11Texture2D* texture;
	HRESULT hr;
	// Create the texture

	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));

	td.Width = imageData.width;
	td.Height = imageData.height;
	td.ArraySize = 1;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.MipLevels = 1;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	UINT rowpitch = 4 * imageData.width;
	UINT imagesize = rowpitch * imageData.height;

	// Copy the memory
	uint8* temp = new uint8[imagesize];

	// May want to put this in a different function i.e. CopyPixels
	if (imageData.components == 4)
	{
		std::memcpy(temp, imageData.data, imagesize * sizeof(uint8_t));
	}
	else if (imageData.components == 3)
	{
		// foreach pixel
		for (uint i = 0; i < imageData.width * imageData.height; i++)
		{
			// copy the rgb
			for (uint j = 0; j < 3; j++)
			{
				temp[i * 4 + j] = imageData.data[i * 3 + j];
			}
			// set alpha to 1
			temp[i * 4 + 3] = 1;
		}
	}
	else if (imageData.components == 1)
	{
		// foreach pixel
		for (uint i = 0; i < imageData.width * imageData.height; i++)
		{
			// copy the rgb
			for (uint j = 0; j < 3; j++)
			{
				temp[i * 4 + j] = imageData.data[i];
			}
			// set alpha to 1
			temp[i * 4 + 3] = 1;
		}
	}

	D3D11_SUBRESOURCE_DATA srd;
	ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));
	srd.pSysMem = temp;
	srd.SysMemPitch = rowpitch;
	srd.SysMemSlicePitch = imagesize;

	device->CreateTexture2D(&td, &srd, &texture);

	// Create the shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	srvd.Format = td.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(texture, &srvd, &srv);

	DELETECOM(texture);
	delete[] temp;
#elif DX12
	
	UINT rowpitch = 4 * imageData.width;
	UINT imagesize = rowpitch * imageData.height;

	uint8* temp = new uint8[imagesize];
	// May want to put this in a different function i.e. CopyPixels
	if (imageData.components == 4)
	{
		std::memcpy(temp, imageData.data, imagesize * sizeof(uint8_t));
	}
	else if (imageData.components == 3)
	{
		// foreach pixel
		for (uint i = 0; i < imageData.width * imageData.height; i++)
		{
			// copy the rgb
			for (uint j = 0; j < 3; j++)
			{
				temp[i * 4 + j] = imageData.data[i * 3 + j];
			}
			// set alpha to 1
			temp[i * 4 + 3] = 1;
		}
	}
	else if (imageData.components == 1)
	{
		// foreach pixel
		for (uint i = 0; i < imageData.width * imageData.height; i++)
		{
			// copy the rgb
			for (uint j = 0; j < 3; j++)
			{
				temp[i * 4 + j] = imageData.data[i];
			}
			// set alpha to 1
			temp[i * 4 + 3] = 1;
		}
	}

	// Create the Texture2D

	D3D12_RESOURCE_DESC td;
	ZeroMemory(&td, sizeof(D3D12_RESOURCE_DESC));
	td.MipLevels = 1;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.Width = imageData.width;
	td.Height = imageData.height;
	td.Flags = D3D12_RESOURCE_FLAG_NONE;
	td.DepthOrArraySize = 1;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	D3D12_HEAP_PROPERTIES hp;
	ZeroMemory(&hp, sizeof(D3D12_HEAP_PROPERTIES));
	hp.Type = D3D12_HEAP_TYPE_DEFAULT;
	hp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	hp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	hp.CreationNodeMask = 1;
	hp.VisibleNodeMask = 1;

	device->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &td, D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr, __uuidof(ID3D12Resource), (void**)&texture);

	uint8* temp2;
	texture->Map(0, nullptr, (void**)&temp2);
	memcpy(temp, imageData.data, sizeof(uint8) * imagesize);
	texture->Unmap(0, nullptr);

	D3D12_DESCRIPTOR_HEAP_DESC srvhd;
	ZeroMemory(&srvhd, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	srvhd.NumDescriptors = 1;
	srvhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvhd.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	device->CreateDescriptorHeap(&srvhd, __uuidof(ID3D12DescriptorHeap), (void**)&srvHeap);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(D3D12_SHADER_RESOURCE_VIEW_DESC));
	srvd.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvd.Format = td.Format;
	srvd.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels = 1;
	device->CreateShaderResourceView(texture, &srvd, srvHeap->GetCPUDescriptorHandleForHeapStart());

	delete[] temp;

#elif GL43

#endif
}

Texture2D::~Texture2D()
{

}

NS_END