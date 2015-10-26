#include <Graphics\DepthStencilState.hpp>

NS_BEGIN

DepthStencilState* DepthStencilState::Default;
DepthStencilState* DepthStencilState::NoDepth;
DepthStencilState* DepthStencilState::StencilPass;
DepthStencilState* DepthStencilState::Deferred;

DepthStencilState::DepthStencilState(ID3D11Device* device, bool depthEnable, bool stencilEnable)
{
	D3D11_DEPTH_STENCIL_DESC dsd;
	dsd.DepthEnable = depthEnable;
	dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	dsd.StencilEnable = stencilEnable;
	dsd.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
	dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	dsd.BackFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
	dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	dsd.StencilReadMask = 0xff;

	if (depthEnable && stencilEnable)
	{
		dsd.DepthFunc = D3D11_COMPARISON_ALWAYS;
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_DECR;
		dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_INCR;
		dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		dsd.StencilReadMask = 0x0;
		dsd.StencilWriteMask = 0x0;
	}

	if (!depthEnable && stencilEnable)
	{
		dsd.DepthEnable = D3D11_COMPARISON_NOT_EQUAL;
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		dsd.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_DECR;
		dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		dsd.BackFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_INCR;
		dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		dsd.StencilReadMask = 0xff;
		dsd.StencilWriteMask = 0xff;
	}

	HRESULT hr = device->CreateDepthStencilState(&dsd, &state);
}

DepthStencilState::~DepthStencilState()
{}

void DepthStencilState::InitializeStates(ID3D11Device* device)
{
	Default = new DepthStencilState(device, true, false);
	NoDepth = new DepthStencilState(device, false, false);
	StencilPass = new DepthStencilState(device, true, true);
	Deferred = new DepthStencilState(device, false, true);
}

void DepthStencilState::DestroyStates()
{
	delete Default;
	delete NoDepth;
	delete Deferred;
	delete StencilPass;
}

ID3D11DepthStencilState* DepthStencilState::GetState()const
{
	return state;
}

NS_END