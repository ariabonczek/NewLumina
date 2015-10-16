#include "SceneManager.hpp"
#include <Scenes\Scene.hpp>
#include <Core\Renderer.hpp>
#include <Core\WorldManager.hpp>

NS_BEGIN


SceneManager::SceneManager()
{}

SceneManager::~SceneManager()
{}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	return &instance;
}

void SceneManager::Initialize()
{
	LoadLevel("TestScene");
}

void SceneManager::Shutdown()
{

}

void SceneManager::AddScene(Scene* scene)
{
	scenes.insert_or_assign(scene->GetName(), scene);
}

void SceneManager::LoadLevel(char* name)
{
	WorldManager::GetInstance()->UnloadCurrentScene();
	Renderer::GetInstance()->UnloadCurrentScene();

	scenes[name]->LoadAssets();
	scenes[name]->Initialize();
}

NS_END