#ifndef TESTSCENE_HPP
#define TESTSCENE_HPP

#include <Scenes\Scene.hpp>
#include <Lumina.hpp>


class TestScene : public Lumina::Scene
{
public:
	TestScene();
	~TestScene();

	void LoadAssets();
	void DestroyAssets();
private:

};

#endif		