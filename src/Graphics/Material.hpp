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

struct VariableInformation
{
	ShaderType type;
	const char* variableName;
	unsigned char* data;
	uint32 size;
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
		LGUID guid;
		std::unordered_map<LGUID, VariableInformation>::iterator it;
		VariableInformation vi;
		switch (type)
		{
		case ShaderType::Vertex:
			guid = Hash(name);
			if ((it = variables.find(guid)) != variables.end())
			{
				if (it->second.type == ShaderType::Vertex)
				{
					memcpy(it->second.data, data, it->second.size);
				}
			}
			else
			{
				vi.type = type;
				vi.variableName = name;
				vi.data = new unsigned char[sizeof(T)];
				vi.size = sizeof(T);
				memcpy(vi.data, data, vi.size);
				variables[guid] = vi;
			}
			break;
		case ShaderType::Hull:

			break;
		case ShaderType::Domain:

			break;
		case ShaderType::Geometry:
			guid = Hash(name);
			if ((it = variables.find(guid)) != variables.end())
			{
				if (it->second.type == ShaderType::Geometry)
				{
					memcpy(it->second.data, data, it->second.size);
				}
			}
			else
			{
				vi.type = type;
				vi.variableName = name;
				vi.data = new unsigned char[sizeof(T)];
				vi.size = sizeof(T);
				memcpy(vi.data, data, vi.size);
				variables[guid] = vi;
			}			
			break;
		case ShaderType::GeometrySO:
			guid = Hash(name);
			if ((it = variables.find(guid)) != variables.end())
			{
				if (it->second.type == ShaderType::GeometrySO)
				{
					memcpy(it->second.data, data, it->second.size);
				}
			}
			else
			{
				vi.type = type;
				vi.variableName = name;
				vi.data = new unsigned char[sizeof(T)];
				vi.size = sizeof(T);
				memcpy(vi.data, data, vi.size);
				variables[guid] = vi;
			}
			break;
		case ShaderType::Pixel:
			guid = Hash(name);
			if ((it = variables.find(guid)) != variables.end())
			{
				if (it->second.type == ShaderType::Pixel)
				{
					memcpy(it->second.data, data, it->second.size);
				}
			}
			else
			{
				vi.type = type;
				vi.variableName = name;
				vi.data = new unsigned char[sizeof(T)];
				vi.size = sizeof(T);
				memcpy(vi.data, data, vi.size);
				variables[guid] = vi;
			}			break;
		}
	}

	/// <summary>
	///
	/// </summary>
	template<typename T>
	void SetShaderVariable(const char* name, T data, ShaderType type)
	{
		LGUID guid;
		std::unordered_map<LGUID, VariableInformation>::iterator it;
		VariableInformation vi;
		switch (type)
		{
		case ShaderType::Vertex:
			guid = Hash(name);
			if ((it = variables.find(guid)) != variables.end())
			{
				if (it->second.type == ShaderType::Vertex)
				{
					memcpy(it->second.data, &data, it->second.size);
				}
			}
			else
			{
				vi.type = type;
				vi.variableName = name;
				vi.data = new unsigned char[sizeof(T)];
				vi.size = sizeof(T);
				memcpy(vi.data, &data, vi.size);
				variables[guid] = vi;
			}
			break;
		case ShaderType::Hull:

			break;
		case ShaderType::Domain:

			break;
		case ShaderType::Geometry:
			guid = Hash(name);
			if ((it = variables.find(guid)) != variables.end())
			{
				if (it->second.type == ShaderType::Geometry)
				{
					memcpy(it->second.data, &data, it->second.size);
				}
			}
			else
			{
				vi.type = type;
				vi.variableName = name;
				vi.data = new unsigned char[sizeof(T)];
				vi.size = sizeof(T);
				memcpy(vi.data, &data, vi.size);
				variables[guid] = vi;
			}
			break;
		case ShaderType::GeometrySO:
			guid = Hash(name);
			if ((it = variables.find(guid)) != variables.end())
			{
				if (it->second.type == ShaderType::GeometrySO)
				{
					memcpy(it->second.data, &data, it->second.size);
				}
			}
			else
			{
				vi.type = type;
				vi.variableName = name;
				vi.data = new unsigned char[sizeof(T)];
				vi.size = sizeof(T);
				memcpy(vi.data, &data, vi.size);
				variables[guid] = vi;
			}
			break;
		case ShaderType::Pixel:
			guid = Hash(name);
			if ((it = variables.find(guid)) != variables.end())
			{
				if (it->second.type == ShaderType::Pixel)
				{
					memcpy(it->second.data, &data, it->second.size);
				}
			}
			else
			{
				vi.type = type;
				vi.variableName = name;
				vi.data = new unsigned char[sizeof(T)];
				vi.size = sizeof(T);
				memcpy(vi.data, &data, vi.size);
				variables[guid] = vi;
			}			
			break;
		}
	}
	
	/// <summary>
	/// Sets the variable at the give index in an array
	/// </summary>
	template<typename T>
	void SetShaderVariable(const char* name, T* data, uint32 index, ShaderType type)
	{
		switch (type)
		{
		case ShaderType::Vertex:
			p_VertexShader->SetData(name, data, index);
			break;
		case ShaderType::Hull:

			break;
		case ShaderType::Domain:

			break;
		case ShaderType::Geometry:
			p_GeometryShader->SetData(name, data, index);
			break;
		case ShaderType::GeometrySO:
			p_GeometryShader->SetData(name, data, index);
			break;
		case ShaderType::Pixel:
			p_PixelShader->SetData(name, data, index);
			break;
		}
	}

	/// <summary>
	///
	/// </summary>
	template<typename T>
	void SetShaderVariable(const char* name, T data, uint32 index, ShaderType type)
	{
		switch (type)
		{
		case ShaderType::Vertex:
			p_VertexShader->SetData(name, data, index);
			break;
		case ShaderType::Hull:

			break;
		case ShaderType::Domain:

			break;
		case ShaderType::Geometry:
			p_GeometryShader->SetData(name, data, index);
			break;
		case ShaderType::GeometrySO:
			p_GeometryShader->SetData(name, data, index);
			break;
		case ShaderType::Pixel:
			p_PixelShader->SetData(name, data, index);
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
	std::unordered_map<LGUID, TextureInformation> textures;
	std::unordered_map<LGUID, VariableInformation> variables;

	VertexShader* p_VertexShader;
	//HullShader hShader;
	//DomainShader dShader;
	GeometryShader* p_GeometryShader;
	PixelShader* p_PixelShader;
};

NS_END

#endif