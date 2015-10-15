#include <Objects\GameObject.hpp>
#include <Core\WorldManager.hpp>
#include <Objects\Transform.hpp>

#include <Objects\BaseRenderer.hpp>

NS_BEGIN

GameObject::GameObject(char* name) :
	name(name),
	enabled(false)
{
	components[Hash(typeid(Transform).name())] = new Transform();
}

GameObject::~GameObject()
{}

void GameObject::Initialize()
{
	guid = Hash(Timer::GetTimeSinceEpoch());
#if _DEBUG
	Debug::Log("[GameObject] created with guid: " + std::to_string(guid));
#endif

	for (std::unordered_map<LGUID, Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		it->second->Initialize();
	}

	Enable();
}

void GameObject::Destroy()
{
	components.clear();
}

void GameObject::Update(float dt)
{

}

void GameObject::Enable()
{
	if (enabled)
		return;

	WorldManager::GetInstance()->AddActiveGameObject(this);
	if (BaseRenderer* r = dynamic_cast<BaseRenderer*>(GetComponent<BaseRenderer>()))
	{
		WorldManager::GetInstance()->AddRenderableGameObject(r);
	}
	// TODO: PhysicsObjects

	enabled = true;
}

void GameObject::Disable()
{
	if (!enabled)
		return;

	WorldManager::GetInstance()->RemoveInactiveGameObject(this);
	if (BaseRenderer* r = dynamic_cast<BaseRenderer*>(GetComponent<BaseRenderer>()))
	{
		WorldManager::GetInstance()->RemoveRenderableGameObject(r);
	}
}


NS_END