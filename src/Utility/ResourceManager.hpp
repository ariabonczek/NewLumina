#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <Core\Common.hpp>
#include <Utility\Resources\Resource.hpp>
#include <unordered_map>

NS_BEGIN

enum class PrimitiveType
{
	Cube,
	Sphere,
	Plane,
	Cone,
	Cylinder,
	Tube,
	Torus
};

typedef std::unordered_map<LGUID, Resource*> Resourcemap;

class ResourceManager
{
	friend class GraphicsDevice;
public:
	~ResourceManager();

	static void Initialize();
	static void Shutdown();

#if DX11
	static Texture2D* LoadTexture2D(char* filepath);
	static Cubemap*   LoadCubemap(char** filepaths);
	static Mesh* LoadMesh(char* filepath);
	static Shader* LoadShader(wchar_t* filepath, ShaderType type);

	static RenderTexture* CreateRenderTexture(uint32 width = 256, uint32 height = 256, bool hasDepth = false);

	static Mesh* CreateFullScreenQuad();
	static Mesh* CreatePlane(float width = 5.0f, float depth = 5.0f, uint32 n = 10, uint32 m = 10);
	static Mesh* CreateSphere(float radius = 1.0f, uint32 numSubdivisions = 3);
	static Mesh* CreateCube(float size = 1.0f);
	static Mesh* CreateCylinder(float radius = 0.5f, float height = 1.0, uint32 axisDivisions = 20, uint32 heightDivisions = 3);
	static Mesh* CreateCone(float radius = 0.5f, float height = 1.0f, uint32 axisDivisions = 20, uint32 heightDivisions = 3);
	static Mesh* CreateTube(float outerRadius = 1.0f, float innerRadius = 0.6f, float height = 1.0f, uint32 axisSubdivisions = 20, uint32 heightDivisions = 3);
	static Mesh* CreateTorus(float radius = 1.0f, float sectionRadius = 0.5f, uint32 numSubdivisions = 20);
#elif DX12
	Resource* LoadTexture2D(char* filepath, ID3D12Device* device);
	Resource* LoadMesh(char* filepath, ID3D12Device* device);
	Resource* LoadShader(char* filepath, ID3D12Device* device);
#elif GL43
	Resource* LoadTexture2D(char* filepath);
	Resource* LoadMesh(char* filepath);
	Resource* LoadShader(char* filepath);
#endif

	static void FreeResource(Resource* resource);
private:
#if DX11
	static ID3D11Device* p_Device;
	static void SetDevice(ID3D11Device* device);
#elif DX12
	ID3D12Device* p_Device;
	void SetDevice(ID3D12Device* device);
#elif GL43

#endif
	ResourceManager();

	static void FreeAllResources();

	static Resourcemap resourceMap;
};

NS_END

#endif