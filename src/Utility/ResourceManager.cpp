#include <Utility\ResourceManager.hpp>

#include <Config.hpp>
#include <Utility\Filesystem.hpp>
#include <Utility\Murmur3\MurmurHash.h>

#include <Graphics\Texture2D.hpp>
#include <Graphics\RenderTexture.hpp>
#include <Graphics\Mesh.hpp>
#include <Graphics\Shader.hpp>
#include <sstream>
#include <Utility\MeshBuilder.hpp>

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
Texture2D* ResourceManager::LoadTexture2D(char* filepath)
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
	Texture2D* tex = new Texture2D(i, guid, p_Device);
#elif GL43
	Texture2D* tex = new Texture2D(i, guid);
#endif

	resourceMap[guid] = tex;
	return tex;
}

#if DX11
Mesh* ResourceManager::LoadMesh(char* filepath)
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
	Mesh* mesh = new Mesh(m, guid, p_Device);
#elif GL43

#endif

	resourceMap[guid] = mesh;
	return mesh;
}
#if DX11
Shader* ResourceManager::LoadShader(wchar_t* filepath, ShaderType type)
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

	Shader* s = Filesystem::LoadShader(filepath, type, p_Device);
	s->SetLGUID(guid);
	//resourceMap[guid] = s;
	return s;
}

RenderTexture* ResourceManager::CreateRenderTexture(uint32 width, uint32 height)
{
	LGUID guid = Hash(Timer::GetTimeSinceEpoch());

	if (resourceMap.find(guid) != resourceMap.end())
	{
		return static_cast<RenderTexture*>(resourceMap[guid]);
	}

	RenderTexture* r = new RenderTexture(width, height, guid, p_Device);
	resourceMap[guid] = r;
	return r;
}

Mesh* ResourceManager::CreatePlane(float width, float depth, uint32 n, uint32 m)
{
	std::ostringstream str;
	str << "Plane" << width << depth << n << m;

	LGUID guid = Hash(str.str().c_str());

	if (resourceMap.find(guid) != resourceMap.end())
	{
		return static_cast<Mesh*>(resourceMap[guid]);
	}
	
	Mesh* mesh = new Mesh(MeshBuilder::CreatePlane(width, depth, n, m), guid, p_Device);

	resourceMap[guid] = mesh;
	return mesh;
}

Mesh* ResourceManager::CreateSphere(float radius, uint32 numSubdivisions)
{
	std::ostringstream str;
	str << "Sphere" << radius << numSubdivisions;

	LGUID guid = Hash(str.str().c_str());

	if (resourceMap.find(guid) != resourceMap.end())
	{
		return static_cast<Mesh*>(resourceMap[guid]);
	}

	Mesh* mesh = new Mesh(MeshBuilder::CreateSphere(radius, numSubdivisions), guid, p_Device);

	resourceMap[guid] = mesh;
	return mesh;
}

Mesh* ResourceManager::CreateCube(float size)
{
	std::ostringstream str;
	str << "Cube" << size;

	LGUID guid = Hash(str.str().c_str());

	if (resourceMap.find(guid) != resourceMap.end())
	{
		return static_cast<Mesh*>(resourceMap[guid]);
	}

	Mesh* mesh = new Mesh(MeshBuilder::CreateCube(size), guid, p_Device);

	resourceMap[guid] = mesh;
	return mesh;	
}

Mesh* ResourceManager::CreateCylinder(float radius, float height, uint32 axisDivisions, uint32 heightDivisions)
{
	std::ostringstream str;
	str << "Cylinder" << radius << height << axisDivisions << heightDivisions;

	LGUID guid = Hash(str.str().c_str());

	if (resourceMap.find(guid) != resourceMap.end())
	{
		return static_cast<Mesh*>(resourceMap[guid]);
	}

	Mesh* mesh = new Mesh(MeshBuilder::CreateCylinder(radius, height, axisDivisions, heightDivisions), guid, p_Device);

	resourceMap[guid] = mesh;
	return mesh;
}

Mesh* ResourceManager::CreateCone(float radius, float height, uint32 axisDivisions, uint32 heightDivisions)
{
	std::ostringstream str;
	str << "Cone" << radius << height << axisDivisions << heightDivisions;

	LGUID guid = Hash(str.str().c_str());

	if (resourceMap.find(guid) != resourceMap.end())
	{
		return static_cast<Mesh*>(resourceMap[guid]);
	}

	Mesh* mesh = new Mesh(MeshBuilder::CreateCone(radius, height, axisDivisions, heightDivisions), guid, p_Device);

	resourceMap[guid] = mesh;
	return mesh;
}

Mesh* ResourceManager::CreateTube(float outerRadius, float innerRadius, float height, uint32 axisSubdivisions, uint32 heightDivisions)
{
	std::ostringstream str;
	str << "Tube" << outerRadius << innerRadius << height << axisSubdivisions << heightDivisions;

	LGUID guid = Hash(str.str().c_str());

	if (resourceMap.find(guid) != resourceMap.end())
	{
		return static_cast<Mesh*>(resourceMap[guid]);
	}

	Mesh* mesh = new Mesh(MeshBuilder::CreateTube(outerRadius, innerRadius, height, axisSubdivisions, heightDivisions), guid, p_Device);

	resourceMap[guid] = mesh;
	return mesh;
}

Mesh* ResourceManager::CreateTorus(float radius, float sectionRadius, uint32 numSubdivisions)
{
	std::ostringstream str;
	str << "Torus" << radius << sectionRadius << numSubdivisions;

	LGUID guid = Hash(str.str().c_str());

	if (resourceMap.find(guid) != resourceMap.end())
	{
		return static_cast<Mesh*>(resourceMap[guid]);
	}

	Mesh* mesh = new Mesh(MeshBuilder::CreateTorus(radius, sectionRadius, numSubdivisions), guid, p_Device);

	resourceMap[guid] = mesh;
	return mesh;
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