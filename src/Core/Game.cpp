//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#include <Core\Game.hpp>

NS_BEGIN

Game::Game()
{}

Game::~Game()
{}

Game Game::GetInstance()
{
	static Game instance;
	return instance;
}

int32 Game::Run()
{
	// Initialize game subsystems and setup data
	Initialize();

	// Game loop
	while (ShouldContinueRunning())
	{
		m_Renderer.Render();
	}

	// Safely shut down subsystems in order
	Shutdown();

	return 0;
}

void Game::Initialize()
{
	m_Renderer.Initialize();
}

void Game::Shutdown()
{
	m_Renderer.Shutdown();
}

bool Game::ShouldContinueRunning()
{
	return m_Renderer.HandleWindowEvents(); // mp_SceneManager.ShouldQuit();
}

NS_END