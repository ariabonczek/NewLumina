#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <Core\Common.hpp>
#include <Graphics\Shader.hpp>
#include <Graphics\Texture2D.hpp>
#include <Graphics\Data.hpp>

NS_BEGIN

class Material
{
public:
	Material();
	~Material();

	void SetTexture2D(const char* name, Texture2D* tex, ShaderType type, ID3D11DeviceContext* deviceContext);
	//void SetRenderTexture(GraphicsShaderType type, RenderTexture* rt);

	template<typename T>
	void SetShaderVariable(const char* name, T* data, ShaderType type)
	{
		switch (type)
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
	}

	void SetVertexShader(VertexShader* shader);
	//void SetHullShader(HullShader* shader);
	//void SetDomainShader(DomainShader* shader);
	void SetGeometryShader(GeometryShader* shader);
	void SetPixelShader(PixelShader* shader);

	void BindMaterial(ID3D11DeviceContext* devCon);
private:
	VertexShader* p_VertexShader;
	//HullShader hShader;
	//DomainShader dShader;
	GeometryShader* p_GeometryShader;
	PixelShader* p_PixelShader;
};

NS_END

#endif