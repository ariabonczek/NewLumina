#include <Objects\Light.hpp>

#include <Core\Renderer.hpp>
#include <Objects\GameObject.hpp>
#include <Objects\Transform.hpp>
#include <Graphics\Mesh.hpp>
#include <Utility\ResourceManager.hpp>

NS_BEGIN

Light::Light(LightType type, Color color, float intensity):
type(type), hasShadows(false), shadowTexture(nullptr)
{
	data.intensity = intensity;
	data.color = color;
	data.range = 10.0f;
	data.spot = 45.0f;
}

Light::~Light()
{}

void Light::Initialize()
{
	switch (type)
	{
	case LightType::Directional:
		SetMesh(ResourceManager::CreateFullScreenQuad());
		break;
	case LightType::Point:
		SetMesh(ResourceManager::CreateSphere(1.0f, 2));
		break;
	case LightType::Spot:
		//SetMesh(ResourceManager::CreateCone(data.spot);
		break;
	}
}

void Light::Update()
{
	data.position = p_GameObject->GetComponent<Transform>()->GetWorldPosition();
}

bool Light::OnAddToGameObject(GameObject* object)
{
	Component::OnAddToGameObject(object);
	Renderer::GetInstance()->AddLight(this);
	return true;
}

void Light::SetShadowTexture(RenderTexture* texture)
{
	shadowTexture = texture;

	hasShadows = texture == nullptr;	
}

void Light::SetMesh(Mesh* mesh)
{
	p_DeferredMesh = mesh;
}

void Light::RenderLightAsGeometry(ID3D11DeviceContext* deviceContext)
{
	ID3D11Buffer* vb = p_DeferredMesh->GetVertexBuffer();
	deviceContext->IASetVertexBuffers(0, 1, &vb, p_DeferredMesh->GetStride(), p_DeferredMesh->GetOffset());
	deviceContext->IASetIndexBuffer(p_DeferredMesh->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);
	deviceContext->DrawIndexed(p_DeferredMesh->GetNumberOfIndices(), 0, 0);
}

NS_END