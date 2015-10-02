//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <Core\Common.hpp>
#include <Core\Window.hpp>
#include <Core\GraphicsDevice.hpp>

NS_BEGIN

/// <summary>
/// Engine subsystem for handling rendering
/// </summary>
class Renderer
{
public:
	Renderer();
	~Renderer();

	/// <summary>
	/// Render the scene
	/// </summary>
	void Render();

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
};

NS_END


#endif