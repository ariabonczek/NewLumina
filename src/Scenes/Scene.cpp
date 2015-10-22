#include "Scene.hpp"

#include <Objects\GameObject.hpp>
#include <Core\Renderer.hpp>

NS_BEGIN

Scene::Scene(char* name):
	name(name)
{}

Scene::~Scene()
{}

void Scene::Initialize()
{
	Renderer::GetInstance()->SetActiveCamera(activeCamera);
	Renderer::GetInstance()->SetAmbientLight(ambientLight);

	for (std::unordered_map<LGUID, GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		it->second->Initialize();
	}

	for (uint i = 0; i < postProcesses.size(); ++i)
	{
		Renderer::GetInstance()->AddPostProcess(postProcesses.front());
		postProcesses.pop();
	}
}

void Scene::Shutdown()
{
	for (std::unordered_map<LGUID, GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		it->second->Destroy();
	}
}

void Scene::AddObject(GameObject* object)
{
	objects[object->GetLGUID()] = object;
}

void Scene::SetActiveCamera(Camera* camera)
{
	if (camera)
	{
		activeCamera = camera;
	}
}

void Scene::SetAmbientLight(Color color)
{
	ambientLight = color;
}

void Scene::AddPostProcess(PostProcess* postProcess)
{
	postProcesses.push(postProcess);
}

char* Scene::GetName()const
{
	return name;
}

NS_END