#include <Graphics\Mesh.hpp>

NS_BEGIN

#if DX11
Mesh::Mesh(MeshData& meshData, LGUID guid, ID3D11Device* device):
	Resource(guid), stride(sizeof(MeshVertex)), offset(0)
{
	MeshVertex* vertices = &meshData.vertices[0];
	uint16* indices = &meshData.indices[0];
	numIndices = meshData.indices.size();

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

	D3D11_SUBRESOURCE_DATA srd;
	ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));

	// Vertex Buffer
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(MeshVertex) * meshData.vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	srd.pSysMem = vertices;
	device->CreateBuffer(&bd, &srd, &vertexBuffer);

	// Index Buffer
	bd.ByteWidth = sizeof(uint16) * numIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	srd.pSysMem = indices;
	device->CreateBuffer(&bd, &srd, &indexBuffer);
}

ID3D11Buffer* Mesh::GetVertexBuffer()const
{
	return vertexBuffer;
}

ID3D11Buffer* Mesh::GetIndexBuffer()const
{
	return indexBuffer;
}

#elif DX12
Mesh::Mesh(MeshData& meshData, LGUID guid, ID3D12Device* device) :
	Resource(guid)
{
	MeshVertex* vertices = &meshData.vertices[0];
	uint16* indices = &meshData.indices[0];

	uint32 vBufferSize = sizeof(MeshVertex) * meshData.vertices.size();
	uint32 iBufferSize = sizeof(uint16) * meshData.indices.size();

	// Vertex Buffer
	D3D12_HEAP_PROPERTIES hp;
	ZeroMemory(&hp, sizeof(D3D12_HEAP_PROPERTIES));
	hp.Type = D3D12_HEAP_TYPE_DEFAULT;
	hp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	hp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	hp.CreationNodeMask = 1;
	hp.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC bd;
	ZeroMemory(&bd, sizeof(D3D12_RESOURCE_DESC));
	bd.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bd.Alignment = 0;
	bd.Width = vBufferSize;
	bd.Height = 1;
	bd.DepthOrArraySize = 1;
	bd.MipLevels = 1;
	bd.Format = DXGI_FORMAT_UNKNOWN;
	bd.SampleDesc.Count = 1;
	bd.SampleDesc.Quality = 0;
	bd.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	bd.Flags = D3D12_RESOURCE_FLAG_NONE;

	device->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &bd, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, __uuidof(ID3D12Resource), (void**)vertexBuffer);
	
	bd.Width = iBufferSize;

	device->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &bd, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, __uuidof(ID3D12Resource), (void**)vertexBuffer);

	uint8* vertDataBegin;
	vertexBuffer->Map(0, nullptr, (void**)&vertDataBegin);
	memcpy(vertDataBegin, vertices, vBufferSize);
	vertexBuffer->Unmap(0, nullptr);

	uint8* indDataBegin;
	indexBuffer->Map(0, nullptr, (void**)&indDataBegin);
	memcpy(indDataBegin, indices, iBufferSize);
	indexBuffer->Unmap(0, nullptr);
}

ID3D12Resource* Mesh::GetVertexBuffer()const
{
	return vertexBuffer;
}

ID3D12Resource* Mesh::GetIndexBuffer()const
{
	return indexBuffer;
}

#elif GL43

#endif

uint32 Mesh::GetNumberOfIndices()const
{
	return numIndices;
}

const uint32* Mesh::GetOffset()const
{
	return &offset;
}

const uint32* Mesh::GetStride()const
{
	return &stride;
}

Mesh::~Mesh()
{
#if DX11
	DELETECOM(vertexBuffer);
	DELETECOM(indexBuffer);
#elif DX12
	DELETECOM(vertexBuffer);
	DELETECOM(indexBuffer);
#elif GL43

#endif
}

NS_END