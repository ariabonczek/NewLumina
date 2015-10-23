#include <Graphics\PostProcess.hpp>
#include <Utility\ResourceManager.hpp>
#include <Graphics\RenderTexture.hpp>

NS_BEGIN

PostProcess::PostProcess()
{
	
}

PostProcess::~PostProcess()
{
	delete renderTexture;
}

void PostProcess::Initialize(ID3D11Device* device)
{
	renderTexture = new RenderTexture(1280, 720, 0, device, true);
	quad.Initialize(device);
}

Material* PostProcess::GetMaterial()const
{
	return p_Material;
}

void PostProcess::SetMaterial(Material* material)
{
	p_Material = material;
}

void PostProcess::Render(ID3D11DeviceContext* deviceContext)
{
	//ID3D11RenderTargetView* rtv = renderTexture->GetRenderTargetView();
	//deviceContext->OMSetRenderTargets(1, &rtv, renderTexture->GetDepthStencilView());
	p_Material->BindMaterial(deviceContext);
	quad.Render(deviceContext);
}

NS_END