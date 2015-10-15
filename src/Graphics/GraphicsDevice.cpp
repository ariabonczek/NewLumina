//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#include <Graphics\GraphicsDevice.hpp>

#include <Graphics\Window.hpp>
#include <d3dcompiler.h>

#include <Utility\ResourceManager.hpp>

NS_BEGIN

GraphicsDevice::GraphicsDevice()
{
#if DX11
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
#elif DX12
	ZeroMemory(&viewport, sizeof(D3D12_VIEWPORT));
#elif GL43
#endif
}

GraphicsDevice::~GraphicsDevice()
{
#if DX11
	for (uint32 i = 0; i < NUM_BUFFERS; ++i)
	{
		DELETECOM(displayBuffers[i].depthStencilView);
		DELETECOM(displayBuffers[i].depthBuffer);
		DELETECOM(displayBuffers[i].renderTargetView);
	}
	DELETECOM(swapChain);
	if (immCon)
		immCon->ClearState();
	DELETECOM(immCon);
	DELETECOM(dev);

#elif DX12
	DELETECOM(dev);
	DELETECOM(commandQueue);
	DELETECOM(swapChain);
	DELETECOM(rtvHeap);
	for (uint8 i = 0; i < NUM_BUFFERS; ++i)
	{
		DELETECOM(renderTarget[i]);
	}
	DELETECOM(commandAllocator[0]);
	DELETECOM(commandAllocator[1]);
	DELETECOM(rootSignature);
	DELETECOM(pipelineState);
	DELETECOM(commandList[0]);
	DELETECOM(commandList[1]);
#elif GL43

#endif
}

void GraphicsDevice::Initialize(Window* window)
{
#if _DEBUG
	Debug::Log("GraphicsDevice Initialized");
#endif
	this->window = window;
	

#if DX11 
	HRESULT hr;

	IDXGIFactory4* factory;
	CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)&factory);

	hr = D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
#ifdef DEBUG
		D3D11_CREATE_DEVICE_DEBUG,
#else
		NULL,
#endif
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&dev,
		NULL,
		&immCon
		);

	ResourceManager::GetInstance().SetDevice(dev);

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = NUM_BUFFERS;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width = window->GetWidth();
	scd.BufferDesc.Height = window->GetHeight();
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = window->GetHandle();
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.SampleDesc.Count = 4;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	factory->CreateSwapChain(dev, &scd, (IDXGISwapChain**)&swapChain);

	DELETECOM(factory);

	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;
	rd.DepthClipEnable = true;

	dev->CreateRasterizerState(&rd, &rasterizerState);
	immCon->RSSetState(rasterizerState);

	OnResize();
#elif DX12

#ifdef _DEBUG
	HRESULT hr;

	IDXGIFactory4* factory;
	CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)&factory);

	// Enable the D3D12 debug layer.
	{
		ID3D12Debug* debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)&debugController)))
		{
			debugController->EnableDebugLayer();
		}
	}
