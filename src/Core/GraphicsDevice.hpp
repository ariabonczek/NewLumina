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
	/// Clears the command lists and prepares for storing new draw calls
	/// </summary>
	void ResetCommandList();

	/// <summary>
	/// Clears the render target (back buffer)
	/// </summary>
	void Clear();

	/// <summary>
	/// Swaps the buffers and displays
	/// </summary>
	void Display();

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
	ID3D11DeviceContext* defCon;
	IDXGISwapChain* swapChain;
	ID3D11Texture2D* depthBuffer;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	D3D11_VIEWPORT viewport;
	D3D_FEATURE_LEVEL featureLevel;
#elif DX12
	// DirectX12 Interfaces
	ID3D12Device* dev;
	IDXGISwapChain3* swapChain;
	ID3D12CommandQueue* commandQueue;
	ID3D12CommandAllocator*	commandAllocator;
	ID3D12DescriptorHeap* rtvHeap;
	ID3D12Resource* renderTarget[NUM_BUFFERS];
	ID3D12RootSignature* rootSignature;
	ID3D12PipelineState* pipelineState;
	ID3D12GraphicsCommandList* commandList;
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