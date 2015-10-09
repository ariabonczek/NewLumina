#include <Graphics\Shader.hpp>
#include <d3dcompiler.h>

NS_BEGIN

Shader::Shader(LGUID guid):
	Resource(guid)
{}

Shader::~Shader()
{}

bool Shader::LoadShaderFromFile(const char* filepath, ID3D11Device* device)
{
	// Load the shader
	shaderBlob = nullptr;
	D3DReadFileToBlob(reinterpret_cast<LPCWSTR>(filepath), &shaderBlob);

	// Shader reflection object
	ID3D11ShaderReflection* reflection;
	D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), __uuidof(ID3D11ShaderReflection), (void**)&reflection);

	// Get the shader description
	D3D11_SHADER_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_SHADER_DESC));
	reflection->GetDesc(&sd);

	// Create the constant buffers
	numConstantBuffers = sd.ConstantBuffers;
	constantBuffers = new ConstantBuffer[numConstantBuffers];

	// Foreach resource...
	uint32 resourceCount = sd.BoundResources;
	for (uint32 i = 0; i < resourceCount; ++i)
	{
		// Grab the shader inputs (textures, samplers, etc)
		D3D11_SHADER_INPUT_BIND_DESC rd;
		ZeroMemory(&rd, sizeof(D3D11_SHADER_INPUT_BIND_DESC));
		reflection->GetResourceBindingDesc(i, &rd);

		switch (rd.Type)
		{
		case D3D_SIT_TEXTURE:
			textures.insert(std::pair<const char*, uint32>(rd.Name, rd.BindPoint));
			break;
		case D3D_SIT_SAMPLER:
			samplers.insert(std::pair<const char*, uint32>(rd.Name, rd.BindPoint));
			break;
		}
	}

	// Foreach constant buffer...
	for (uint32 i = 0; i < numConstantBuffers; ++i)
	{
		// Grab the buffer
		ID3D11ShaderReflectionConstantBuffer* cb = reflection->GetConstantBufferByIndex(i);

		// And its description
		D3D11_SHADER_BUFFER_DESC sbd;
		ZeroMemory(&sbd, sizeof(D3D11_SHADER_BUFFER_DESC));
		cb->GetDesc(&sbd);

		// Grab its binding
		D3D11_SHADER_INPUT_BIND_DESC sibd;
		ZeroMemory(&sibd, sizeof(D3D11_SHADER_INPUT_BIND_DESC));
		reflection->GetResourceBindingDescByName(sibd.Name, &sibd);

		constantBuffers[i].index = sibd.BindPoint;
		constantBuffersMap.insert(std::pair<const char*, ConstantBuffer*>(sibd.Name, &constantBuffers[i]));

		// Create the D3D object
		D3D11_BUFFER_DESC bfd;
		ZeroMemory(&bfd, sizeof(D3D11_BUFFER_DESC));
		bfd.Usage = D3D11_USAGE_DEFAULT;
		bfd.ByteWidth = sbd.Size;
		bfd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bfd.CPUAccessFlags = 0;
		bfd.MiscFlags = 0;
		bfd.StructureByteStride = 0;
		device->CreateBuffer(&bfd, 0, &constantBuffers[i].buffer);

		constantBuffers[i].dataBuffer = new uint8[sbd.Size];
		ZeroMemory(constantBuffers[i].dataBuffer, sbd.Size);

		// Foreach variable in the buffer
		for (uint32 j = 0; j < sbd.Variables; ++j)
		{
			// Grab it
			ID3D11ShaderReflectionVariable* var = cb->GetVariableByIndex(j);

			D3D11_SHADER_VARIABLE_DESC svd;
			ZeroMemory(&svd, sizeof(D3D11_SHADER_VARIABLE_DESC));
			var->GetDesc(&svd);

			// Store it
			ShaderVariable variable;
			variable.index = j;
			variable.offset = svd.StartOffset;
			variable.size = svd.Size;

			variables.insert(std::pair<const char*, ShaderVariable>(svd.Name, variable));
		}
	}

	reflection->Release();
	return true;
}

void Shader::Destroy()
{
	for (uint32 i = 0; i < numConstantBuffers; ++i)
	{
		constantBuffers[i].buffer->Release();
		delete[] constantBuffers[i].dataBuffer;
	}
	delete[] constantBuffers;
	numConstantBuffers = 0;

	constantBuffersMap.clear();
	variables.clear();
	textures.clear();
	samplers.clear();
}

