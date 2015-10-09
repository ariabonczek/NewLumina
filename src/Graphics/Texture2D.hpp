#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include <Core\Common.hpp>
#include <Graphics\Sampler.hpp>
#include <Utility\Resources\Resource.hpp>

NS_BEGIN

class Texture2D : public Resource
{
public:
#if DX11
	Texture2D(Image& imageData, LGUID guid, ID3D11Device* device);
#elif DX12
	Texture2D(Image& imageData, LGUID guid, ID3D12Device* device);
#elif GL43
	Texture2D(Image& imageData, LGUID guid);
#endif
	~Texture2D();

	Sampler* GetSampler()const;
	ID3D11ShaderResourceView* GetShaderResourceView()const;

private:
	Sampler* sampler;
#if DX11
	ID3D11ShaderResourceView* srv;
#elif DX12
	ID3D12Resource* texture;
	ID3D12DescriptorHeap* srvHeap;
#endif
};

NS_END


#endif