#ifndef SHADER_HPP
#define SHADER_HPP

#include <Core\Common.hpp>
#include <Utility\Resources\Resource.hpp>
#include <unordered_map>

NS_BEGIN

struct ShaderVariable
{
	uint32 offset;
	uint32 size;
	uint32 index;
};

struct ConstantBuffer
{
	uint32 index;
	ID3D11Buffer* buffer;
	unsigned char* dataBuffer;
};

class Shader : public Resource
{
	friend class ResourceManager;
public:
	Shader(LGUID guid);
	~Shader();

	bool LoadShaderFromFile(wchar_t* filepath, ID3D11Device* device);
	virtual void CreateShader(ID3D11Device* device) = 0;

	virtual void Destroy();

	virtual void BindShader(ID3D11DeviceContext* deviceContext);

	template<typename T>
	void SetData(const char* name, T data)
	{
		ShaderVariable* variable = GetShaderVariable(name, sizeof(T));
		if (!variable)
			return;
		memcpy(constantBuffers[variable->index].dataBuffer + variable->offset, &data, sizeof(T));
		dirty = true;
	}

	template<typename T>
	void SetData(const char* name, T data, uint32 index)
	{
		ShaderVariable* variable = GetShaderVariable(name, sizeof(T));
		if (!variable)
			return;
		memcpy(constantBuffers[variable->index].dataBuffer + variable->offset + index * sizeof(T), &data, sizeof(T));
		dirty = true;
	}

	void SetDataSize(const char* name, unsigned char* data, uint32 size)
	{
		ShaderVariable* variable = GetShaderVariable(name, size);
		if (!variable)
			return;
		memcpy(constantBuffers[variable->index].dataBuffer + variable->offset, data, size);
		dirty = true;
	}

	virtual void SetShaderResourceView(const char* name, ID3D11ShaderResourceView* srv, ID3D11DeviceContext* deviceContext) = 0;
	virtual void SetSamplerState(const char* name, ID3D11SamplerState* sampler, ID3D11DeviceContext* deviceContext) = 0;

protected:
	bool dirty;
	ID3DBlob* shaderBlob;
	ConstantBuffer* constantBuffers;
	uint32 numConstantBuffers;

	std::unordered_map<LGUID, ConstantBuffer*> constantBuffersMap;
	std::unordered_map<LGUID, ShaderVariable> variables;
	std::unordered_map<LGUID, uint32> textures;
	std::unordered_map<LGUID, uint32> samplers;

	void UpdateResources(ID3D11DeviceContext* deviceContext);

	ShaderVariable* GetShaderVariable(const char* name, uint32 size);
	ConstantBuffer* GetConstantBuffer(const char* name);
	int32 GetTextureIndex(const char* name);
	int32 GetSamplerIndex(const char* name);

	/// <summary>
	/// Used only by the ResourceManager to set the GUID after filesystem creates the allocated shaders
	/// </summary>
	void SetLGUID(LGUID guid);
};

class VertexShader : public Shader
{
public:
	VertexShader(LGUID guid);
	~VertexShader();

	void CreateShader(ID3D11Device* device);

	void Destroy();

	void BindShader(ID3D11DeviceContext* deviceContext);
	void SetShaderResourceView(const char* name, ID3D11ShaderResourceView* srv, ID3D11DeviceContext* deviceContext);
	void SetSamplerState(const char* name, ID3D11SamplerState* sampler, ID3D11DeviceContext* deviceContext);
private:
	ID3D11VertexShader* vertexShader;
	ID3D11InputLayout* inputLayout;
};

// TODO: Finish Hull Shader integration
//class HullShader : public Shader
//{
//	HullShader(LGUID guid);
//	~HullShader();
//
//	void Destroy();
//
//	bool SetShaderResourceView(const char* name, ID3D11ShaderResourceView* srv);
//	bool SetSamplerState(const char* name, ID3D11SamplerState* sampler);
//private:
//	ID3D11HullShader* vertexShader;
//	virtual bool CreateShader(ID3DBlob* shaderBlob) = 0;
//};

// TODO: Finish Domain Shader integration
//class DomainShader : public Shader
//{
//	DomainShader(LGUID guid);
//	~DomainShader();
//
//	void Destroy();
//
//	bool SetShaderResourceView(const char* name, ID3D11ShaderResourceView* srv);
//	bool SetSamplerState(const char* name, ID3D11SamplerState* sampler);
//private:
//	ID3D11DomainShader* vertexShader;
//	ID3D11InputLayout* inputLayout;
//	virtual bool CreateShader(ID3DBlob* shaderBlob) = 0;
//};

class GeometryShader : public Shader
{
public:
	GeometryShader(LGUID guid, bool useStreamOut = false, bool rasterizeStreamOut = false);
	~GeometryShader();

	void CreateShader(ID3D11Device* device);
	void CreateStreamOutBuffer(ID3D11Buffer** buffer, uint32 vertexCount, ID3D11Device* device);

	static void UnbindStreamOut(ID3D11DeviceContext* deviceContext);

	void Destroy();

	void BindShader(ID3D11DeviceContext* deviceContext);
	void SetShaderResourceView(const char* name, ID3D11ShaderResourceView* srv, ID3D11DeviceContext* deviceContext);
	void SetSamplerState(const char* name, ID3D11SamplerState* sampler, ID3D11DeviceContext* deviceContext);
private:
	ID3D11GeometryShader* geometryShader;

	uint32 streamOutVertexSize;
	uint32 CalculateComponentCount(uint32 mask);
	bool streamOut;
	bool rasterizeSO;
};

class PixelShader : public Shader
{
public:
	PixelShader(LGUID guid);
	~PixelShader();

	void CreateShader(ID3D11Device* device);

	void Destroy();

	void BindShader(ID3D11DeviceContext* deviceContext);
	void SetShaderResourceView(const char* name, ID3D11ShaderResourceView* srv, ID3D11DeviceContext* deviceContext);
	void SetSamplerState(const char* name, ID3D11SamplerState* sampler, ID3D11DeviceContext* deviceContext);
private:
	ID3D11PixelShader* pixelShader;

};

// TODO: Finish Compute Shader integration
// May need to build this one from scratch

//class ComputeShader : public Shader
//{
//	VertexShader(LGUID guid);
//	~VertexShader();
//
//	void Destroy();
//
//	bool SetShaderResourceView(const char* name, ID3D11ShaderResourceView* srv);
//	bool SetSamplerState(const char* name, ID3D11SamplerState* sampler);
//private:
//	ID3D11VertexShader* vertexShader;
//	ID3D11InputLayout* inputLayout;
//	virtual bool CreateShader(ID3DBlob* shaderBlob) = 0;
//};


NS_END

#endif