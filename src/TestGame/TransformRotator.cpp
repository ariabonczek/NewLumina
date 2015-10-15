#include <TestGame\TransformRotator.hpp>

TransformRotator::TransformRotator(Vector3 axis, float speed):
	axis(axis), speed(speed)
{}

TransformRotator::~TransformRotator()
{}

void TransformRotator::Initialize()
{
	p_Transform = p_GameObject->GetComponent<Transform>();
}

void TransformRotator::Destroy()
{}

void TransformRotator::Update()
{
	p_Transform->Rotate(Quaternion::CreateFromAxisAngle(axis, speed * Timer::GetFrameTime()));
}

bool TransformRotator::OnAddToGameObject(GameObject* object)
{
	Component::OnAddToGameObject(object);
	return true;
}