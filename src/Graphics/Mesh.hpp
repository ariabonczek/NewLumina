#ifndef MESH_HPP
#define MESH_HPP

#include <Core\Common.hpp>

#include <Utility\Resources\Resource.hpp>

NS_BEGIN

class Mesh : public Resource
{
public:
#if DX11
	Mesh(MeshData& meshData, GUID guid, ID3D11Device* device);
#elif DX12
	Mesh(MeshData& meshData, GUID guid, ID3D12Device* device);
#elif GL43
	Mesh(MeshData& meshData, GUID guid);
#endif
	~Mesh();

#if DX11
	ID3D11Buffer* GetVertexBuffer()const;
	ID3D11Buffer* GetIndexBuffer()const;
#elif DX12
	ID3D12Resource* GetVertexBuffer()const;
	ID3D12Resource* GetIndexBuffer()const;
#elif GL43

#endif
private:
#if DX11
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
#elif DX12
	ID3D12Resource* vertexBuffer;
	ID3D12Resource* indexBuffer;
#elif GL43

#endif
};

NS_END


#endif