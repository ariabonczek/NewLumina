#include <Graphics\BlendState.hpp>

NS_BEGIN

BlendState* BlendState::None;
BlendState* BlendState::Transparency;
BlendState* BlendState::Additive;
BlendState* BlendState::Subtractive;

BlendState::BlendState(ID3D11Device* device, bool enableBlend, BlendType source, BlendType destination, BlendOp operation, BlendType sourceAlpha, BlendType destinationAlpha, BlendOp alphaOperation)
{
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));

	bd.AlphaToCoverageEnable = false;
	bd.IndependentBlendEnable = false;

	bd.RenderTarget[0].BlendEnable = enableBlend;
	bd.RenderTarget[0].SrcBlend = (D3D11_BLEND)source;
	bd.RenderTarget[0].DestBlend = (D3D11_BLEND)destination;
	bd.RenderTarget[0].BlendOp = (D3D11_BLEND_OP)operation;
	bd.RenderTarget[0].SrcBlendAlpha = (D3D11_BLEND)sourceAlpha;
	bd.RenderTarget[0].DestBlendAlpha = (D3D11_BLEND)destinationAlpha;
	bd.RenderTarget[0].BlendOpAlpha = (D3D11_BLEND_OP)alphaOperation;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	device->CreateBlendState(&bd, &state);
}

BlendState::~BlendState()
{}

void BlendState::InitializeStates(ID3D11Device* device)
{
	None = new BlendState(device, false);
	Transparency = new BlendState(device, true, BlendType::SrcAlpha, BlendType::InvSrcAlpha, BlendOp::Additive);
	Additive = new BlendState(device, true, BlendType::One, BlendType::One, BlendOp::Additive, BlendType::One, BlendType::One, BlendOp::Additive);
	Subtractive = new BlendState(device, true, BlendType::One, BlendType::One, BlendOp::Subtractive, BlendType::One, BlendType::One, BlendOp::Subtractive);
}

void BlendState::DestroyStates()
{
	delete None;
	delete Transparency;
	delete Additive;
	delete Subtractive;
}

ID3D11BlendState* BlendState::GetState()const
{
	return state;
}

NS_END