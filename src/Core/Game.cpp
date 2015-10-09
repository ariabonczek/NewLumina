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
		m_Scheduler.StartRenderThread();

		m_Scheduler.RunWorkerThreads();

		// Sync the threads
		m_Scheduler.WaitForSync();
	}

	// Safely shut down subsystems in order
	Shutdown();

	return 0;
}

void Game::Initialize()
{
	Allocator::Initialize(PROGRAM_HEAP_MEMORY_IN_BYTES);
#if _DEBUG
	Debug::Initialize();
	Debug::Log("Game Initialized");
#endif

	Filesystem::Initialize();
	ResourceManager::GetInstance().Initialize();
	p_Renderer = Renderer::GetInstance();			// Creates
	p_WorldManager = WorldManager::GetInstance();
	p_Renderer->Initialize();						// Initializes
	p_WorldManager->Initialize();
	m_Scheduler.Initialize(p_Renderer, p_WorldManager);
}

void Game::Shutdown()
{
	m_Scheduler.Shutdown();
	p_Renderer->Shutdown();
	p_WorldManager->Shutdown();
	ResourceManager::GetInstance().Shutdown();
	Filesystem::Shutdown();

#if _Debug
	Debug::Shutdown();
#endif
	Allocator::Shutdown();
}

bool Game::ShouldContinueRunning()
{
	return p_Renderer->HandleWindowEvents() && !p_WorldManager->ShouldQuit();
}

NS_END