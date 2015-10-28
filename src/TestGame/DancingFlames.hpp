#ifndef DANCING_FLAMES_SCENE_HPP
#define DANCING_FLAMES_SCENE_HPP

#include <Scenes\Scene.hpp>
#include <Lumina.hpp>


class DancingFlamesScene : public Lumina::Scene
{
public:
	DancingFlamesScene();
	~DancingFlamesScene();

	void LoadAssets();
	void DestroyAssets();
private:

};

#endif