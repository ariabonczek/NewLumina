#ifndef SCENE_HPP
#define SCENE_HPP

#include <Core\Common.hpp>
#include <Graphics\Data.hpp>

NS_BEGIN

/// <summary>
///
/// </summary>
class Scene
{
	friend class WorldManager;
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
	virtual void LoadAssets(ID3D11Device* dev) = 0;

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
	std::vector<GameObject*> objects;
	Camera* activeCamera;
	Color ambientLight;
	char* name;
};

NS_END

#endif