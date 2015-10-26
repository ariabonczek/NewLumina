//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#include <Core\Renderer.hpp>

#include <Config.hpp>
#include <Graphics\Sampler.hpp>
#include <Graphics\RasterizerState.hpp>
#include <Graphics\BlendState.hpp>
#include <Graphics\DepthStencilState.hpp>
#include <Graphics\Material.hpp>
#include <Objects\GameObject.hpp>
#include <Objects\Transform.hpp>
#include <Objects\BaseRenderer.hpp>
#include <Scenes\Scene.hpp>
#include <Objects\Camera.hpp>
#include <Utility\ResourceManager.hpp>
#include <Graphics\RenderTexture.hpp>

NS_BEGIN

static const float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

Renderer::Renderer()
{
	mp_Window = new Window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	mp_GraphicsDevice = new GraphicsDevice();
}

Renderer::~Renderer()
{}

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
	RasterizerState::InitializeStates(mp_GraphicsDevice->dev);
	BlendState::InitializeStates(mp_GraphicsDevice->dev);
	DepthStencilState::InitializeStates(mp_GraphicsDevice->dev);
	p_Viewport = &mp_GraphicsDevice->viewport;
	p_SwapChain = mp_GraphicsDevice->swapChain;
	p_ImmediateContext = mp_GraphicsDevice->immCon;

	InitializeRenderTarget();
	SetupCommandLists();

#elif DX12

#elif GL43

#endif
}

