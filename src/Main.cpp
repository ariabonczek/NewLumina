//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#include "Core/Game.hpp"

#if DX11 || DX12
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int showCmd)
#elif GL43
int main(int argc, char** argv)
#endif
{
	Lumina::Game game = Lumina::Game::GetInstance();
	return game.Run();
}