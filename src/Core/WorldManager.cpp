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
}

void WorldManager::Shutdown()
{

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
	for (std::unordered_map<LGUID, PhysicsObject*>::iterator it = _this->physicsObjects.begin(); it != _this->physicsObjects.end(); ++it)
	{
		//physicsObjects[i]->Update(dt);
	}
	return 0;
}

DWORD WINAPI WorldManager::Render(void* param)
{
	WorldManager* _this = static_cast<WorldManager*>(param);

	for (std::unordered_map<LGUID, BaseRenderer*>::iterator it = _this->renderableObjects.begin(); it != _this->renderableObjects.end(); ++it)
	{
		it->second->Render(_this->p_DeviceContext);
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
	activeObjects[gameObject->GetLGUID()] = gameObject;
}

void WorldManager::AddRenderableGameObject(BaseRenderer* renderer)
{
	renderableObjects[renderer->GetLGUID()] = renderer;
}

void WorldManager::AddPhysicsObject(PhysicsObject* physicsObject)
{
	physicsObjects[physicsObject->GetLGUID()] = physicsObject;
}

void WorldManager::RemoveInactiveGameObject(GameObject* gameObject)
{
	std::unordered_map<LGUID, GameObject*>::iterator it = activeObjects.begin();
	for (it; it->second != gameObject; ++it)
	{}

	activeObjects.erase(it);
}
void WorldManager::RemoveRenderableGameObject(BaseRenderer* renderer)
{
	std::unordered_map<LGUID, BaseRenderer*>::iterator it = renderableObjects.begin();
	for (it; it->second != renderer; ++it)
	{}

	renderableObjects.erase(it);
}

void WorldManager::RemovePhysicsObject(PhysicsObject* physicsObject)
{
	std::unordered_map<LGUID, PhysicsObject*>::iterator it = physicsObjects.begin();
	for (it; it->second != physicsObject; ++it)
	{}

	physicsObjects.erase(it);
}
NS_END