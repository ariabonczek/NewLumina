//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <Core\Common.hpp>

NS_BEGIN

class Renderer;
class GraphicsDevice;

/// <summary>
/// Handles windowing using Windows for DirectX and GLFW for OpenGL
/// </summary>
class Window
{
public:
	Window(uint16 width, uint16 height, std::string title);
	~Window();

	/// <summary>
	/// Creates the window
	/// </summary>
	void Initialize(GraphicsDevice* graphicsDevice);

	/// <summary>
	/// Destroys the window
	/// </summary>
	void Shutdown();

	/// <summary>
	/// Checks the window for messages
	/// </summary>
	bool PollMessages();

	/// <summary>
	/// Handles messages received from the window
	/// </summary>
	bool HandleMessages();

	/// <summary>
	/// Closes the window
	/// </summary>
	void Close();

	/// <summary>
	/// Returns the width of the window
	/// </summary>
	uint16 GetWidth()const;

	/// <summary>
	/// Returns the height of the window
	/// </summary>
	uint16 GetHeight()const;

	/// <summary>
	/// Returns the aspect ratio
	/// </summary>
	float GetAspect()const;
#if DX11 || DX12
	/// <summary>
	/// Returns a handle to the window object
	/// </summary>
	HWND GetHandle()const;
#elif GL43
	/// <summary>
	/// Returns a handle to the window object
	/// </summary>
	GLFWwindow* GetHandle()const;
#endif

#if DX11 || DX12
	/// <summary>
	/// Handles Windows messages
	/// </summary>
	LRESULT HandleMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#elif GL43

#endif

private:
	uint16 width, height;
	std::string title;

#if DX11 || DX12
	HINSTANCE hInstance;
	HWND window;
	MSG msg;
	bool isOpen;
#elif GL43
	GLFWwindow* window;
#endif
};

NS_END

#endif