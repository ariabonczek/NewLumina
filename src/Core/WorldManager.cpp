//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#include <Core\WorldManager.hpp>

#include <Objects\GameObject.hpp>
#include <Objects\PhysicsObject.hpp>
#include <Objects\BaseRenderer.hpp>
#include <Scenes\Scene.hpp>

NS_BEGIN

WorldManager::WorldManager():
	shouldQuit(false)
{}

WorldManager::~WorldManager()
{

}

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

	activeObjects.reserve(UPDATABLES_EXPECTED);
	renderableObjects.reserve(RENDERABLES_EXPECTED);
	physicsObjects.reserve(PHYSICS_EXPECTED);
}

void WorldManager::Shutdown()
{

}

DWORD WINAPI WorldManager::Update(void* param)
{
	WorldManager* _this = static_cast<WorldManager*>(param);

	float dt = Timer::GetFrameTime();
	for (uint32 i = 0; i < _this->activeObjects.size(); ++i)
	{
		_this->activeObjects[i]->Update(dt);
	}
	return 0;
}

DWORD WINAPI WorldManager::Physics(void* param)
{
	WorldManager* _this = static_cast<WorldManager*>(param);

	// TODO: Perform physics calculations at a fixed timestep
	float dt = Timer::GetFrameTime();
	for (uint32 i = 0; i < _this->physicsObjects.size(); ++i)
	{
		//physicsObjects[i]->Update(dt);
	}
	return 0;
}

DWORD WINAPI WorldManager::Render(void* param)
{
	WorldManager* _this = static_cast<WorldManager*>(param);

	for (uint32 i = 0; i < _this->renderableObjects.size(); ++i)
	{
		_this->renderableObjects[i]->Render(_this->p_DeviceContext);
	}

	return 0;
}

bool WorldManager::ShouldQuit()
{
	return shouldQuit;
}

void WorldManager::UnloadCurrentScene()
{
	activeObjects.clear();
	renderableObjects.clear();
	physicsObjects.clear();
}

void WorldManager::LoadNewScene(Scene* scene)
{
	ID3D11Device* device;
	p_DeviceContext->GetDevice(&device);
	scene->LoadAssets(device);
	scene->Initialize();
	activeCamera = scene->activeCamera;
}

// TODO: Optimize this by expanding LGUID system and storing these in an unordered_map

void WorldManager::AddActiveGameObject(GameObject* gameObject)
{
	activeObjects.push_back(gameObject);
}

void WorldManager::AddRenderableGameObject(BaseRenderer* renderer)
{
	renderableObjects.push_back(renderer);
}

void WorldManager::AddPhysicsObject(PhysicsObject* physicsObject)
{
	physicsObjects.push_back(physicsObject);
}

void WorldManager::RemoveInactiveGameObject(GameObject* gameObject)
{
	std::vector<GameObject*>::iterator it = activeObjects.begin();
	for (it; *it != gameObject; ++it)
	{}

	activeObjects.erase(it);
}
void WorldManager::RemoveRenderableGameObject(BaseRenderer* renderer)
{
	std::vector<BaseRenderer*>::iterator it = renderableObjects.begin();
	for (it; *it != renderer; ++it)
	{}

	renderableObjects.erase(it);
}

void WorldManager::RemovePhysicsObject(PhysicsObject* physicsObject)
{
	std::vector<PhysicsObject*>::iterator it = physicsObjects.begin();
	for (it; *it != physicsObject; ++it)
	{}

	physicsObjects.erase(it);
}
NS_END