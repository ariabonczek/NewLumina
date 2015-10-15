#include <Objects\Component.hpp>
#include <Objects\GameObject.hpp>

NS_BEGIN

Component::Component()
{
	guid = Hash(Timer::GetTimeSinceEpoch());
}

Component::~Component()
{}

bool Component::OnAddToGameObject(GameObject* gameObject)
{
	if (p_GameObject)
	{
#if _DEBUG
		Debug::LogError("[Component] Tried to add an instance of a component to more than one GameObject.");
#endif
		return false;
	}
	p_GameObject = gameObject;
	return true;

}

LGUID Component::GetLGUID()const
{
	return guid;
}

NS_END