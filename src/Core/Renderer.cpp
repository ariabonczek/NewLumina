//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#include <Core\Renderer.hpp>

#include <Config.hpp>
#include <Graphics\Sampler.hpp>
#include <Graphics\Material.hpp>

NS_BEGIN

Renderer::Renderer()
{
	mp_Window = new Window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	mp_GraphicsDevice = new GraphicsDevice();
}

Renderer::~Renderer()
{
	
}

Renderer* Renderer::GetInstance()
{
	static Renderer instance;
	return &instance;
}

void Renderer::Initialize()
{
#if _DEBUG
	Debug::Log("Renderer Initialized");
#endif
	mp_Window->Initialize(mp_GraphicsDevice);
	mp_GraphicsDevice->Initialize(mp_Window);
#if DX11

	Sampler::InitializeSamplers(mp_GraphicsDevice->dev);
	p_Viewport = &mp_GraphicsDevice->viewport;
	p_SwapChain = mp_GraphicsDevice->swapChain;
	p_ImmediateContext = mp_GraphicsDevice->immCon;

	SetupCommandLists();

#elif DX12

#elif GL43

#endif
}

void Renderer::Shutdown()
{
	Sampler::DestroySamplers();
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

void Renderer::Clear()
{
	if (!p_BackBuffer)
		return;
	p_ImmediateContext->RSSetViewports(1, p_Viewport);
	p_ImmediateContext->OMSetRenderTargets(1, &p_BackBuffer, p_DepthBuffer);

	static const float color[4] = { 0.392f, 0.584f, 0.929f, 1.0f };
	p_ImmediateContext->ClearRenderTargetView(p_BackBuffer, color);
	p_ImmediateContext->ClearDepthStencilView(p_DepthBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Renderer::SetupCommandLists()
{
	mp_OpaqueCommandList.Create(mp_GraphicsDevice->immCon);
	//mp_TransparentCommandList.Create(mp_GraphicsDevice->immCon);
	//mp_ParticlesCommandList.Create(mp_GraphicsDevice->immCon);
	//mp_LightingCommandList.Create(mp_GraphicsDevice->immCon);
}

void Renderer::SetupFrame()
{
	mp_OpaqueCommandList.SetupFrame();
}

void Renderer::CloseCommandLists()
{
	mp_OpaqueCommandList.Finish();
	//mp_TransparentCommandList.Finish();
	//mp_ParticlesCommandList.Finish();
	//mp_LightingCommandList.Finish();
}

void Renderer::ExecuteCommandLists()
{
	mp_OpaqueCommandList.Execute();
	//mp_TransparentCommandList.Execute();
	//mp_ParticlesCommandList.Execute();
	//mp_LightingCommandList.Execute();
}

void Renderer::Flush()
{
	p_SwapChain->Present(VSYNC_ENABLED, 0);
	frameIndex = p_SwapChain->GetCurrentBackBufferIndex();
	p_BackBuffer = mp_GraphicsDevice->displayBuffers[frameIndex].renderTargetView;
	p_DepthBuffer = mp_GraphicsDevice->displayBuffers[frameIndex].depthStencilView;
}

#if DX11 || DX12
DWORD WINAPI Renderer::FireThread(void* param)
{
	Renderer* _this = static_cast<Renderer*>(param);
	
	_this->Clear();
	_this->ExecuteCommandLists();
	_this->Flush();

	return 0;
}
#elif GL43
void* Renderer::FireThread(void* param)
{
	Renderer* _this = (Renderer*)param;
	_this->Run();
	return 0;
}
#endif

NS_END