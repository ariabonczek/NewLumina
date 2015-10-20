#ifndef TRANSFORM_ROTATOR_HPP
#define TRANSFORM_ROTATOR_HPP

#include <Objects\Component.hpp>
#include <Objects\Transform.hpp>
#include <Objects\GameObject.hpp>
#include <Math\Math.hpp>

using namespace Lumina;

class TransformRotator : public Component
{
public:
	TransformRotator(Vector3 axis, float speed);
	~TransformRotator();

	void Initialize();

	void Update();
	bool OnAddToGameObject(GameObject* object);
private:
	Transform* p_Transform;
	Vector3 axis;
	float speed;
};

#endif