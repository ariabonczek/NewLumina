#include "Scene.hpp"

#include <Objects\GameObject.hpp>

NS_BEGIN

Scene::Scene(char* name):
	name(name)
{}

Scene::~Scene()
{}

void Scene::Initialize()
{
	uint size = objects.size();
	for (uint i = 0; i < size; ++i)
	{
		objects[i]->Initialize();
	}
}

void Scene::Shutdown()
{

}

void Scene::AddObject(GameObject* object)
{
	objects.push_back(object);
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

}

char* Scene::GetName()const
{
	return name;
}

NS_END