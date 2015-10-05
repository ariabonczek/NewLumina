//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#include <Core\PhysicsManager.hpp>

NS_BEGIN

PhysicsManager::PhysicsManager()
{

}

PhysicsManager::~PhysicsManager()
{

}

void PhysicsManager::Initialize()
{
#if _DEBUG
	Debug::Log("PhysicsManager Initialized");
#endif
}

void PhysicsManager::Shutdown()
{

}

void PhysicsManager::Run()
{

}

NS_END