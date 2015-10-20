//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <Core\Common.hpp>
#include <Graphics\Window.hpp>
#include <Graphics\GraphicsDevice.hpp>
#include <Graphics\GraphicsCommandList.hpp>
#include <Objects\Light.hpp>

NS_BEGIN


/// <summary>
/// Engine subsystem for handling rendering
/// </summary>
class Renderer
{
	friend class Scheduler;
public:
	~Renderer();

	static Renderer* GetInstance();

	/// <summary>
	/// 
	/// </summary>
	void Clear();

	/// <summary>
	/// 
	/// </summary>
	void ExecuteCommandLists();

	/// <summary>
	/// 
	/// </summary>
	void Flush();

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

	/// <summary>
	/// 
	/// </summary>
	void AddRenderableGameObject(BaseRenderer* renderer);

	/// <summary>
	/// 
	/// </summary>
	void AddLight(Light* light);

	/// <summary>
	/// Shuts down the renderer and its subsystems
	/// </summary>
	void RemoveRenderableGameObject(BaseRenderer* renderer);

	/// <summary>
	///
	/// </summary>
	void RemoveLight(Light* light);

	void UnloadCurrentScene();

	Camera const* GetActiveCamera()const;
	void SetActiveCamera(Camera* camera);
	void SetAmbientLight(Color color);

private:
	Renderer();

	Window* mp_Window;
	GraphicsDevice* mp_GraphicsDevice;

	void SetupCommandLists();
	void SetupFrame();
	void CloseCommandLists();

	GraphicsCommandList mp_OpaqueCommandList;
	GraphicsCommandList mp_TransparentCommandList;
	GraphicsCommandList mp_ParticlesCommandList;
	GraphicsCommandList mp_LightingCommandList;

	std::unordered_map<LGUID, BaseRenderer*> renderableObjects;
	std::unordered_map<LGUID, Light*> lights;
	Camera const* activeCamera;
	Color ambientLight;

	void SetLightData(Material* material);

	ID3D11DeviceContext* p_ImmediateContext;
	IDXGISwapChain3* p_SwapChain;
	ID3D11RenderTargetView* p_BackBuffer;
	ID3D11DepthStencilView* p_DepthBuffer;
	D3D11_VIEWPORT* p_Viewport;
	uint32 frameIndex;
	
	//-----------
	// Threading
	//-----------
#if DX11 || DX12
	/// <summary>
	/// Fires the command lists
	/// </summary>   
	static DWORD WINAPI FireThread(void* param);
	/// <summary>
	/// Registers draw calls to the command lists
	/// </summary>   
	static DWORD WINAPI Render(void* param);
#elif GL43
	static void* FireThread(void* param);
#endif
};

NS_END


#endif