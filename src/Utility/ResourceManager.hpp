#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <Core\Common.hpp>
#include <Utility\Resources\Resource.hpp>
#include <unordered_map>

NS_BEGIN


typedef std::unordered_map<LGUID, Resource*> Resourcemap;

class ResourceManager
{
	friend class GraphicsDevice;
public:
	~ResourceManager();
	static ResourceManager GetInstance();

	void Initialize();
	void Shutdown();

#if DX11
	Resource* LoadTexture2D(char* filepath, ID3D11Device* device);
	Resource* LoadMesh(char* filepath, ID3D11Device* device);
	Resource* LoadShader(char* filepath, ShaderType type, ID3D11Device* device);
	Resource* LoadMaterial(char* filepath, ID3D11Device* device);
#elif DX12
	Resource* LoadTexture2D(char* filepath, ID3D12Device* device);
	Resource* LoadMesh(char* filepath, ID3D12Device* device);
	Resource* LoadShader(char* filepath, ID3D12Device* device);
	Resource* LoadMaterial(char* filepath, ID3D12Device* device);
#elif GL43
	Resource* LoadTexture2D(char* filepath);
	Resource* LoadMesh(char* filepath);
	Resource* LoadShader(char* filepath);
	Resource* LoadMaterial(char* filepath);
#endif

	void FreeResource(Resource* resource);
private:
#if DX11
	ID3D11Device* p_Device;
	void SetDevice(ID3D11Device* device);
#elif DX12
	ID3D12Device* p_Device;
	void SetDevice(ID3D12Device* device);
#elif GL43

#endif
	ResourceManager();

	void FreeAllResources();

	Resourcemap resourceMap;
};

NS_END

#endif