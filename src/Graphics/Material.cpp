#include <Graphics\Material.hpp>

NS_BEGIN

Material::Material()
{}

Material::~Material()
{}

void Material::SetTexture2D(const char* textureName, const char* samplerName, Texture2D* tex, ShaderType type, ID3D11DeviceContext* deviceContext)
{
	switch (type)
	{
	case ShaderType::Vertex:
		p_VertexShader->SetShaderResourceView(textureName, tex->GetShaderResourceView(), deviceContext);
		p_VertexShader->SetSamplerState(samplerName, tex->GetSampler()->GetSamplerState(), deviceContext);
		break;
	case ShaderType::Hull:

		break;
	case ShaderType::Domain:

		break;
	case ShaderType::Geometry:
		p_GeometryShader->SetShaderResourceView(textureName, tex->GetShaderResourceView(), deviceContext);
		p_GeometryShader->SetSamplerState(samplerName, tex->GetSampler()->GetSamplerState(), deviceContext);
		break;
	case ShaderType::Pixel:
		p_PixelShader->SetShaderResourceView(textureName, tex->GetShaderResourceView(), deviceContext);
		p_PixelShader->SetSamplerState(samplerName, tex->GetSampler()->GetSamplerState(), deviceContext);
		break;
	}
}

void Material::SetVertexShader(VertexShader* shader)
{
	p_VertexShader = shader;
}

//void SetHullShader(HullShader* shader);
//void SetDomainShader(DomainShader* shader);
void Material::SetGeometryShader(GeometryShader* shader)
{
	p_GeometryShader = shader;
}

void Material::SetPixelShader(PixelShader* shader)
{
	p_PixelShader = shader;
}

VertexShader* Material::GetVertexShader()const
{
	return p_VertexShader;
}

GeometryShader* Material::GetGeometryShader()const
{
	return p_GeometryShader;
}

PixelShader* Material::GetPixelShader()const
{
	return p_PixelShader;
}

void Material::BindMaterial(ID3D11DeviceContext* deviceContext)
{
	if (p_VertexShader)
		p_VertexShader->BindShader(deviceContext);
	if (p_GeometryShader)
		p_GeometryShader->BindShader(deviceContext);
	if (p_PixelShader)
		p_PixelShader->BindShader(deviceContext);
}


NS_END