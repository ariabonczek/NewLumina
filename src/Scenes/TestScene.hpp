#ifndef TESTSCENE_HPP
#define TESTSCENE_HPP

#include <Scenes\Scene.hpp>

class TestScene : public Lumina::Scene
{
public:
	TestScene();
	~TestScene();

	void LoadAssets(ID3D11Device* device);
	void DestroyAssets();
private:

};

#endif