#include <Objects\MeshRenderer.hpp>
#include <Core\WorldManager.hpp>
#include <Objects\GameObject.hpp>
#include <Objects\Transform.hpp>
#include <Utility\ResourceManager.hpp>
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

void MeshRenderer::Update()
{

}

void MeshRenderer::Render(ID3D11DeviceContext* deviceContext)
{
	ID3D11Device* device;
	deviceContext->GetDevice(&device);

	p_Material->GetVertexShader()->SetData<Matrix>("world", &p_GameObject->GetComponent<Transform>()->GetWorldMatrix().Transpose());
	p_Material->GetVertexShader()->SetData<Matrix>("view", &WorldManager::GetInstance()->activeCamera->GetView().Transpose());
	p_Material->GetVertexShader()->SetData<Matrix>("projection", &WorldManager::GetInstance()->activeCamera->GetProj().Transpose());
	p_Material->SetTexture2D("_Albedo", "_Sampler", ResourceManager::LoadTexture2D("Textures/MaidOfTime.png", device), ShaderType::Pixel, deviceContext);
	//p_Material->SetTexture2D("_Normal", "_Sampler", ResourceManager::LoadTexture2D("Textures/brick_nor.jpg", device), ShaderType::Pixel, deviceContext);

	p_Material->BindMaterial(deviceContext);

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