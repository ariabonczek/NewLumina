//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#include "Core\Game.hpp"
#include "Scenes\SceneManager.hpp"
#include "TestGame\TestScene.hpp"
#include "TestGame\DancingFlames.hpp"

using namespace Lumina;

#if DX11 || DX12
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int showCmd)
#elif GL43
int main(int argc, char** argv)
#endif
{
	SceneManager::GetInstance()->AddScene(new DancingFlamesScene());
	SceneManager::GetInstance()->AddScene(new TestScene());
	return Game::GetInstance().Run();
}