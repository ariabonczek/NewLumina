//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#ifndef WORLD_MANAGER_HPP
#define WORLD_MANAGER_HPP

#include <Core\Common.hpp>

NS_BEGIN

class GameObject;
class BaseRenderer;
class PhysicsObject;

/// <summary>
/// Manages the world and what is in it
/// </summary>
class WorldManager
{
	friend class Scheduler;
public:
	~WorldManager();

	static WorldManager* GetInstance();

	/// <summary>
	/// Initializes the WorldManager and its subsystems
	/// </summary>
	void Initialize();

	/// <summary>
	/// Shuts the WorldManager and its subsystems down
	/// </summary>
	void Shutdown();

#if DX11 || DX12
	/// <summary>
	///
	/// </summary>
	static DWORD WINAPI Update(void* param);

	/// <summary>
	///
	/// </summary>
	static DWORD WINAPI Physics(void* param);

	/// <summary>
	///
	/// </summary>   
	static DWORD WINAPI Render(void* param);
#elif GL43

#endif
	
	void AddActiveGameObject(GameObject* gameObject);
	void AddRenderableGameObject(BaseRenderer* renderer);
	void AddPhysicsObject(PhysicsObject* physicsObject);

	void RemoveInactiveGameObject(GameObject* gameObject);
	void RemoveRenderableGameObject(BaseRenderer* renderer);
	void RemovePhysicsObject(PhysicsObject* physicsObject);

	/// <summary>
	/// Returns true if the application should quit
	/// </summary>
	bool ShouldQuit();
private:
	WorldManager();

	ID3D11DeviceContext* p_DeviceContext;

	std::vector<GameObject*> activeObjects;
	std::vector<BaseRenderer*> renderableObjects;
	std::vector<PhysicsObject*> physicsObjects;

	bool shouldQuit;
};

NS_END


#endif