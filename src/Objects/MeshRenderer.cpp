#include <Objects\MeshRenderer.hpp>
#include <Core\WorldManager.hpp>
#include <Objects\GameObject.hpp>
#include <Objects\Transform.hpp>
#include <Utility\ResourceManager.hpp>
#include <Objects\Camera.hpp>
#include <Core\Renderer.hpp>

NS_BEGIN

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material):
	BaseRenderer(material),
	p_Mesh(mesh)
{}

MeshRenderer::~MeshRenderer()
{}

void MeshRenderer::OnEnable()
{
	Renderer::GetInstance()->AddRenderableGameObject(this);
}

void MeshRenderer::OnDisable()
{
	Renderer::GetInstance()->RemoveRenderableGameObject(this);
}

void MeshRenderer::Render(ID3D11DeviceContext* deviceContext)
{
	ID3D11Device* device;
	deviceContext->GetDevice(&device);

	p_Material->SetShaderVariable<Matrix>("world", p_GameObject->GetComponent<Transform>()->GetWorldMatrix().Transpose(), ShaderType::Vertex);
	p_Material->SetShaderVariable<Matrix>("view", Renderer::GetInstance()->GetActiveCamera()->GetView().Transpose(), ShaderType::Vertex);
	p_Material->SetShaderVariable<Matrix>("projection", Renderer::GetInstance()->GetActiveCamera()->GetProj().Transpose(), ShaderType::Vertex);
	p_Material->SetShaderVariable<Vector3>("eyePos", Renderer::GetInstance()->GetActiveCamera()->GetGameObject()->GetComponent<Transform>()->GetWorldPosition(), ShaderType::Vertex);

	p_Material->BindMaterial(deviceContext);

	ID3D11Buffer* vb = p_Mesh->GetVertexBuffer();
	deviceContext->IASetVertexBuffers(0, 1, &vb, p_Mesh->GetStride(), p_Mesh->GetOffset());
	deviceContext->IASetIndexBuffer(p_Mesh->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);
	deviceContext->DrawIndexed(p_Mesh->GetNumberOfIndices(), 0, 0);
}

bool MeshRenderer::OnAddToGameObject(GameObject* object)
{
	Component::OnAddToGameObject(object);
	Renderer::GetInstance()->AddRenderableGameObject(this);
	return true;
}


NS_END