#endif
	//
	// Create the Device
	//

		// Grab the hardware Adapter
	IDXGIAdapter1* hardwareAdapter;

	for (uint32 adapterIndex = 0; factory->EnumAdapters1(adapterIndex, &hardwareAdapter); ++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 desc;
		hardwareAdapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// SOFTWARE BAD
			continue;
		}

		// Check if a device can be created, but don't create it yet
		if (SUCCEEDED(D3D12CreateDevice(hardwareAdapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)))
		{
			// HARDWARE GOOD
			break;
		}
	}
	
	hr = D3D12CreateDevice(hardwareAdapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**)&dev);

	ResourceManager::GetInstance().SetDevice(dev);

	//
	// Command Queue
	//
	D3D12_COMMAND_QUEUE_DESC qd;
	ZeroMemory(&qd, sizeof(D3D12_COMMAND_QUEUE_DESC));
	qd.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	qd.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	hr = dev->CreateCommandQueue(&qd, __uuidof(ID3D12CommandQueue), (void**)&commandQueue);

	//
	// Swap Chain
	//
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = NUM_BUFFERS;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width = window->GetWidth();
	scd.BufferDesc.Height = window->GetHeight();
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = window->GetHandle();
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	scd.SampleDesc.Count = 1;
	
	hr = factory->CreateSwapChain(commandQueue, &scd, (IDXGISwapChain**)&swapChain);

	DELETECOM(factory);

	frameIndex = swapChain->GetCurrentBackBufferIndex();

	OnResize();

	//
	// Root Simgnature
	//
	D3D12_ROOT_SIGNATURE_DESC rsd;
	ZeroMemory(&rsd, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	rsd.NumParameters = 0;
	rsd.pParameters = nullptr;
	rsd.NumStaticSamplers = 0;
	rsd.pStaticSamplers = nullptr;
	rsd.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* signature;
	ID3DBlob* error;
	D3D12SerializeRootSignature(&rsd, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
	dev->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), __uuidof(ID3D12RootSignature),(void**) &rootSignature);

	//
	// Pipeline State
	//
	ID3DBlob* vShader;
	ID3DBlob* pShader;

	//hr = D3DCompileFromFile(L"Shaders/DirectX/default.hlsl", nullptr, nullptr, "vert", "vs_5_0", 0, 0, &vShader, 0);
	//hr = D3DCompileFromFile(L"Shaders/DirectX/default.hlsl", nullptr, nullptr, "pix", "ps_5_0", 0, 0, &pShader, 0);

	D3DReadFileToBlob(L"Shaders/DirectX/defaultVertex.cso", &vShader);
	D3DReadFileToBlob(L"Shaders/DirectX/defaultPixel.cso", &pShader);

	D3D12_INPUT_ELEMENT_DESC elementDesc[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsd;
	ZeroMemory(&gpsd, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	gpsd.InputLayout = { elementDesc, _countof(elementDesc) };
	gpsd.pRootSignature = rootSignature;
	gpsd.VS = { reinterpret_cast<uint8*>(vShader->GetBufferPointer()), vShader->GetBufferSize() };
	gpsd.PS = { reinterpret_cast<uint8*>(pShader->GetBufferPointer()), pShader->GetBufferSize() };
	gpsd.RasterizerState = { D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, FALSE, D3D12_DEFAULT_DEPTH_BIAS,
		D3D12_DEFAULT_DEPTH_BIAS_CLAMP, D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS, TRUE, FALSE, FALSE, 0,
		D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF };
	gpsd.BlendState = { FALSE, FALSE };

	const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
	{
		FALSE,FALSE,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_LOGIC_OP_NOOP,
		D3D12_COLOR_WRITE_ENABLE_ALL,
	};
	for (uint8 i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
	{
		gpsd.BlendState.RenderTarget[i] = defaultRenderTargetBlendDesc;
	}
	gpsd.DepthStencilState.DepthEnable = FALSE;
	gpsd.DepthStencilState.StencilEnable = FALSE;
	gpsd.SampleMask = UINT_MAX;
	gpsd.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpsd.NumRenderTargets = 1;
	gpsd.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpsd.SampleDesc.Count = 1;

	hr = dev->CreateGraphicsPipelineState(&gpsd, __uuidof(ID3D12PipelineState),(void**) &pipelineState);

	hr = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator[0], pipelineState, _uuidof(ID3D12GraphicsCommandList), (void**)&commandList[0]);
	hr = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator[1], pipelineState, _uuidof(ID3D12GraphicsCommandList), (void**)&commandList[1]);
	commandList[0]->Close();
	commandList[1]->Close();
	commandIndex = 0;
	executeIndex = 1;

	dev->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&fence);
	fenceValue = 1;

	fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
#elif GL43
	glfwSwapInterval(1);

	// Depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Back-face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	// MSAA
	glEnable(GL_MULTISAMPLE);

	glClearColor(0.392f, 0.584f, 0.929f, 1.0f);

	glewExperimental = true;
	glewInit();
#endif
}

void GraphicsDevice::OnResize()
{
#if DX11
	swapChain->ResizeBuffers(
		NUM_BUFFERS,
		window->GetWidth(),
		window->GetHeight(),
		DXGI_FORMAT_R8G8B8A8_UNORM,
		NULL);

	for (uint32 i = 0; i < NUM_BUFFERS; ++i)
	{
		DELETECOM(displayBuffers[i].renderTargetView);
		DELETECOM(displayBuffers[i].depthStencilView);
		DELETECOM(displayBuffers[i].depthBuffer);

		ID3D11Texture2D* backBuffer;
		swapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
		dev->CreateRenderTargetView(backBuffer, 0, &displayBuffers[i].renderTargetView);
		DELETECOM(backBuffer);

		D3D11_TEXTURE2D_DESC dsd;
		ZeroMemory(&dsd, sizeof(D3D11_TEXTURE2D_DESC));
		dsd.Width = window->GetWidth();
		dsd.Height = window->GetHeight();
		dsd.MipLevels = 1;
		dsd.ArraySize = 1;
		dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsd.Usage = D3D11_USAGE_DEFAULT;
		dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		dsd.CPUAccessFlags = NULL;
		dsd.MiscFlags = NULL;
		dsd.SampleDesc.Count = 4;

		HRESULT hr = dev->CreateTexture2D(&dsd, NULL, &displayBuffers[i].depthBuffer);
		hr = dev->CreateDepthStencilView(displayBuffers[i].depthBuffer, NULL, &displayBuffers[i].depthStencilView);
	}

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)window->GetWidth();
	viewport.Height = (float)window->GetHeight();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
