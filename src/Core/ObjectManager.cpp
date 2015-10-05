//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#include <Core\ObjectManager.hpp>

NS_BEGIN

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{

}

void ObjectManager::Initialize()
{
#if _DEBUG
	Debug::Log("ObjectManager Initialized");
#endif
}

void ObjectManager::Shutdown()
{

}

void ObjectManager::Run()
{

}

NS_END