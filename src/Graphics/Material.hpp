#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <Core\Common.hpp>
#include <Graphics\Shader.hpp>
#include <Graphics\Texture2D.hpp>
#include <Graphics\Data.hpp>

NS_BEGIN

struct TextureInformation
{
	ShaderType type;
	Texture2D* texture;
	const char* textureName;
	const char* samplerName;
};

class Material
{
public:
	Material();
	~Material();

	/// <summary>
	///
	/// </summary>
	void SetTexture2D(const char* textureName, const char* samplerName, Texture2D* tex, ShaderType type);
	//void SetRenderTexture(GraphicsShaderType type, RenderTexture* rt);

	/// <summary>
	///
	/// </summary>
	template<typename T>
	void SetShaderVariable(const char* name, T* data, ShaderType type)
	{
		switch (type)
		{
		case ShaderType::Vertex:
			p_VertexShader->SetData(name, data);
			break;
		case ShaderType::Hull:

			break;
		case ShaderType::Domain:

			break;
		case ShaderType::Geometry:
			p_GeometryShader->SetData(name, data);
			break;
		case ShaderType::GeometrySO:
			p_GeometryShader->SetData(name, data);
			break;
		case ShaderType::Pixel:
			p_PixelShader->SetData(name, data);
			break;
		}
	}

	/// <summary>
	///
	/// </summary>
	void SetVertexShader(VertexShader* shader);
	//void SetHullShader(HullShader* shader);
	//void SetDomainShader(DomainShader* shader);

	/// <summary>
	///
	/// </summary>
	void SetGeometryShader(GeometryShader* shader);

	/// <summary>
	///
	/// </summary>
	void SetPixelShader(PixelShader* shader);

	/// <summary>
	///
	/// </summary>
	VertexShader* GetVertexShader()const;

	/// <summary>
	///
	/// </summary>
	GeometryShader* GetGeometryShader()const;

	/// <summary>
	///
	/// </summary>
	PixelShader* GetPixelShader()const;

	/// <summary>
	///
	/// </summary>
	void BindMaterial(ID3D11DeviceContext* devCon);
private:
	// TODO: moar generic
	std::unordered_map<LGUID, TextureInformation> textures;
	VertexShader* p_VertexShader;
	//HullShader hShader;
	//DomainShader dShader;
	GeometryShader* p_GeometryShader;
	PixelShader* p_PixelShader;
};

NS_END

#endif