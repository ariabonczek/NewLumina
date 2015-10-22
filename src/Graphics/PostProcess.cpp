#include <Graphics\PostProcess.hpp>
#include <Utility\ResourceManager.hpp>

NS_BEGIN

PostProcess::PostProcess()
{
	
}

PostProcess::~PostProcess()
{
	delete p_Material;
}

void PostProcess::Initialize(ID3D11Device* device)
{
	p_Material = new Material();
	p_Material->SetVertexShader((VertexShader*)ResourceManager::LoadShader(L"Shaders/DirectX/fullScreenQuadVertex.cso", ShaderType::Vertex));
	p_Material->SetPixelShader((PixelShader*)ResourceManager::LoadShader(L"Shaders/DirectX/copyTexture.cso", ShaderType::Pixel));

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
	p_Material->BindMaterial(deviceContext);
	quad.Render(deviceContext);
}

NS_END