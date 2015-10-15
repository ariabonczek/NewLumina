//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#ifndef GAME_HPP
#define GAME_HPP

#include <Core\Common.hpp>
#include <Core\Renderer.hpp>
#include <Core\Scheduler.hpp>
#include <Core\WorldManager.hpp>
#include <Utility\ResourceManager.hpp>
#include <Utility\Filesystem.hpp>
#include <Scenes\SceneManager.hpp>

NS_BEGIN

/// <summary>
/// Top-level single to encapsulate the entire game
/// </summary>
class Game
{
public:
	~Game();

	static Game GetInstance();

	/// <summary>
	/// Starts the game, called in main/WinMain
	/// </summary>
	int32 Run();
private:
	Game();

	/// <summary>
	/// Initializes game subsystems and loads necessary data
	/// </summary>
	void Initialize();

	/// <summary>
	/// Shuts down game subsystems in a safe order
	/// </summary>
	void Shutdown();

	/// <summary>
	/// Used by the game loop. Returns true if the game should keep going
	/// </summary>
	bool ShouldContinueRunning();

	Scheduler m_Scheduler;
	Renderer* p_Renderer;
	WorldManager* p_WorldManager;
};

NS_END

#endif