//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <Core\Common.hpp>
#include <Graphics\Window.hpp>
#include <Graphics\GraphicsDevice.hpp>

NS_BEGIN

/// <summary>
/// Engine subsystem for handling rendering
/// </summary>
class Renderer
{
	friend class Scheduler;
public:
	Renderer();
	~Renderer();

	/// <summary>
	/// Resets the renderer and prepares for a new frame
	/// </summary>
	void BeginFrame();

	/// <summary>
	/// Closes the command list and prepares for execution on the next frame
	/// </summary>
	void EndFrame();

	/// <summary>
	/// Poll for and handle events from the window
	/// </summary>
	bool HandleWindowEvents();

	/// <summary>
	/// Initializes the Renderer's subsystems
	/// </summary>
	void Initialize();

	/// <summary>
	/// Shuts down the renderer and its subsystems
	/// </summary>
	void Shutdown();
private:
	Window* mp_Window;
	GraphicsDevice* mp_GraphicsDevice;

	/// <summary>
	/// Runs graphics thread (only when called from the Scheduler!)
	/// </summary>
	void Run();

#if DX11 || DX12
	static DWORD WINAPI FireThread(void* param);
#elif GL43
	static void* FireThread(void* param);
#endif
};

NS_END


#endif