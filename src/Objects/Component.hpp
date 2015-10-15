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

	virtual void Update() = 0;

	virtual bool OnAddToGameObject(GameObject* gameObject);

	LGUID GetLGUID()const;
protected:
	GameObject* p_GameObject;
	LGUID guid;
};

NS_END

#endif