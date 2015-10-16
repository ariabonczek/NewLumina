#ifndef SCENE_HPP
#define SCENE_HPP

#include <Core\Common.hpp>
#include <Graphics\Data.hpp>
#include <unordered_map>

NS_BEGIN

/// <summary>
///
/// </summary>
class Scene
{
	friend class WorldManager;
	friend class Renderer;
public:
	Scene(char* name);
	~Scene();

	/// <summary>
	///
	/// </summary>
	void Initialize();

	/// <summary>
	///
	/// </summary>
	void Shutdown();

	/// <summary>
	///
	/// </summary>
	virtual void LoadAssets() = 0;

	/// <summary>
	///
	/// </summary>
	virtual void DestroyAssets() = 0;

	/// <summary>
	/// 
	/// </summary>
	char* GetName()const;
protected:
	/// <summary>
	///
	/// </summary>
	void AddObject(GameObject* object);

	/// <summary>
	///
	/// </summary>
	void SetActiveCamera(Camera* camera);

	/// <summary>
	///
	/// </summary>
	void SetAmbientLight(Color color);
private:
	std::unordered_map<LGUID,GameObject*> objects;
	Camera* activeCamera;
	Color ambientLight;
	char* name;
};

NS_END

#endif