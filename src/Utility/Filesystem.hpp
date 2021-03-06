#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <Core\Common.hpp>
#include <Graphics\Data.hpp>
#include <assimp\scene.h>

NS_BEGIN

class Resource;

class Filesystem
{
public:
	
	static void Initialize();
	static void Shutdown();
	
	static Image LoadTexture2D(char* filepath);
	static MeshData LoadMesh(char* filepath);
	static Resource* LoadShader(char* filepath);
	static Resource* LoadMaterial(char* filepath);
private:
	Filesystem();
	~Filesystem();

	static MeshData AssimpProcessScene(aiNode* node, const aiScene* scene);
	static MeshData AssimpProcessMesh(aiMesh* mesh, const aiScene* scene);
};

NS_END

#endif