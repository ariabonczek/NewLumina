#include <Objects\MeshRenderer.hpp>
#include <Core\WorldManager.hpp>
#include <Objects\GameObject.hpp>
#include <Objects\Transform.hpp>
#include <Objects\Camera.hpp>

NS_BEGIN

MeshRenderer::MeshRenderer(Mesh* mesh):
	p_Mesh(mesh)
{}

MeshRenderer::~MeshRenderer()
{}

void MeshRenderer::Initialize()
{

}

void MeshRenderer::Render(ID3D11DeviceContext* deviceContext)
{
	p_Material->BindMaterial(deviceContext);
	Matrix m = WorldManager::GetInstance()->activeCamera->GetProj();

	p_Material->GetVertexShader()->SetData<Matrix>("world", &p_GameObject->GetComponent<Transform>()->GetWorldMatrix().Transpose());
	p_Material->GetVertexShader()->SetData<Matrix>("view", &WorldManager::GetInstance()->activeCamera->GetView().Transpose());
	p_Material->GetVertexShader()->SetData<Matrix>("projection", &WorldManager::GetInstance()->activeCamera->GetProj().Transpose());

	ID3D11Buffer* vb = p_Mesh->GetVertexBuffer();
	deviceContext->IASetVertexBuffers(0, 1, &vb, p_Mesh->GetStride(), p_Mesh->GetOffset());
	deviceContext->IASetIndexBuffer(p_Mesh->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);
	deviceContext->DrawIndexed(p_Mesh->GetNumberOfIndices(), 0, 0);
}

void MeshRenderer::Destroy()
{

}

bool MeshRenderer::OnAddToGameObject(GameObject* object)
{
	Component::OnAddToGameObject(object);
	WorldManager::GetInstance()->AddRenderableGameObject(this);
	return true;
}

void MeshRenderer::SetMaterial(Material* material)
{
	p_Material = material;
}
NS_END