#include <Objects\Light.hpp>

#include <Core\Renderer.hpp>
#include <Objects\GameObject.hpp>
#include <Objects\Transform.hpp>

NS_BEGIN

Light::Light(LightType type, Color color, float intensity):
type(type), hasShadows(false), shadowTexture(nullptr)
{
	data.color = color;
	data.intensity = intensity;
	data.range = 10.0f;
	data.direction = Vector3(0.0f, 0.0f, 1.0f);
	data.spot = 45.0f;
}

Light::~Light()
{}

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

NS_END