#elif DX12
	for (uint8 i = 0; i < NUM_BUFFERS; ++i)
	{
		DELETECOM(renderTarget[i]);
	}

	swapChain->ResizeBuffers(NUM_BUFFERS, window->GetWidth(), window->GetHeight(), DXGI_FORMAT_R8G8B8A8_UNORM, NULL);

	// Create descriptor heap
	D3D12_DESCRIPTOR_HEAP_DESC dhd;
	ZeroMemory(&dhd, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	dhd.NumDescriptors = NUM_BUFFERS;
	dhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	dhd.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dev->CreateDescriptorHeap(&dhd, __uuidof(ID3D12DescriptorHeap), (void**)&rtvHeap);

	descriptorSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart());

	for (uint16 i = 0; i < NUM_BUFFERS; ++i)
	{
		// Create frame resources
		swapChain->GetBuffer(i, __uuidof(ID3D12Resource), (void**)&renderTarget[i]);
		dev->CreateRenderTargetView(renderTarget[i], nullptr, rtvHandle);
		rtvHandle.ptr += descriptorSize;
	}
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)window->GetWidth();
	viewport.Height = (float)window->GetHeight();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
#elif GL43

#endif
}

void GraphicsDevice::Shutdown()
{

}

//void GraphicsDevice::ResetCommandList()
//{
//#if DX11
//	defCon[contextIndex]->RSSetViewports(1, &viewport);
//	defCon[contextIndex]->OMSetRenderTargets(1, &displayBuffers[frameIndex].renderTargetView, displayBuffers[frameIndex].depthStencilView);
//
//	const float color[4] = { 0.392f, 0.584f, 0.929f, 1.0f };
//	defCon[contextIndex]->ClearRenderTargetView(displayBuffers[frameIndex].renderTargetView, color);
//#elif DX12
//	commandAllocator[commandIndex]->Reset();
//	commandList[commandIndex]->Reset(commandAllocator[commandIndex], pipelineState);
//	commandList[commandIndex]->SetGraphicsRootSignature(rootSignature);
//	commandList[commandIndex]->RSSetViewports(1, &viewport);
//
//	// Use to render
//	D3D12_RESOURCE_BARRIER rb;
//	ZeroMemory(&rb, sizeof(D3D12_RESOURCE_BARRIER));
//	rb.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
//	rb.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
//	rb.Transition.pResource = renderTarget[(frameIndex + 1) % NUM_BUFFERS];
//	rb.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
//	rb.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
//	rb.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
//
//	commandList[commandIndex]->ResourceBarrier(1, &rb);
//
//	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart());
//	rtvHandle.ptr += (frameIndex + 1) % NUM_BUFFERS * descriptorSize;
//	commandList[commandIndex]->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
//
//	const float color[4] = { 0.392f, 0.584f, 0.929f, 1.0f };
//	commandList[commandIndex]->ClearRenderTargetView(rtvHandle, color, 0, NULL);
//#elif GL43
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//#endif
//}
//
//void GraphicsDevice::CloseCommandList()
//{
//#if DX11
//	defCon[contextIndex]->FinishCommandList(FALSE, &commandList);
//	contextIndex ^= 1;
//#elif DX12
//	D3D12_RESOURCE_BARRIER rb;
//	ZeroMemory(&rb, sizeof(D3D12_RESOURCE_BARRIER));
//	rb.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
//	rb.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
//	rb.Transition.pResource = renderTarget[(frameIndex + 1) % NUM_BUFFERS];
//	rb.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
//	rb.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
//	commandList[commandIndex]->ResourceBarrier(1, &rb);
//	HRESULT hr = commandList[commandIndex]->Close();
//#elif GL43
//	glfwSwapBuffers(window->GetHandle());
//#endif
//}
//
//void GraphicsDevice::ExecuteCommandList()
//{
//#if DX11
//	immCon->ExecuteCommandList(commandList, TRUE);
//	swapChain->Present(VSYNC_ENABLED, 0);
//	frameIndex = swapChain->GetCurrentBackBufferIndex();
//#elif DX12
//	ID3D12CommandList* pCommandLists[] = { commandList[executeIndex] };
//
//	commandQueue->ExecuteCommandLists(_countof(pCommandLists), pCommandLists);
//	swapChain->Present(VSYNC_ENABLED, 0);
//
//	// Synchronize
//	const uint64 _fence = fenceValue;
//	commandQueue->Signal(fence, _fence);
//	fenceValue++;
//
//	if (fence->GetCompletedValue() < _fence)
//	{
//		fence->SetEventOnCompletion(_fence, fenceEvent);
//		WaitForSingleObject(fenceEvent, INFINITE);
//	}
//
//	// Swap the indices
//	frameIndex = swapChain->GetCurrentBackBufferIndex();
//	std::swap(commandIndex, executeIndex);
//#elif GL43
//	// WARNING: DO NOT MAKE GL CALLS HERE AS THE CONTEXT ONLY SUPPORTS ONE THREAD
//#endif
//}

NS_END