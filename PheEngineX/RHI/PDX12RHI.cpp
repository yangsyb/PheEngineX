#include "pch.h"
#include "PDX12RHI.h"
#include "DxException.h"
#include "PEngine.h"

namespace Phe
{
	PDX12RHI::PDX12RHI() : PRHI(), PCbvSrvUavDescriptorSize(0), PDsvDescriptorSize(0), PFenceEvent(HANDLE()), PFenceValue(0), PRtvDescriptorSize(0)
	{

	}

	void PDX12RHI::InitRHI()
	{
		ComPtr<ID3D12Debug> DebugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&DebugController))))
		{
			DebugController->EnableDebugLayer();
		}
		ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiInfoQueue.GetAddressOf()))))
		{
			dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
			dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
		}

		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&PDXGIFactory)));

		ThrowIfFailed(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&PDevice)));

		ThrowIfFailed(PDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&PFence)));

		PRtvDescriptorSize = PDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		PDsvDescriptorSize = PDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		PCbvSrvUavDescriptorSize = PDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		//CreateCommandObjects
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		ThrowIfFailed(PDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(PCommandQueue.GetAddressOf())));

		ThrowIfFailed(PDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(PCommandAllocator.GetAddressOf())));

		ThrowIfFailed(PDevice->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			PCommandAllocator.Get(),
			nullptr,
			IID_PPV_ARGS(PCommandList.GetAddressOf())));

		PCommandList->Close();

		//CreateSwapChain
		PSwapChain.Reset();
		PWindowWin32* CurrentWindow = dynamic_cast<PWindowWin32*>(PEngine::GetSingleton().GetWindow());

		DXGI_SWAP_CHAIN_DESC sd;
		sd.BufferDesc.Width = CurrentWindow->GetWidth();
		sd.BufferDesc.Height = CurrentWindow->GetHeight();
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = SwapChainBufferCount;
		//		sd.OutputWindow = GetActiveWindow();
		sd.OutputWindow = CurrentWindow->GetCurrentHwnd();
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		ThrowIfFailed(PDXGIFactory->CreateSwapChain(
			PCommandQueue.Get(),
			&sd,
			PSwapChain.GetAddressOf()));

		//CreateRtvAndDsvDescriptorHeaps
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
		rtvHeapDesc.NumDescriptors = SwapChainBufferCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvHeapDesc.NodeMask = 0;
		ThrowIfFailed(PDevice->CreateDescriptorHeap(
			&rtvHeapDesc, IID_PPV_ARGS(PRtvHeap.GetAddressOf())));


		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
		dsvHeapDesc.NumDescriptors = 1;
		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		dsvHeapDesc.NodeMask = 0;
		ThrowIfFailed(PDevice->CreateDescriptorHeap(
			&dsvHeapDesc, IID_PPV_ARGS(PDsvHeap.GetAddressOf())));
	}

	void PDX12RHI::BeginFrame()
	{

	}

	void PDX12RHI::EndFrame()
	{

	}

	void PDX12RHI::Flush()
	{
		PFenceValue++;

		ThrowIfFailed(PCommandQueue->Signal(PFence.Get(), PFenceValue));

		if (PFence->GetCompletedValue() < PFenceValue)
		{
			HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);

			ThrowIfFailed(PFence->SetEventOnCompletion(PFenceValue, eventHandle));

			if (eventHandle)
			{
				WaitForSingleObject(eventHandle, INFINITE);
				CloseHandle(eventHandle);
			}
		}
	}

	void PDX12RHI::InitGraphicsPipeline()
	{
		//CBVSRVUAV HEAP
		PCbvSrvUavDescriptorSize = GraphicContext::GetSingleton().Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		D3D12_DESCRIPTOR_HEAP_DESC cbHeapDesc;
		cbHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		cbHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbHeapDesc.NumDescriptors = 4096;
		cbHeapDesc.NodeMask = 0;
		GraphicContext::GetSingleton().Device()->CreateDescriptorHeap(&cbHeapDesc, IID_PPV_ARGS(&CbvHeap));

		PerObjectCBufferByteSize = (sizeof(PerObjectCBuffer) + 255) & ~255;
		PerCameraCBufferByteSize = (sizeof(PerCameraCBuffer) + 255) & ~255;
		PerFrameCBufferByteSize = (sizeof(PerFrameCBuffer) + 255) & ~255;
		PerMaterialCBufferByteSize = (sizeof(PerMaterialCBuffer) + 255) & ~255;

		PerCameraCB = std::make_unique<UploadBuffer<PerCameraCBuffer>>(GraphicContext::GetSingleton().Device().Get(), 1, true);
	}

	void PDX12RHI::AddRenderResourceMesh()
	{

	}

}