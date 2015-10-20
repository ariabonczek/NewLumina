#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <Core\Common.hpp>
#include <Objects\Component.hpp>
#include <unordered_map>
#include <type_traits>
#include <Core\WorldManager.hpp>

NS_BEGIN

class MeshRenderer;

/// <summary>
///
/// </summary>
class GameObject
{
public:
	GameObject(char* name);
	virtual ~GameObject();

	/// <summary>
	///
	/// </summary>
	virtual void Initialize();

	/// <summary>
	///
	/// </summary>
	virtual void Destroy();

	/// <summary>
	///
	/// </summary>
	virtual void Update();

	/// <summary>
	///
	/// </summary>
	void Enable();

	/// <summary>
	///
	/// </summary>
	void Disable();

	/// <summary>
	///
	/// </summary>
	template<class T>
	T* GetComponent()
	{
		uint32 hash = Hash(typeid(T).name());
		if(components.find(hash) != components.end())
			return reinterpret_cast<T*>(components[Hash(typeid(T).name())]);
		return nullptr;
	}

	/// <summary>
	///
	/// </summary>
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

	/// <summary>
	///
	/// </summary>
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