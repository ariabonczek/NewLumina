#include <Graphics\RasterizerState.hpp>

NS_BEGIN

RasterizerState* RasterizerState::FrontSolid;
RasterizerState* RasterizerState::FrontWireframe;
RasterizerState* RasterizerState::BackSolid;
RasterizerState* RasterizerState::BackWireframe;
RasterizerState* RasterizerState::BothSolid;
RasterizerState* RasterizerState::BothWireframe;

RasterizerState::RasterizerState(FaceMode faceMode, FillMode fillMode, ID3D11Device* device)
{
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
	rd.FillMode = (D3D11_FILL_MODE)fillMode;
	rd.CullMode = (D3D11_CULL_MODE)faceMode;
	rd.DepthClipEnable = true;

	device->CreateRasterizerState(&rd, &state);
}

RasterizerState::~RasterizerState()
{}

void RasterizerState::InitializeStates(ID3D11Device* device)
{
	FrontSolid = new RasterizerState(FaceMode::Front, FillMode::Solid, device);
	FrontWireframe = new RasterizerState(FaceMode::Front, FillMode::Wireframe, device);
	BackSolid = new RasterizerState(FaceMode::Back, FillMode::Solid, device);
	BackWireframe = new RasterizerState(FaceMode::Back, FillMode::Wireframe, device);
	BothSolid = new RasterizerState(FaceMode::Both, FillMode::Solid, device);
	BothWireframe = new RasterizerState(FaceMode::Both, FillMode::Wireframe, device);
}

void RasterizerState::DestroyStates()
{
	delete FrontSolid;
	delete FrontWireframe;
	delete BackSolid;
	delete BackWireframe;
	delete BothSolid;
	delete BothWireframe;
}

ID3D11RasterizerState* RasterizerState::GetState()const
{
	return state;
}

NS_END