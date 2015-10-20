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

	virtual void Initialize();
	virtual void Destroy();

	virtual void Update();

	virtual bool OnAddToGameObject(GameObject* gameObject);
	virtual void OnEnable();
	virtual void OnDisable();

	LGUID GetLGUID()const;
protected:
	GameObject* p_GameObject;
	LGUID guid;
};

NS_END

#endif