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
public:
	Renderer();
	~Renderer();

	void BeginFrame();

	void EndFrame();

	/// <summary>
	/// Executes "previous" frame command list
	/// </summary>
	void ExecuteCommandList();

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

#if DX11 || DX12
	static DWORD WINAPI ThreadFunc(void* param);
#endif

private:
	Window* mp_Window;
	GraphicsDevice* mp_GraphicsDevice;
};

NS_END


#endif