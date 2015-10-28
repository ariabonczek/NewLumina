#include <Objects\PhysicsObject.hpp>

NS_BEGIN

PhysicsObject::PhysicsObject()
{}

PhysicsObject::~PhysicsObject()
{}

bool PhysicsObject::OnAddToGameObject(GameObject* obj)
{
	Component::OnAddToGameObject(obj);
	return true;
}

NS_END