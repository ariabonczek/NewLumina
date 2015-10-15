#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <Core\Common.hpp>
#include <Objects\Component.hpp>
#include <unordered_map>
#include <type_traits>
#include <Core\WorldManager.hpp>

NS_BEGIN

class MeshRenderer;

class GameObject
{
public:
	GameObject(char* name);
	virtual ~GameObject();

	virtual void Initialize();
	virtual void Destroy();

	virtual void Update();

	void Enable();
	void Disable();

	template<class T>
	T* GetComponent()
	{
		if(components.find(Hash(typeid(T).name())) != components.end())
			return reinterpret_cast<T*>(components[Hash(typeid(T).name())]);
		return nullptr;
	}

	template<class T>
	void AddComponent(Component* component)
	{
		LGUID guid = Hash(typeid(T).name());
		if (components.find(guid) != components.end())
		{
#if _DEBUG
			Debug::LogError("[GameObject] Attempted to add a new instance of an already existing component.");
#endif
			return;
		}
 
		component->OnAddToGameObject(this);
		components[guid] = component;
	}

	LGUID GetLGUID()const;
	// TODO: Template specialization for physics objects
	
protected:
	LGUID guid;
	char* name;
	bool enabled;
	std::unordered_map<LGUID, Component*> components;
};

NS_END

#endif GAMEOBJECT_HPP