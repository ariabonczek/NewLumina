//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#ifndef WORLD_MANAGER_HPP
#define WORLD_MANAGER_HPP

#include <Core\Common.hpp>
#include <Core\PhysicsManager.hpp>
#include <Core\ObjectManager.hpp>

NS_BEGIN

/// <summary>
/// Manages the world and what is in it
/// </summary>
class WorldManager
{
	friend class Scheduler;
public:
	WorldManager();
	~WorldManager();

	/// <summary>
	/// Initializes the WorldManager and its subsystems
	/// </summary>
	void Initialize();

	/// <summary>
	/// Shuts the WorldManager and its subsystems down
	/// </summary>
	void Shutdown();

	/// <summary>
	/// Returns true if the application should quit
	/// </summary>
	bool ShouldQuit();
private:
	PhysicsManager m_PhysicsManager;
	ObjectManager m_ObjectManager;
	bool shouldQuit;

#if DX11 || DX12
	static DWORD WINAPI FirePhysicsThread(void* param);
	static DWORD WINAPI FireUpdateThread(void* param);
#elif GL43
	static void* FirePhysicsThread(void* param);
	static void* FireUpdateThread(void* param);
#endif
};

NS_END


#endif