#ifndef PHYSICS_OBJECT_HPP
#define PHYSICS_OBJECT_HPP

#include <Core\Common.hpp>
#include <Objects\Component.hpp>

#include <PhysX\PxPhysicsAPI.h>

NS_BEGIN

class PhysicsObject : public Component
{
public:
	PhysicsObject();
	~PhysicsObject();

	bool OnAddToGameObject(GameObject* obj);

private:
	physx::PxMaterial* physicsMaterial;
	physx::PxRigidActor* rigidbody;
	physx::PxShape* collider;
};

NS_END

#endif