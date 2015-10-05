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
		// Start drawing the "previous" frame
		m_Scheduler.StartThreads();

		m_Renderer.BeginFrame();

		// Render the world here

		m_Renderer.EndFrame();

		// Sync the threads
		m_Scheduler.WaitForSync();
	}

	// Safely shut down subsystems in order
	Shutdown();

	return 0;
}

void Game::Initialize()
{
#if _DEBUG
	Debug::Initialize();
	Debug::Log("Game Initialized");
#endif
	m_Renderer.Initialize();
	m_WorldManager.Initialize();
	m_Scheduler.Initialize(&m_Renderer, &m_WorldManager);
}

void Game::Shutdown()
{
	m_Scheduler.Shutdown();
	m_Renderer.Shutdown();
	m_WorldManager.Shutdown();
#if _Debug
	Debug::Shutdown();
#endif
}

bool Game::ShouldContinueRunning()
{
	return m_Renderer.HandleWindowEvents() && !m_WorldManager.ShouldQuit();
}

NS_END