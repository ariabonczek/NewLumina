//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#ifndef GRAPHICSDEVICE_HPP
#define GRAPHICSDEVICE_HPP

#include <Core\Common.hpp>
#include <Config.hpp>

NS_BEGIN

class Window;

/// <summary>
/// Renderer's handle to the graphics context
/// </summary>
class GraphicsDevice
{
public:
	GraphicsDevice();
	~GraphicsDevice();

	/// <summary>
	/// Initializes GraphicsDevice subsystems
	/// </summary>
	void Initialize(Window* window);

	/// <summary>
	/// Shuts down GraphicsDevice subsystems
	/// </summary>
	void Shutdown();

	/// <summary>
	/// Resets command lists and clears the render target
	/// </summary>
	void ResetCommandList();

	/// <summary>
	/// Closes the current command list to prevent further adding of draw calls
	/// </summary>
	void CloseCommandList();

	/// <summary>
	/// Executes the commands in the list
	/// </summary>
	void ExecuteCommandList();

	/// <summary>
	/// Called when the window is resized to handle recreation of back buffer and viewport
	/// </summary>
	void OnResize();
private:
	Window* window;

#if DX11 
	// DirectX11 Interfaces
	ID3D11Device* dev;
	ID3D11DeviceContext* immCon;
	ID3D11DeviceContext* defCon[2];
	ID3D11CommandList* commandList;
	uint32 contextIndex;		// Points the the context currently storing draw calls
	IDXGISwapChain3* swapChain;
	ID3D11Texture2D* depthBuffer[NUM_BUFFERS];
	ID3D11RenderTargetView* renderTargetView[NUM_BUFFERS];
	ID3D11DepthStencilView* depthStencilView[NUM_BUFFERS];
	D3D11_VIEWPORT viewport;
	D3D_FEATURE_LEVEL featureLevel;
	uint32 frameIndex;
#elif DX12
	// DirectX12 Interfaces
	ID3D12Device* dev;
	IDXGISwapChain3* swapChain;
	ID3D12CommandQueue* commandQueue;
	ID3D12CommandAllocator*	commandAllocator[2];
	ID3D12DescriptorHeap* rtvHeap;
	ID3D12Resource* renderTarget[NUM_BUFFERS];
	ID3D12RootSignature* rootSignature;
	ID3D12PipelineState* pipelineState;
	ID3D12GraphicsCommandList* commandList[2];
	uint32 commandIndex;
	uint32 executeIndex;
	D3D12_VIEWPORT viewport;
	uint32 descriptorSize;
	uint32 frameIndex;

	// Synchronization
	HANDLE fenceEvent;
	ID3D12Fence* fence;
	uint64 fenceValue;
#elif GL43

#endif
};

NS_END

#endif