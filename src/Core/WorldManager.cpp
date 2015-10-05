//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#include <Core\WorldManager.hpp>

NS_BEGIN

WorldManager::WorldManager():
	shouldQuit(false)
{}

WorldManager::~WorldManager()
{

}

void WorldManager::Initialize()
{
#if _DEBUG
	Debug::Log("WorldManager Initialized");
#endif
	m_ObjectManager.Initialize();
	m_PhysicsManager.Initialize();
}

void WorldManager::Shutdown()
{
	m_PhysicsManager.Shutdown();
	m_ObjectManager.Shutdown();
}

bool WorldManager::ShouldQuit()
{
	return shouldQuit;
}

#if DX11 || DX12
DWORD WINAPI WorldManager::FirePhysicsThread(void* param)
#elif GL43
void* WorldManager::FirePhysicsThread(void* param)
#endif
{
	PhysicsManager* ptr = static_cast<PhysicsManager*>(param);
	ptr->Run();
	return 0;
}

#if DX11 || DX12
DWORD WINAPI WorldManager::FireUpdateThread(void* param)
#elif GL43
void* WorldManager::FireUpdateThread(void* param)
#endif
{
	ObjectManager* ptr = static_cast<ObjectManager*>(param);
	ptr->Run();
	return 0;
}

NS_END