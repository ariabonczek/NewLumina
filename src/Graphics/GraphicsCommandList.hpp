#ifndef GRAPHICS_COMMAND_LIST_HPP
#define GRAPHICS_COMMAND_LIST_HPP

#include <Core\Common.hpp>

NS_BEGIN

/// <summary>
/// Encapsulates data for registering command lists for rendering
/// </summary>
class GraphicsCommandList
{
public:
	GraphicsCommandList();
	~GraphicsCommandList();
#if DX11
	/// <summary>
	///
	/// </summary>
	void Create(ID3D11DeviceContext* immContext);
#elif DX12
	/// <summary>
	///
	/// </summary>
	void Create(ID3D12Device* device);
#endif
	/// <summary>
	///
	/// </summary>
	void SetupFrame();

	/// <summary>
	///
	/// </summary>
	void Finish();

	/// <summary>
	///
	/// </summary>
	void Execute();

	/// <summary>
	///
	/// </summary>
	ID3D11DeviceContext* GetDeferredContext()const;

private:
#if DX11
	ID3D11DeviceContext* p_ImmediateContext;
	ID3D11DeviceContext* context;
	ID3D11CommandList* commandList;
	ID3D11RenderTargetView* renderTarget;
	ID3D11DepthStencilView* depthStencil;
	D3D11_VIEWPORT viewport;
	ID3D11RasterizerState* rasterizerState;
#elif DX12
	ID3D12GraphicsCommandList* commandList;
	ID3D12CommandAllocator* commandAllocator;
#elif GL43

#endif
};

NS_END

#endif