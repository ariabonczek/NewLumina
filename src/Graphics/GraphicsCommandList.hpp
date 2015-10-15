#ifndef GRAPHICS_COMMAND_LIST_HPP
#define GRAPHICS_COMMAND_LIST_HPP

#include <Core\Common.hpp>

NS_BEGIN

class GraphicsCommandList
{
public:
	GraphicsCommandList();
	~GraphicsCommandList();
#if DX11
	void Create(ID3D11DeviceContext* immContext);
#elif DX12
	void Create(ID3D12Device* device);
#endif

	void SetupFrame();
	void Finish();
	void Execute();

	ID3D11DeviceContext* GetDeferredContext()const;

private:
#if DX11
	ID3D11DeviceContext* p_ImmediateContext;
	ID3D11DeviceContext* context;
	ID3D11CommandList* commandList;
	ID3D11RenderTargetView* renderTarget;
	ID3D11DepthStencilView* depthStencil;
	D3D11_VIEWPORT viewport;
#elif DX12
	ID3D12GraphicsCommandList* commandList;
	ID3D12CommandAllocator* commandAllocator;
#elif GL43

#endif
};

NS_END

#endif