void Renderer::Shutdown()
{
	delete finalRenderMaterial;
	delete lightMaterial;
	delete nullMaterial;
	Sampler::DestroySamplers();
	RasterizerState::DestroyStates();
	BlendState::DestroyStates();
	DepthStencilState::DestroyStates();
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

void Renderer::InitializeRenderTarget()
{
	ID3D11Device* device;
	p_ImmediateContext->GetDevice(&device);
	
	gbuffer.albedo = ResourceManager::CreateRenderTexture(p_Viewport->Width, p_Viewport->Height, false);
	gbuffer.normals = ResourceManager::CreateRenderTexture(p_Viewport->Width, p_Viewport->Height, false);
	gbuffer.lit = ResourceManager::CreateRenderTexture(p_Viewport->Width, p_Viewport->Height, false);

	finalRender.Initialize(device);
	finalRenderMaterial = new Material();
	finalRenderMaterial->SetVertexShader((VertexShader*)ResourceManager::LoadShader(L"Shaders/DirectX/fullScreenQuadVertex.cso", ShaderType::Vertex));
	finalRenderMaterial->SetPixelShader((PixelShader*)ResourceManager::LoadShader(L"Shaders/DirectX/copyTexture.cso", ShaderType::Pixel));
	finalRender.SetMaterial(finalRenderMaterial);

	lightMaterial = new Material();
	lightMaterial->SetVertexShader((VertexShader*)ResourceManager::LoadShader(L"Shaders/DirectX/deferredLightVertex.cso", ShaderType::Vertex));
	lightMaterial->SetPixelShader((PixelShader*)ResourceManager::LoadShader(L"Shaders/DirectX/deferredLightPixel.cso", ShaderType::Pixel));

	nullMaterial = new Material();
	nullMaterial->SetVertexShader((VertexShader*)ResourceManager::LoadShader(L"Shaders/DirectX/nullVertex.cso", ShaderType::Vertex));
	nullMaterial->SetPixelShader((PixelShader*)ResourceManager::LoadShader(L"Shaders/DirectX/nullPixel.cso", ShaderType::Pixel));
}

void Renderer::SetupCommandLists()
{
	mp_OpaqueCommandList.Create(mp_GraphicsDevice->immCon);
	//mp_TransparentCommandList.Create(mp_GraphicsDevice->immCon);
	//mp_ParticlesCommandList.Create(mp_GraphicsDevice->immCon);
	//mp_LightingCommandList.Create(mp_GraphicsDevice->immCon);
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
	p_DepthSrv = mp_GraphicsDevice->displayBuffers[frameIndex].depthSrv;
}

void Renderer::AddRenderableGameObject(BaseRenderer* renderer)
{
	renderableObjects[renderer->GetLGUID()] = renderer;
}

void Renderer::RemoveRenderableGameObject(BaseRenderer* renderer)
{
	std::unordered_map<LGUID, BaseRenderer*>::iterator it = renderableObjects.begin();
	for (it; it->second != renderer; ++it)
	{}
	renderableObjects.erase(it);
}

void Renderer::AddLight(Light* light)
{
	lights[light->GetLGUID()] = light;
}

void Renderer::RemoveLight(Light* light)
{
	std::unordered_map<LGUID, Light*>::iterator it = lights.begin();
	for (it; it->second != light; ++it)
	{
	}
	lights.erase(it);
}

void Renderer::SetLightData(Material* material)
{
	uint32 numDL = 0;
	uint32 numPL = 0;
	uint32 numSL = 0;
	for (std::unordered_map<LGUID, Light*>::iterator it = lights.begin(); it != lights.end(); ++it)
	{
		Light* light = it->second;
		switch (light->type)
		{
		case LightType::Directional:
			material->SetShaderVariable<LightData>("directionalLight", light->data, numDL++, ShaderType::Pixel);
			material->SetShaderVariable<uint32>("numDirectionalLights", numDL, ShaderType::Pixel);
			break;
		case LightType::Point:
			material->SetShaderVariable<LightData>("pointLight", light->data, numPL++, ShaderType::Pixel);
			material->SetShaderVariable<uint32>("numPointLights", numPL, ShaderType::Pixel);
			break;
		case LightType::Spot:
			material->SetShaderVariable<LightData>("spotLight", light->data, numSL++, ShaderType::Pixel);
			material->SetShaderVariable<uint32>("numSpotLights", numSL, ShaderType::Pixel);
			break;
		}
	}
	material->SetShaderVariable<Color>("ambientLight", ambientLight, ShaderType::Pixel);
}

Camera const * Renderer::GetActiveCamera()const
{
	return activeCamera;
}

void Renderer::SetActiveCamera(Camera* camera)
{
	activeCamera = camera;
}

void Renderer::SetAmbientLight(Color color)
{
	ambientLight = color;
}

void Renderer::AddPostProcess(PostProcess* postProcess)
{
	postProcesses.push_back(postProcess);
	postProcess->Initialize(mp_GraphicsDevice->dev);
}

void Renderer::UnloadCurrentScene()
{
	renderableObjects.clear();
}

#if DX11 || DX12
DWORD WINAPI Renderer::FireThread(void* param)
{
	Renderer* _this = static_cast<Renderer*>(param);

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

//--------------------------------
// SCENE COMMANDS REGISTERED HERE!
//--------------------------------
DWORD WINAPI Renderer::Render(void* param)
{
	Renderer* _this = static_cast<Renderer*>(param);

	_this->SetupGBuffer(_this->mp_OpaqueCommandList.GetDeferredContext());
	_this->RenderPass(RenderPassType::OpaqueGeometry);
	_this->RenderPass(RenderPassType::StencilPass);
	_this->RenderPass(RenderPassType::DeferredLighting);

	_this->ClearBackBuffer(_this->mp_OpaqueCommandList.GetDeferredContext());
	_this->RenderToBackBuffer(_this->mp_OpaqueCommandList.GetDeferredContext());

	return 0;
}

void Renderer::SetupGBuffer(ID3D11DeviceContext* deferredContext)
{
	deferredContext->RSSetViewports(1, p_Viewport);
	ID3D11RenderTargetView* rtv[] = { gbuffer.albedo->GetRenderTargetView(), gbuffer.normals->GetRenderTargetView() };
	deferredContext->OMSetRenderTargets(2, rtv, p_DepthBuffer);
	deferredContext->RSSetState(RasterizerState::FrontSolid->GetState());

	//static const float color[4] = { 0.392f, 0.584f, 0.929f, 1.0f };
    static const float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	deferredContext->ClearRenderTargetView(rtv[0], color);
	deferredContext->ClearRenderTargetView(rtv[1], color);
	deferredContext->ClearRenderTargetView(gbuffer.lit->GetRenderTargetView(), color);
	
	deferredContext->ClearDepthStencilView(p_DepthBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Renderer::RenderPass(RenderPassType type)
{
	ID3D11DeviceContext* deferredContext = mp_OpaqueCommandList.GetDeferredContext();
	ID3D11RenderTargetView* rtv;

	switch (type)
	{
		case RenderPassType::OpaqueGeometry:
			deferredContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			deferredContext->RSSetState(RasterizerState::FrontSolid->GetState());
			deferredContext->OMSetDepthStencilState(DepthStencilState::Default->GetState(),0);

			for (std::unordered_map<LGUID, BaseRenderer*>::iterator it = renderableObjects.begin(); it != renderableObjects.end(); ++it)
			{
				it->second->Render(deferredContext);
			}
			break;
		case RenderPassType::StencilPass:
			deferredContext->RSSetState(RasterizerState::BothSolid->GetState());
			deferredContext->OMSetBlendState(BlendState::None->GetState(), blendFactor, 0xffffff);
			
			rtv = gbuffer.lit->GetRenderTargetView();
			
			deferredContext->OMSetRenderTargets(0, 0, p_DepthBuffer);
			deferredContext->ClearDepthStencilView(p_DepthBuffer, D3D11_CLEAR_STENCIL, 1.0f, 0);
			deferredContext->OMSetDepthStencilState(DepthStencilState::StencilPass->GetState(), 0);
			
			nullMaterial->SetShaderVariable("view", activeCamera->GetView().Transpose(), ShaderType::Vertex);
			nullMaterial->SetShaderVariable("projection", activeCamera->GetProj().Transpose(), ShaderType::Vertex);
			
			for (std::unordered_map<LGUID, Light*>::iterator it = lights.begin(); it != lights.end(); ++it)
			{
				nullMaterial->SetShaderVariable("world", it->second->GetGameObject()->GetComponent<Transform>()->GetWorldMatrix().Transpose(), ShaderType::Vertex);
			
				nullMaterial->BindMaterial(deferredContext);
				it->second->RenderLightAsGeometry(deferredContext);
			}
			
			break;
		case RenderPassType::DeferredLighting:
			deferredContext->RSSetState(RasterizerState::BackSolid->GetState());
			deferredContext->OMSetBlendState(BlendState::Additive->GetState(), blendFactor, 0xffffff);

			rtv = gbuffer.lit->GetRenderTargetView();
			deferredContext->OMSetRenderTargets(1, &rtv, 0);// p_DepthBuffer);
			deferredContext->OMSetDepthStencilState(DepthStencilState::Deferred->GetState(), 0);
			
			lightMaterial->SetShaderVariable("view", activeCamera->GetView().Transpose(), ShaderType::Vertex);
			lightMaterial->SetShaderVariable("projection", activeCamera->GetProj().Transpose(), ShaderType::Vertex);
			lightMaterial->SetShaderVariable("invViewProjection", Matrix::Inverse(activeCamera->GetViewProjection()).Transpose(), ShaderType::Pixel);
			lightMaterial->SetShaderVariable("width", p_Viewport->Width, ShaderType::Pixel);
			lightMaterial->SetShaderVariable("height", p_Viewport->Height, ShaderType::Pixel);

			lightMaterial->SetTexture("_Albedo", "_Sampler", gbuffer.albedo, ShaderType::Pixel);
			lightMaterial->SetTexture("_Normal", "_Sampler", gbuffer.normals, ShaderType::Pixel);
			lightMaterial->SetTextureEX("_Depth", "_DepthSampler", p_DepthSrv , Sampler::ClampPoint->GetSamplerState(), ShaderType::Pixel);

			for (std::unordered_map<LGUID, Light*>::iterator it = lights.begin(); it != lights.end(); ++it)
			{
				lightMaterial->SetShaderVariable("world", it->second->GetGameObject()->GetComponent<Transform>()->GetWorldMatrix().Transpose(), ShaderType::Vertex);
				lightMaterial->SetShaderVariable("pointlight", it->second->data, ShaderType::Pixel);

				lightMaterial->BindMaterial(deferredContext);
				it->second->RenderLightAsGeometry(deferredContext);
			}
			break;
	}
}

void Renderer::ClearBackBuffer(ID3D11DeviceContext* deferredContext)
{
	if (!p_BackBuffer)
		return;
	deferredContext->RSSetViewports(1, p_Viewport);
	deferredContext->OMSetRenderTargets(1, &p_BackBuffer, p_DepthBuffer);

	static const float color[4] = { 0.392f, 0.584f, 0.929f, 1.0f };
	//static const float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	deferredContext->ClearRenderTargetView(p_BackBuffer, color);
	deferredContext->ClearDepthStencilView(p_DepthBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Renderer::RenderToBackBuffer(ID3D11DeviceContext* deferredContext)
{
	deferredContext->OMSetBlendState(BlendState::None->GetState(), blendFactor, 0xffffff);
	deferredContext->RSSetState(RasterizerState::FrontSolid->GetState());
	deferredContext->OMSetDepthStencilState(DepthStencilState::Default->GetState(), 0);

	finalRender.GetMaterial()->SetTexture("_Source", "_Sampler", gbuffer.lit, ShaderType::Pixel);
	finalRender.Render(deferredContext);
}

NS_END