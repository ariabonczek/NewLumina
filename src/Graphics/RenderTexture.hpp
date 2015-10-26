#ifndef RENDER_TEXTURE_HPP
#define RENDER_TEXTURE_HPP

#include <Core\Common.hpp>
#include <Graphics\Texture.hpp>

NS_BEGIN

class RenderTexture : public Texture
{
public:
	RenderTexture(uint32 width, uint32 height, LGUID guid, ID3D11Device* device, bool usesDepth = false, uint32 arraySize = 1);
	~RenderTexture();

	void SetAsRenderTarget();

	ID3D11RenderTargetView* GetRenderTargetView()const;
	ID3D11DepthStencilView* GetDepthStencilView()const;
	ID3D11ShaderResourceView* GetDepthShaderResourceView()const;
private:
	ID3D11RenderTargetView* rtv;
	ID3D11ShaderResourceView* depthSrv;
	ID3D11DepthStencilView* dsv;
	ID3D11Texture2D* depthBuffer;
};

NS_END

#endif