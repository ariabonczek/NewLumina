#include <Objects\Component.hpp>
#include <Objects\GameObject.hpp>

NS_BEGIN

Component::Component()
{}

Component::~Component()
{}

bool Component::OnAddToGameObject(GameObject* gameObject)
{
	if (gameObject)
	{
#if _DEBUG
		Debug::LogError("[Component] Tried to add an instance of a component to more than one GameObject.");
#endif
		return false;
	}
	p_GameObject = gameObject;
	return true;
}

NS_END