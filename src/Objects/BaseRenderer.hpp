#ifndef BASERENDERER_HPP
#define BASERENDERER_HPP

#include <Core\Common.hpp>
#include <Objects\Component.hpp>

NS_BEGIN

class BaseRenderer : public Component
{
public:
	BaseRenderer(Material* material = nullptr);
	~BaseRenderer();

#if DX11
	virtual void Render(ID3D11DeviceContext* deviceContext) = 0;
#elif DX12
	virtual void Render(ID3D12GraphicsCommandList* commandList) = 0;
#elif GL43
	virtual void Render() = 0;
#endif

	void SetMaterial(Material* material);

	Material* GetMaterial()const;
protected:
	Material* p_Material;
};

NS_END

#endif