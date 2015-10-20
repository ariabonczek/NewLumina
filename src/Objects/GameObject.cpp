#include <Objects\GameObject.hpp>
#include <Core\WorldManager.hpp>
#include <Objects\Transform.hpp>
#include <Core\Renderer.hpp>
#include <Objects\BaseRenderer.hpp>

NS_BEGIN

GameObject::GameObject(char* name) :
	name(name),
	enabled(false)
{
	LGUID t = Hash(typeid(Transform).name());
	components[t] = new Transform();
	GetComponent<Transform>()->OnAddToGameObject(this);
	guid = Hash(Timer::GetTimeSinceEpoch());
}

GameObject::~GameObject()
{}

void GameObject::Initialize()
{
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
	for (std::unordered_map<LGUID, Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		it->second->OnDisable();
	}
	components.clear();
}

void GameObject::Update()
{
	for (std::unordered_map<LGUID, Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		it->second->Update();
	}
}

void GameObject::Enable()
{
	if (enabled)
		return;

	WorldManager::GetInstance()->AddActiveGameObject(this);
	for (std::unordered_map<LGUID, Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		it->second->OnEnable();
	}

	enabled = true;
}

void GameObject::Disable()
{
	if (!enabled)
		return;

	WorldManager::GetInstance()->RemoveInactiveGameObject(this);
	for (std::unordered_map<LGUID, Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		it->second->OnDisable();
	}

	enabled = false;
}

LGUID GameObject::GetLGUID()const
{
	return guid;
}

NS_END