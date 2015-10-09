#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <Core\Common.hpp>

NS_BEGIN

class GameObject;

class Component
{
public:
	Component();
	virtual ~Component();

	virtual void Initialize() = 0;
	virtual void Destroy() = 0;

	virtual bool OnAddToGameObject(GameObject* gameObject);
protected:
	GameObject* p_GameObject;
};

NS_END

#endif