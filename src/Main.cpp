//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#include "Core\Game.hpp"
#include "Scenes\SceneManager.hpp"
#include "TestGame\TestScene.hpp"
#include "TestGame\DancingFlames.hpp"

//using namespace Lumina;

#if DX11 || DX12
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int showCmd)
#elif GL43
int main(int argc, char** argv)
#endif
{
	Lumina::SceneManager::GetInstance()->AddScene(new DancingFlamesScene());
	Lumina::SceneManager::GetInstance()->AddScene(new TestScene());
	return Lumina::Game::GetInstance().Run();
}