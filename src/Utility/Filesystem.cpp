#include <Utility\Filesystem.hpp>

#include <Utility\Resources\Resource.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <Utility\stb_image.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>

NS_BEGIN

void Filesystem::Initialize()
{

}

void Filesystem::Shutdown()
{

}

Image Filesystem::LoadTexture2D(char* filepath)
{
	Image image;
	int width, height, components;
	image.data = stbi_load(filepath, &width, &height, &components, 0);

	if (!image.data)
	{
#if _DEBUG
		Debug::LogError("[Filesystem] Failed to load image.");
#endif
	}
	image.width = width;
	image.height = height;
	image.components = components;

	return image;
}

MeshData Filesystem::LoadMesh(char* filepath)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filepath, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
	
	if (!scene)
	{
#if _DEBUG
		Debug::LogError("[ResourceManager] Assimp importer could not read mesh file: " + std::string(filepath));
#endif
	}
	
	return AssimpProcessScene(scene->mRootNode, scene);
}

Resource* Filesystem::LoadShader(char* filepath)
{
	return 0;
}

Resource* Filesystem::LoadMaterial(char* filepath)
{
	return 0;
}

MeshData Filesystem::AssimpProcessScene(aiNode* node, const aiScene* scene)
{
	MeshData data;

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];
		data.Append(AssimpProcessMesh(mesh, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		AssimpProcessScene(node->mChildren[i], scene);
	}

	return data;
}

MeshData Filesystem::AssimpProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	MeshData data;

	data.vertices.resize(mesh->mNumVertices);

	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		MeshVertex temp;

		// Vertices
		memcpy(&temp.position, &mesh->mVertices[i], sizeof(float) * 3);
		
		// Normals
		memcpy(&temp.normal, &mesh->mNormals[i], sizeof(float) * 3);

		// Tangents
		if (mesh->mTangents)
		{
			memcpy(&temp.tangent, &mesh->mTangents, sizeof(float) * 3);
		}
		else
		{
			memset(&temp.tangent, 0, sizeof(float) * 3);
		}
		// UVs
		if (mesh->mTextureCoords[0])
		{
			memcpy(&temp.texCoord, &mesh->mTextureCoords, sizeof(float) * 2);
		}
		else
		{
			memset(&temp.tangent, 0, sizeof(float) * 2);
		}

		data.vertices[i] = temp;
	}

	data.indices.resize(mesh->mNumFaces * 3);

	for (uint16 i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (uint16 j = 0; j < face.mNumIndices; j++)
		{
			data.indices[i * 3 + j] = face.mIndices[j];
		}
	}

	return data;
}

NS_END