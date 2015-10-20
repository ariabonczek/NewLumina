#include <Objects\Component.hpp>
#include <Objects\GameObject.hpp>

NS_BEGIN

Component::Component()
{
	guid = Hash(Timer::GetTimeSinceEpoch());
}

Component::~Component()
{}

void Component::Initialize()
{

}

void Component::Destroy()
{

}

void Component::Update()
{

}

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

void Component::OnEnable()
{

}

void Component::OnDisable()
{

}

LGUID Component::GetLGUID()const
{
	return guid;
}

NS_END