#ifndef BASERENDERER_HPP
#define BASERENDERER_HPP

#include <Core\Common.hpp>
#include <Objects\Component.hpp>

NS_BEGIN

class BaseRenderer : public Component
{
public:
	BaseRenderer();
	~BaseRenderer();

	virtual void Initialize() = 0;
	virtual void Destroy() = 0;

#if DX11
	virtual void Render(ID3D11DeviceContext* deviceContext) = 0;
#elif DX12
	virtual void Render(ID3D12GraphicsCommandList* commandList) = 0;
#elif GL43
	virtual void Render() = 0;
#endif
private:
	
};

NS_END

#endif