void Shader::BindShader(ID3D11DeviceContext* deviceContext)
{
	if(dirty)
		UpdateResources(deviceContext);
}

void Shader::UpdateResources(ID3D11DeviceContext* deviceContext)
{
	for (uint32 i = 0; i < numConstantBuffers; ++i)
	{
		deviceContext->UpdateSubresource(constantBuffers[i].buffer, 0, 0, constantBuffers[i].dataBuffer, 0, 0);
	}
	dirty = false;
}

ShaderVariable* Shader::GetShaderVariable(const char* name, uint32 size)
{
	std::unordered_map<const char*, ShaderVariable>::iterator it = variables.find(name);

	if (it == variables.end())
	{
		return nullptr;
	}

	ShaderVariable* variable = &(it->second);
	if (variable->size != size)
	{
		return nullptr;
	}

	return variable;
}

ConstantBuffer* Shader::GetConstantBuffer(const char* name)
{
	std::unordered_map<const char*, ConstantBuffer*>::iterator it = constantBuffersMap.find(name);

	if (it == constantBuffersMap.end())
	{
		return nullptr;
	}

	return it->second;
}

uint32 Shader::GetTextureIndex(const char* name)
{
	std::unordered_map<const char*, uint32>::iterator it = textures.find(name);

	if (it == textures.end())
	{
		return -1;
	}

	return it->second;
}

uint32 Shader::GetSamplerIndex(const char* name)
{
	std::unordered_map<const char*, uint32>::iterator it = samplers.find(name);

	if (it == samplers.end())
	{
		return -1;
	}

	return it->second;
}

void Shader::SetLGUID(LGUID guid)
{
	this->guid = guid;
}

VertexShader::VertexShader(LGUID guid):
	Shader(guid)
{

}

VertexShader::~VertexShader()
{
	Destroy();
}

GeometryShader::GeometryShader(LGUID guid, bool streamOut, bool rasterizeSO) :
	Shader(guid), streamOut(streamOut), rasterizeSO(rasterizeSO)
{

}

GeometryShader::~GeometryShader()
{
	Destroy();
}

PixelShader::PixelShader(LGUID guid) :
	Shader(guid)
{

}

PixelShader::~PixelShader()
{
	Destroy();
}

void VertexShader::Destroy()
{
	Shader::Destroy();
	DELETECOM(vertexShader);
	DELETECOM(inputLayout);
}
void GeometryShader::Destroy()
{
	Shader::Destroy();
	DELETECOM(geometryShader);
}
void PixelShader::Destroy()
{
	Shader::Destroy();
	DELETECOM(pixelShader);
}

void VertexShader::SetShaderResourceView(const char* name, ID3D11ShaderResourceView* srv, ID3D11DeviceContext* deviceContext)
{
	uint32 index = GetTextureIndex(name);

	deviceContext->VSSetShaderResources(index, 1, &srv);
}

void VertexShader::SetSamplerState(const char* name, ID3D11SamplerState* sampler, ID3D11DeviceContext* deviceContext)
{
	uint32 index = GetSamplerIndex(name);

	deviceContext->VSSetSamplers(index, 1, &sampler);
}

void VertexShader::BindShader(ID3D11DeviceContext* deviceContext)
{
	Shader::BindShader(deviceContext);
	deviceContext->VSSetShader(vertexShader, 0, 0);

	for (uint32 i = 0; i < numConstantBuffers; ++i)
	{
		deviceContext->VSSetConstantBuffers(
			constantBuffers[i].index,
			1,
			&constantBuffers[i].buffer);
	}
}

void GeometryShader::SetShaderResourceView(const char* name, ID3D11ShaderResourceView* srv, ID3D11DeviceContext* deviceContext)
{
	uint32 index = GetTextureIndex(name);

	deviceContext->GSSetShaderResources(index, 1, &srv);
}

void GeometryShader::SetSamplerState(const char* name, ID3D11SamplerState* sampler, ID3D11DeviceContext* deviceContext)
{
	uint32 index = GetSamplerIndex(name);

	deviceContext->GSSetSamplers(index, 1, &sampler);
}

