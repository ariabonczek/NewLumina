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
	ID3D11ShaderResourceView* srv;
	ID3D11SamplerState* sampler;
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
	void SetTexture(const char* textureName, const char* samplerName, Texture* tex, ShaderType type);
	void SetTextureEX(const char* textureName, const char* samplerName, ID3D11ShaderResourceView* srv, ID3D11SamplerState* sampler, ShaderType type);

	/// <summary>
	///
	/// </summary>
	template<typename T>
	void SetShaderVariable(const char* name, T data, ShaderType type)
	{
		LGUID guid;
		std::unordered_map<LGUID, VariableInformation>::iterator it;
		VariableInformation vi;

		guid = Hash(name);
		if ((it = variables.find(guid)) != variables.end())
		{
			if (it->second.type == type)
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
	}

	/// <summary>
	///
	/// </summary>
	template<typename T>
	void SetShaderVariable(const char* name, T data, uint32 index, ShaderType type)
	{
		LGUID guid;
		std::unordered_map<LGUID, VariableInformation>::iterator it;
		VariableInformation vi;

		guid = Hash(name);
		if ((it = variables.find(guid)) != variables.end())
		{
			if (it->second.type == type)
			{
				memcpy(it->second.data + index * sizeof(T), &data + index * sizeof(T), it->second.size);
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