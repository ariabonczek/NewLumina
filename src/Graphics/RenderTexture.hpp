#ifndef RENDER_TEXTURE_HPP
#define RENDER_TEXTURE_HPP

#include <Core\Common.hpp>
#include <Graphics\Texture.hpp>

NS_BEGIN

class RenderTexture : public Texture
{
public:
	RenderTexture(uint32 width, uint32 height, LGUID guid, ID3D11Device* device, uint32 arraySize = 1);
	~RenderTexture();

	ID3D11RenderTargetView* GetRenderTargetView()const;
private:
	ID3D11RenderTargetView* rtv;
};

NS_END

#endif