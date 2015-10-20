#include <Objects\Light.hpp>

#include <Core\Renderer.hpp>

NS_BEGIN

Light::Light(LightType type, Color color, float intensity):
type(type)
{
	data.color = color;
	data.intensity = intensity;
	data.range = 10.0f;
	data.direction = Vector3(0.0f, 0.0f, 1.0f);
}

Light::~Light()
{}

bool Light::OnAddToGameObject(GameObject* object)
{
	Component::OnAddToGameObject(object);
	Renderer::GetInstance()->AddLight(this);
	return true;
}

void Light::SetDirection(Vector3 direction)
{
	data.direction = Vector3::Normalize(direction);
}

NS_END