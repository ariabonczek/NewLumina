#include <Objects\GameObject.hpp>
#include <Core\WorldManager.hpp>

#include <Objects\BaseRenderer.hpp>

NS_BEGIN

GameObject::GameObject(char* name) :
	name(name),
	enabled(false)
{}

GameObject::~GameObject()
{}

void GameObject::Initialize()
{

}

void GameObject::Destroy()
{

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