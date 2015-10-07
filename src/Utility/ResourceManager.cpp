#include <Utility\ResourceManager.hpp>

#include <Config.hpp>
#include <Utility\Filesystem.hpp>
#include <Utility\Murmur3\MurmurHash.h>

#include <Graphics\Texture2D.hpp>
#include <Graphics\Mesh.hpp>

NS_BEGIN

ResourceManager::ResourceManager()
{}

ResourceManager::~ResourceManager()
{}

void ResourceManager::Initialize()
{
#if _DEBUG
	Debug::Log("ResourceManager Initialized");
#endif
}

void ResourceManager::Shutdown()
{
	FreeAllResources();
}

ResourceManager ResourceManager::GetInstance()
{
	static ResourceManager instance;
	return instance;
}

uint32 ResourceManager::Hash(char* key)
{
	uint32 hash;
	MurmurHash3_x86_32(key, strlen(key), MURMUR3_SEED, &hash);
	return hash;
}

#if DX11
Resource* ResourceManager::LoadTexture2D(char* filepath, ID3D11Device* device)
#elif DX12
Resource* ResourceManager::LoadTexture2D(char* filepath, ID3D12Device* device)
#elif GL43

#endif
{
	GUID guid = Hash(filepath);

	if (resourceMap.find(guid) != resourceMap.end())
	{
		return resourceMap[guid];
	}

	Image i = Filesystem::LoadTexture2D(filepath);
	
#if DX11 || DX12
	Texture2D* tex = new Texture2D(i, guid, device);
#elif GL43
	Texture2D* tex = new Texture2D(i, guid);
#endif

	resourceMap[guid] = tex;
	return tex;
}

#if DX11
Resource* ResourceManager::LoadMesh(char* filepath, ID3D11Device* device)
#elif DX12
Resource* ResourceManager::LoadMesh(char* filepath, ID3D12Device* device)

#elif GL43

#endif
{
	GUID guid = Hash(filepath);

	if (resourceMap.find(guid) != resourceMap.end())
	{
		return resourceMap[guid];
	}

	MeshData m = Filesystem::LoadMesh(filepath);
#if DX11 || DX12
	Mesh* mesh = new Mesh(m, guid, device);
#elif GL43

#endif

	resourceMap[guid] = mesh;
	return mesh;
}
#if DX11
Resource* ResourceManager::LoadShader(char* filepath, ID3D11Device* device)
#elif DX12
Resource* ResourceManager::LoadShader(char* filepath, ID3D12Device* device)

#elif GL43

#endif
{
	GUID guid = Hash(filepath);

	if (resourceMap.find(guid) != resourceMap.end())
	{
		return resourceMap[guid];
	}

	return Filesystem::LoadShader(filepath);
}

#if DX11
Resource* ResourceManager::LoadMaterial(char* filepath, ID3D11Device* device)
#elif DX12
Resource* ResourceManager::LoadMaterial(char* filepath, ID3D12Device* device)
#elif GL43

#endif
{
	GUID guid = Hash(filepath);

	if (resourceMap.find(guid) != resourceMap.end())
	{
		return resourceMap[guid];
	}

	return Filesystem::LoadMaterial(filepath);
}

void ResourceManager::FreeAllResources()
{
	for (Resourcemap::iterator it = resourceMap.begin(); it != resourceMap.end(); ++it)
	{
#if _DEBUG
		Debug::Log("[ResourceManager] WARNING: Deleting all resources may not clean up all necessary memory!");
#endif
		delete it->second;
	}
	resourceMap.clear();
}

void ResourceManager::FreeResource(Resource* resource)
{
	GUID guid = resource->GetGUID();
	if (resourceMap.find(guid) != resourceMap.end())
	{
#if _DEBUG
		Debug::Log("[ResourceManager] WARNING: Deleting a resource may not clean up all necessary memory!");
#endif
		delete resourceMap[guid];
	}
}

#if DX11
void ResourceManager::SetDevice(ID3D11Device* device)
{
	p_Device = device;
}

#elif DX12
void ResourceManager::SetDevice(ID3D12Device* device)
{
	p_Device = device;
}
#elif GL43

#endif

NS_END