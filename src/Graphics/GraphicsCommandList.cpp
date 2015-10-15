#include <Graphics\GraphicsCommandList.hpp>

NS_BEGIN

GraphicsCommandList::GraphicsCommandList()
{}

GraphicsCommandList::~GraphicsCommandList()
{}

#if DX11
void GraphicsCommandList::Create(ID3D11DeviceContext* immContext)
{
	p_ImmediateContext = immContext;

	ID3D11Device* p_Device;
	p_ImmediateContext->GetDevice(&p_Device);
	p_Device->CreateDeferredContext(0, &context);
}
#elif DX12
void GraphicsCommandList::Create(ID3D12Device* device)
{
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&commandAllocator);
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, pipelineState, _uuidof(ID3D12GraphicsCommandList), (void**)&commandList[1]);
}
#elif GL43

#endif

void GraphicsCommandList::SetupFrame()
{
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	p_ImmediateContext->OMGetRenderTargets(1, &renderTarget, &depthStencil);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);
	
	uint x = 1;
	p_ImmediateContext->RSGetViewports(&x, &viewport);
	context->RSSetViewports(1, &viewport);

	ID3D11RasterizerState* rs;
	p_ImmediateContext->RSGetState(&rs);
	context->RSSetState(rs);
}

void GraphicsCommandList::Finish()
{
	context->FinishCommandList(FALSE, &commandList);
}

void GraphicsCommandList::Execute()
{
	p_ImmediateContext->ExecuteCommandList(commandList, TRUE);
}

ID3D11DeviceContext* GraphicsCommandList::GetDeferredContext()const
{
	return context;
}

NS_END