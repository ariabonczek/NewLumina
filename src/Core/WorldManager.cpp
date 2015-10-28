//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#include <Core\WorldManager.hpp>

#include <Objects\GameObject.hpp>
#include <Objects\BaseRenderer.hpp>
#include <Scenes\Scene.hpp>

NS_BEGIN

WorldManager::WorldManager() :
	shouldQuit(false)//accumulator(0.0f)
{}

WorldManager::~WorldManager()
{}

WorldManager* WorldManager::GetInstance()
{
	static WorldManager instance;
	return &instance;
}

void WorldManager::Initialize()
{
#if _DEBUG
	Debug::Log("WorldManager Initialized");
#endif

	//foundation = PxCreateFoundation(PX_PHYSICS_VERSION, g_DefaultAllocator, g_DefaultErrorCallback);
	//physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale(), true);

}

void WorldManager::Shutdown()
{
#if _DEBUG
	Debug::Log("WorldManager Shutdown");
#endif

	//physics->release();
//	foundation->release();
}

DWORD WINAPI WorldManager::Update(void* param)
{
	WorldManager* _this = static_cast<WorldManager*>(param);

	float dt = Timer::GetFrameTime();
	for (std::unordered_map<LGUID, GameObject*>::iterator it = _this->activeObjects.begin(); it != _this->activeObjects.end(); ++it)
	{
		it->second->Update();
	}
	return 0;
}

DWORD WINAPI WorldManager::Physics(void* param)
{
	WorldManager* _this = static_cast<WorldManager*>(param);

	// TODO: Perform physics calculations at a fixed timestep
	float dt = Timer::GetFrameTime();
	//_this->accumulator += dt;
	//
	//if (_this->accumulator < STEP_SIZE)
	//	return 0;
	//
	//_this->scene->simulate(STEP_SIZE);
	//
	//_this->scene->fetchResults(true);

	return 0;
}

bool WorldManager::ShouldQuit()
{
	return shouldQuit;
}

void WorldManager::UnloadCurrentScene()
{
	activeObjects.clear();
	physicsObjects.clear();
}

void WorldManager::CreatePhysicsScene()
{
	//PxSceneDesc sd(physics->getTolerancesScale());
	//sd.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	//
	//if (!sd.cpuDispatcher)
	//{
	//	sd.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	//}
	//if (!sd.filterShader)
	//{
	//	// TODO
	//	//sd.filterShader = PxDefaultSimulationFilterShader();
	//}
	//
	//scene = physics->createScene(sd);
	//
	//// HARDCODED SCENE CHANGE THIS LATER
	//material = physics->createMaterial(0.5f, 0.5f, 0.1f);
}

void WorldManager::AddActiveGameObject(GameObject* gameObject)
{
	activeObjects[gameObject->GetLGUID()] = gameObject;
}

void WorldManager::AddPhysicsObject(PhysicsObject* physicsObject)
{
	//physicsObjects[physicsObject->GetLGUID()] = physicsObject;
}

void WorldManager::RemoveInactiveGameObject(GameObject* gameObject)
{
	std::unordered_map<LGUID, GameObject*>::iterator it = activeObjects.begin();
	for (it; it->second != gameObject; ++it)
	{}

	activeObjects.erase(it);
}

void WorldManager::RemovePhysicsObject(PhysicsObject* physicsObject)
{
	std::unordered_map<LGUID, PhysicsObject*>::iterator it = physicsObjects.begin();
	for (it; it->second != physicsObject; ++it)
	{}

	physicsObjects.erase(it);
}

NS_END