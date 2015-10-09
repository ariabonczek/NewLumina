#include <Objects\MeshRenderer.hpp>
#include <Core\WorldManager.hpp>

NS_BEGIN

MeshRenderer::MeshRenderer()
{}

MeshRenderer::~MeshRenderer()
{}

void MeshRenderer::Initialize()
{

}

void MeshRenderer::Render(ID3D11DeviceContext* deviceContext)
{
	ID3D11Buffer* vb = p_Mesh->GetVertexBuffer();
	deviceContext->IASetVertexBuffers(0, 1, &vb, p_Mesh->GetStride(), p_Mesh->GetOffset());
	deviceContext->IASetIndexBuffer(p_Mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->DrawIndexed(p_Mesh->GetNumberOfIndices(), 0, 0);
}

void MeshRenderer::Destroy()
{

}

bool MeshRenderer::OnAddToGameObject(GameObject* object)
{
	WorldManager::GetInstance()->AddRenderableGameObject(this);
	return true;
}


NS_END