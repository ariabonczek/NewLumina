#ifndef POST_PROCESS_HPP
#define POST_PROCESS_HPP

#include <Core\Common.hpp>
#include <Graphics\FullScreenQuad.hpp>

NS_BEGIN

class PostProcess
{
public:
	PostProcess();
	~PostProcess();

	void Initialize(ID3D11Device* device);

	void SetMaterial(Material* material);
	Material* GetMaterial()const;

	RenderTexture* GetTexture()const;

	void Render(ID3D11DeviceContext* deviceContext, bool useRenderTarget);

private:
	FullScreenQuad quad;
	Material* p_Material;
	RenderTexture* renderTexture;
};

NS_END

#endif