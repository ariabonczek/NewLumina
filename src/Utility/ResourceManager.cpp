#include <Utility\ResourceManager.hpp>

#include <Config.hpp>
#include <Utility\Filesystem.hpp>
#include <Utility\Murmur3\MurmurHash.h>

#include <Graphics\Texture2D.hpp>
#include <Graphics\Mesh.hpp>
#include <Graphics\Shader.hpp>

NS_BEGIN

ID3D11Device* ResourceManager::p_Device;
Resourcemap ResourceManager::resourceMap;

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

#if DX11
Texture2D* ResourceManager::LoadTexture2D(char* filepath, ID3D11Device* device)
#elif DX12
Resource* ResourceManager::LoadTexture2D(char* filepath, ID3D12Device* device)
#elif GL43

#endif
{
	LGUID guid = Hash(filepath);

	if (resourceMap.find(guid) != resourceMap.end())
	{
		return static_cast<Texture2D*>(resourceMap[guid]);
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
Mesh* ResourceManager::LoadMesh(char* filepath, ID3D11Device* device)
#elif DX12
Resource* ResourceManager::LoadMesh(char* filepath, ID3D12Device* device)

#elif GL43

#endif
{
	LGUID guid = Hash(filepath);

	if (resourceMap.find(guid) != resourceMap.end())
	{
		return static_cast<Mesh*>(resourceMap[guid]);
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
Shader* ResourceManager::LoadShader(wchar_t* filepath, ShaderType type, ID3D11Device* device)
#elif DX12
Resource* ResourceManager::LoadShader(char* filepath, ID3D12Device* device)

#elif GL43

#endif
{
	LGUID guid = Hash(reinterpret_cast<const char*>(filepath));

	if (resourceMap.find(guid) != resourceMap.end())
	{
		return static_cast<Shader*>(resourceMap[guid]);
	}

	Shader* s = Filesystem::LoadShader(filepath, type, device);
	s->SetLGUID(guid);
	//resourceMap[guid] = s;
	return s;
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
	LGUID guid = resource->GetLGUID();
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