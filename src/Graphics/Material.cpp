#include <Graphics\Material.hpp>

NS_BEGIN

Material::Material()
{}

Material::~Material()
{}

void Material::SetTexture(const char* textureName, const char* samplerName, Texture* tex, ShaderType type)
{
	TextureInformation ti;
	ti.srv = tex->GetShaderResourceView();
	ti.sampler = tex->GetSampler()->GetSamplerState();
	ti.textureName = textureName;
	ti.samplerName = samplerName;
	ti.type = type;

	textures[Hash(textureName)] = ti;
}

void Material::SetTextureEX(const char* textureName, const char* samplerName, ID3D11ShaderResourceView* srv, ID3D11SamplerState* sampler, ShaderType type)
{
	TextureInformation ti;
	ti.srv = srv;
	ti.sampler = sampler;
	ti.textureName = textureName;
	ti.samplerName = samplerName;
	ti.type = type;

	textures[Hash(textureName)] = ti;
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
	for (std::unordered_map<LGUID, TextureInformation>::iterator it = textures.begin(); it != textures.end(); ++it)
	{
		switch (it->second.type)
		{
		case ShaderType::Vertex:
			if (p_VertexShader)
			{
				p_VertexShader->SetShaderResourceView(it->second.textureName, it->second.srv, deviceContext);
				p_VertexShader->SetSamplerState(it->second.samplerName, it->second.sampler, deviceContext);
			}
			break;
		case ShaderType::Geometry:
			if (p_GeometryShader)
			{
				p_GeometryShader->SetShaderResourceView(it->second.textureName, it->second.srv, deviceContext);
				p_GeometryShader->SetSamplerState(it->second.samplerName, it->second.sampler, deviceContext);
			}
			break;
		case ShaderType::GeometrySO:
			if (p_GeometryShader)
			{
				p_GeometryShader->SetShaderResourceView(it->second.textureName, it->second.srv, deviceContext);
				p_GeometryShader->SetSamplerState(it->second.samplerName, it->second.sampler, deviceContext);
			}
			break;
		case ShaderType::Pixel:
			if (p_PixelShader)
			{
				p_PixelShader->SetShaderResourceView(it->second.textureName, it->second.srv, deviceContext);
				p_PixelShader->SetSamplerState(it->second.samplerName, it->second.sampler, deviceContext);
			}
			break;
		}
	}

	for (std::unordered_map<LGUID, VariableInformation>::iterator it = variables.begin(); it != variables.end(); ++it)
	{
		switch (it->second.type)
		{
		case ShaderType::Vertex:
			if (p_VertexShader)
			{
				p_VertexShader->SetDataSize(it->second.variableName, it->second.data, it->second.size);
			}
			break;
		case ShaderType::Geometry:
			if (p_GeometryShader)
			{
				p_GeometryShader->SetDataSize(it->second.variableName, it->second.data, it->second.size);
			}
			break;
		case ShaderType::GeometrySO:
			if (p_GeometryShader)
			{
				p_GeometryShader->SetDataSize(it->second.variableName, it->second.data, it->second.size);
			}
			break;
		case ShaderType::Pixel:
			if (p_PixelShader)
			{
				p_PixelShader->SetDataSize(it->second.variableName, it->second.data, it->second.size);
			}
			break;
		}
	}
	if (p_VertexShader)
		p_VertexShader->BindShader(deviceContext);
	if (p_GeometryShader)
		p_GeometryShader->BindShader(deviceContext);
	if (p_PixelShader)
		p_PixelShader->BindShader(deviceContext);
}


NS_END