#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP

#include <Core\Common.hpp>
#include <unordered_map>

NS_BEGIN

class SceneManager
{
public:
	~SceneManager();
	static SceneManager* GetInstance();
	void Initialize();
	void Shutdown();

	void AddScene(Scene* scene);
	void LoadLevel(char* name);
private:
	SceneManager();
	std::unordered_map<char*, Scene*> scenes;
};

NS_END

#endif