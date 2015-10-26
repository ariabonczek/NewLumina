#ifndef DEPTH_STENCIL_STATE_HPP
#define DEPTH_STENCIL_STATE_HPP

#include <Core\Common.hpp>

NS_BEGIN

class DepthStencilState
{
public:
	DepthStencilState(ID3D11Device* device, bool depthEnable = true, bool stencilEnable = false);
	~DepthStencilState();

	static DepthStencilState* Default;
	static DepthStencilState* NoDepth;
	static DepthStencilState* StencilPass;
	static DepthStencilState* Deferred;

	static void InitializeStates(ID3D11Device* device);
	static void DestroyStates();

	ID3D11DepthStencilState* GetState()const;
private:
	ID3D11DepthStencilState* state;
};

NS_END

#endif