void GeometryShader::BindShader(ID3D11DeviceContext* deviceContext)
{
	Shader::BindShader(deviceContext);
	deviceContext->GSSetShader(geometryShader, 0, 0);

	for (uint32 i = 0; i < numConstantBuffers; ++i)
	{
		deviceContext->GSSetConstantBuffers(
			constantBuffers[i].index,
			1,
			&constantBuffers[i].buffer);
	}
}

void PixelShader::SetShaderResourceView(const char* name, ID3D11ShaderResourceView* srv, ID3D11DeviceContext* deviceContext)
{
	uint32 index = GetTextureIndex(name);

	deviceContext->PSSetShaderResources(index, 1, &srv);
}

void PixelShader::SetSamplerState(const char* name, ID3D11SamplerState* sampler, ID3D11DeviceContext* deviceContext)
{
	uint32 index = GetSamplerIndex(name);

	deviceContext->PSSetSamplers(index, 1, &sampler);
}

void PixelShader::BindShader(ID3D11DeviceContext* deviceContext)
{
	Shader::BindShader(deviceContext);
	deviceContext->PSSetShader(pixelShader, 0, 0);

	for (uint32 i = 0; i < numConstantBuffers; ++i)
	{
		deviceContext->PSSetConstantBuffers(
			constantBuffers[i].index,
			1,
			&constantBuffers[i].buffer);
	}
}

void VertexShader::CreateShader(ID3D11Device* device)
{
	device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &vertexShader);
	shaderBlob->Release();

}

void GeometryShader::CreateShader(ID3D11Device* device)
{
	if (streamOut)
	{
		ID3D11ShaderReflection* reflection;
		D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), _uuidof(ID3D11ShaderReflection), (void**)&reflection);

		D3D11_SHADER_DESC sd;
		ZeroMemory(&sd, sizeof(D3D11_SHADER_DESC));
		reflection->GetDesc(&sd);

		streamOutVertexSize = 0;
		std::vector<D3D11_SO_DECLARATION_ENTRY> soDecl;
		for (uint32 i = 0; i < sd.OutputParameters; ++i)
		{
			D3D11_SIGNATURE_PARAMETER_DESC spd;
			ZeroMemory(&spd, sizeof(D3D11_SIGNATURE_PARAMETER_DESC));
			reflection->GetOutputParameterDesc(i, &spd);

			D3D11_SO_DECLARATION_ENTRY e;
			ZeroMemory(&e, sizeof(D3D11_SO_DECLARATION_ENTRY));
			e.SemanticIndex = spd.SemanticIndex;
			e.SemanticName = spd.SemanticName;
			e.Stream = spd.Stream;
			e.StartComponent = 0;
			e.OutputSlot = 0;
			e.ComponentCount = CalculateComponentCount(spd.Mask);

			streamOutVertexSize += e.ComponentCount * sizeof(float);
			soDecl.push_back(e);
		}

		uint32 rasterize = rasterizeSO ? 0 : D3D11_SO_NO_RASTERIZED_STREAM;

		device->CreateGeometryShaderWithStreamOutput(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &soDecl[0],
			soDecl.size(), NULL, 0, rasterize, NULL, &geometryShader);
	}
	else
	{
		device->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &geometryShader);
	}
	shaderBlob->Release();
}

void GeometryShader::CreateStreamOutBuffer(ID3D11Buffer** buffer, uint32 vertices, ID3D11Device* device)
{
	if (!streamOut || streamOutVertexSize == 0)
	{
		return;
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.BindFlags = D3D11_BIND_STREAM_OUTPUT | D3D11_BIND_VERTEX_BUFFER;
	bd.ByteWidth = streamOutVertexSize * vertices;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	bd.Usage = D3D11_USAGE_DEFAULT;

	device->CreateBuffer(&bd, 0, buffer);
}

uint32 GeometryShader::CalculateComponentCount(uint32 mask)
{
	uint32 result = 0;
	result += (uint32)((mask & 1) == 1);
	result += (uint32)((mask & 2) == 2);
	result += (uint32)((mask & 4) == 4);
	result += (uint32)((mask & 8) == 8);
	return result;
}

void GeometryShader::UnbindStreamOut(ID3D11DeviceContext* deviceContext)
{
	uint32 offset = 0;
	ID3D11Buffer* dummy[1] = { 0 };
	deviceContext->SOSetTargets(1, dummy, &offset);
}

void PixelShader::CreateShader(ID3D11Device* device)
{
	device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &pixelShader);
	shaderBlob->Release();
}

NS_END