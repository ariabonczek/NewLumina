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

	virtual void Update(float dt);

	void Enable();
	void Disable();

	template<class T>
	T* GetComponent()
	{
		// TODO: Log the output of typeid(T).name() to make sure I'm using it write
		// Also try raw_name();
		return static_cast<T*>(components[Hash(typeid(T).name())]);
	}

	template<class T>
	void AddComponent(Component* component)
	{
		LGUID guid = Hash(typeid(T).name());
		if (components[guid])
		{
#if _DEBUG
			Debug::LogError("[GameObject] Attempted to add a new instance of an already existing component.");
#endif
			return;
		}
 
		component->OnAddToGameObject(this);
		components[guid] = component;
	}

	// TODO: Template specialization for physics objects
	
protected:
	LGUID guid;
	char* name;
	bool enabled;
	std::unordered_map<LGUID, Component*> components;
};

NS_END

#endif GAMEOBJECT_HPP