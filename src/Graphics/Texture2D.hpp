#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include <Core\Common.hpp>
#include <Graphics\Texture.hpp>

NS_BEGIN

/// <summary>
///
/// </summary>
class Texture2D : public Texture
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
private:
#if DX12
	ID3D12Resource* texture;
	ID3D12DescriptorHeap* srvHeap;
#endif
};

NS_END


#endif