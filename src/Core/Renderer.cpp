//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#include <Core\Renderer.hpp>

#include <Config.hpp>

NS_BEGIN

Renderer::Renderer()
{
	mp_Window = new Window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	mp_GraphicsDevice = new GraphicsDevice();
}

Renderer::~Renderer()
{
	
}

void Renderer::Initialize()
{
	mp_Window->Initialize(mp_GraphicsDevice);
	mp_GraphicsDevice->Initialize(mp_Window);

#if DX11 || DX12

#elif GL43

#endif
}

void Renderer::Shutdown()
{
	mp_Window->Shutdown();
	mp_GraphicsDevice->Shutdown();
}

bool Renderer::HandleWindowEvents()
{
	if (mp_Window->PollMessages())
	{
		return mp_Window->HandleMessages();
	}

	return true;
}

void Renderer::BeginFrame()
{
	mp_GraphicsDevice->ResetCommandList();
}

void Renderer::EndFrame()
{
	mp_GraphicsDevice->CloseCommandList();
}

void Renderer::ExecuteCommandList()
{
	mp_GraphicsDevice->ExecuteCommandList();
}

#if DX11 || DX12
DWORD WINAPI Renderer::ThreadFunc(void* param)
{
	Renderer* _this = (Renderer*)param;
	_this->ExecuteCommandList();
	return 0;
}
#elif GL43
void* Renderer::ThreadFunc(void* param)
{
	Renderer* _this = (Renderer*)param;
	_this->ExecuteCommandList();
	return 0;
}
#endif

NS_END