//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#include <Core\Window.hpp>

#include <Core\Game.hpp>

NS_BEGIN

#if DX11 || DX12

// Global pointer to the window for message handleing
Window* gWindow;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return gWindow->HandleMessages(hwnd, msg, wParam, lParam);
}
#endif

Window::Window(uint16 width, uint16 height, std::string title):
	width(width), height(height), title(title)
{
#if DX11 || DX12
	gWindow = this;
	hInstance = GetModuleHandle(NULL);
	msg = { 0 };
#endif
}

Window::~Window()
{}

//
// WARNING: DO NOT USE graphicsDevice - it is created but not initialized!!!
//
void Window::Initialize(GraphicsDevice* graphicsDevice)
{
#if DX11 || DX12

	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(WNDCLASS));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "WindowClassMain";
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.cbWndExtra = NULL;
	wc.cbClsExtra = NULL;

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, "RegisterClass failed.", NULL, NULL);
	}

	RECT wr = { 0, 0, width, height };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	window = CreateWindow(
		"WindowClassMain",
		title.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!window)
	{
		MessageBox(NULL, "CreateWindow Failed.", NULL, NULL);
	}

	ShowWindow(window, SW_SHOW);
	UpdateWindow(window);
	isOpen = true;

#elif GL43

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Enable multisampling
	//glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	glfwMakeContextCurrent(window);	
#endif
}

void Window::Shutdown()
{
#if DX11 || DX12

#elif GL43
	glfwDestroyWindow(window);
#endif
}

bool Window::PollMessages()
{
#if DX11 || DX12
	return !!PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE);
#elif GL43
	glfwPollEvents();
	return true;
#endif	
}

bool Window::HandleMessages()
{
#if DX11 || DX12
	TranslateMessage(&msg);
	DispatchMessage(&msg);
	return isOpen;
#elif GL43
	return !glfwWindowShouldClose(window);
#endif
}

void Window::Close()
{
#if DX11 || DX12
	PostQuitMessage(0);
	isOpen = false;
#elif GL43
	glfwSetWindowShouldClose(window, true);
#endif	
}

#if DX11 || DX12

LRESULT Window::HandleMessages(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_QUIT:
		Close();
		return 0;
	case WM_SIZE:
		return 0;
	case WM_ENTERSIZEMOVE:
		return 0;
	case WM_EXITSIZEMOVE:
		return 0;
	case WM_DESTROY:
		Close();
		return 0;
	case WM_MENUCHAR:
		return MAKELRESULT(0, MNC_CLOSE);
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
#endif

uint16 Window::GetWidth()const
{
	return width;
}

uint16 Window::GetHeight()const
{
	return height;
}

#if DX11 || DX12
HWND Window::GetHandle()const
#elif GL43
GLFWwindow* Window::GetHandle()const
#endif
{
	return window;
}


NS_END