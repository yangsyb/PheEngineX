#include "pch.h"
#include "PRenderThread.h"
#include "DxException.h"
namespace Phe
{
	PRenderThread* PRenderThread::pRenderThread = nullptr;

	void PRenderThread::CreateRenderThread()
	{
		pRenderThread = new PRenderThread();
		pRenderThread->Start();
	}

	void PRenderThread::DestroyRenderThread()
	{
		pRenderThread->Stop();
	}

	PRenderThread* PRenderThread::Get()
	{
		return pRenderThread;
	}

	PRenderThread::PRenderThread() : PRenderNum(0), RenderFrameIndex(0), NextFrameIndex(0)
	{
		Initialize();
		OnResize();
		PGraphicContext = std::make_unique<GraphicContext>(PDevice.Get(), PCommandList.Get(), PCommandAllocator.Get(), PCommandQueue.Get(), PFence.Get(), PCbvSrvUavDescriptorSize);
		
	}

	PRenderThread::~PRenderThread()
	{

	}

	void PRenderThread::Start()
	{
		PRScene = new PRenderScene();
		if (IsRunning)
		{
			return;
		}

		IsRunning = true;

		pThread = new std::thread(PThread::ThreadExecute, this);
	}

	void PRenderThread::Stop()
	{
		if (pThread)
		{

			std::unique_lock<std::mutex> RenderLock(Rendermtx);
			++PRenderNum;
			NextFrameIndex = (NextFrameIndex + 1) % 2;
			IsRunning = false;
			RenderCV.notify_all();
			RenderLock.unlock();

			pThread->join();
			ComPtr<IDXGIDebug1> dxgiDebug;

			delete PRScene;
			PRScene = nullptr;
			delete pThread;
			pThread = nullptr;
			delete pRenderThread;
			pRenderThread = nullptr;
		}
	}

	void PRenderThread::Run()
	{
		std::unique_lock<std::mutex> RenderLock(Rendermtx);
		PRenderNum--;
		RenderCV.wait(RenderLock);
		DoTasks();
		Render();
	}

	void PRenderThread::AddTask(PTask* RenderTask)
	{
		if (IsRunning)
		{
			RenderFrame[NextFrameIndex].PTaskQueue.push(RenderTask);
		}
	}

	void PRenderThread::DoTasks()
	{
		PTask* CurrentTask = nullptr;
		while (!RenderFrame[RenderFrameIndex].PTaskQueue.empty())
		{
			CurrentTask = RenderFrame[RenderFrameIndex].PTaskQueue.front();
			CurrentTask->Execute();
			RenderFrame[RenderFrameIndex].PTaskQueue.pop();
			CurrentTask = nullptr;

		}
		RenderFrameIndex = (RenderFrameIndex + 1) % 2;
	}

	void PRenderThread::TriggerRender()
	{
		std::unique_lock<std::mutex> RenderLock(Rendermtx);
		++PRenderNum;
		NextFrameIndex = (NextFrameIndex + 1) % 2;
		RenderCV.notify_one();
	}

	void PRenderThread::Initialize()
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

		DXGI_SWAP_CHAIN_DESC sd;
		sd.BufferDesc.Width = PWidth;
		sd.BufferDesc.Height = PHeight;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = SwapChainBufferCount;
		sd.OutputWindow = GetActiveWindow();
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

	void PRenderThread::Render()
	{
		PCommandAllocator->Reset();

		ThrowIfFailed(PCommandList->Reset(PCommandAllocator.Get(), nullptr));

		PCommandList->RSSetViewports(1, &PViewport);
		PCommandList->RSSetScissorRects(1, &PScissorRect);

		auto PresourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
			D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		PCommandList->ResourceBarrier(1, &PresourceBarrier);

		PCommandList->ClearRenderTargetView(CurrentBackBufferView(), RTVColor, 0, nullptr);
		PCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

		auto Prtv = CurrentBackBufferView();
		auto Pdsv = DepthStencilView();
		PCommandList->OMSetRenderTargets(1, &Prtv, true, &Pdsv);

		PRScene->Render();

		PresourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		PCommandList->ResourceBarrier(1, &PresourceBarrier);

		ThrowIfFailed(PCommandList->Close());

		ID3D12CommandList* ppCommandLists[] = { PCommandList.Get() };
		PCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		ThrowIfFailed(PSwapChain->Present(0, 0));
		PCurrBackBuffer = (PCurrBackBuffer + 1) % SwapChainBufferCount;

		FlushCommandQueue();
	}

	void PRenderThread::OnResize()
	{
		FlushCommandQueue();

		ThrowIfFailed(PCommandList->Reset(PCommandAllocator.Get(), nullptr));

		for (int i = 0; i < SwapChainBufferCount; ++i)
			PSwapChainBuffer[i].Reset();
		PDepthStencilBuffer.Reset();

		ThrowIfFailed(PSwapChain->ResizeBuffers(
			SwapChainBufferCount,
			PWidth, PHeight,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

		PCurrBackBuffer = 0;


		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(PRtvHeap->GetCPUDescriptorHandleForHeapStart());
		for (UINT i = 0; i < SwapChainBufferCount; i++)
		{
			ThrowIfFailed(PSwapChain->GetBuffer(i, IID_PPV_ARGS(&PSwapChainBuffer[i])));
			PDevice->CreateRenderTargetView(PSwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
			rtvHeapHandle.Offset(1, PRtvDescriptorSize);
		}


		D3D12_RESOURCE_DESC depthStencilDesc;
		depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		depthStencilDesc.Alignment = 0;
		depthStencilDesc.Width = PWidth;
		depthStencilDesc.Height = PHeight;
		depthStencilDesc.DepthOrArraySize = 1;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE optClear;
		optClear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		optClear.DepthStencil.Depth = 1.0f;
		optClear.DepthStencil.Stencil = 0;

		auto PheapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		ThrowIfFailed(PDevice->CreateCommittedResource(&PheapProperty, D3D12_HEAP_FLAG_NONE, &depthStencilDesc,
			D3D12_RESOURCE_STATE_COMMON, &optClear, IID_PPV_ARGS(PDepthStencilBuffer.GetAddressOf())));


		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.Texture2D.MipSlice = 0;
		PDevice->CreateDepthStencilView(PDepthStencilBuffer.Get(), &dsvDesc, PDsvHeap->GetCPUDescriptorHandleForHeapStart());

		auto PresourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(PDepthStencilBuffer.Get(),
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
		PCommandList->ResourceBarrier(1, &PresourceBarrier);


		ThrowIfFailed(PCommandList->Close());
		ID3D12CommandList* cmdsLists[] = { PCommandList.Get() };
		PCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

		FlushCommandQueue();

		PViewport.TopLeftX = 0;
		PViewport.TopLeftY = 0;
		PViewport.Width = static_cast<float>(PWidth);
		PViewport.Height = static_cast<float>(PHeight);
		PViewport.MinDepth = 0.0f;
		PViewport.MaxDepth = 1.0f;

		PScissorRect = CD3DX12_RECT(0, 0, PWidth, PHeight);
	}

	ID3D12Resource* PRenderThread::CurrentBackBuffer() const
	{
		return PSwapChainBuffer[PCurrBackBuffer].Get();
	}

	D3D12_CPU_DESCRIPTOR_HANDLE PRenderThread::CurrentBackBufferView() const
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(
			PRtvHeap->GetCPUDescriptorHandleForHeapStart(),
			PCurrBackBuffer,
			PRtvDescriptorSize);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE PRenderThread::DepthStencilView() const
	{
		return PDsvHeap->GetCPUDescriptorHandleForHeapStart();
	}

	void PRenderThread::FlushCommandQueue()
	{
		PFenceValue++;

		ThrowIfFailed(PCommandQueue->Signal(PFence.Get(), PFenceValue));

		if (PFence->GetCompletedValue() < PFenceValue)
		{
			HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);

			ThrowIfFailed(PFence->SetEventOnCompletion(PFenceValue, eventHandle));

			WaitForSingleObject(eventHandle, INFINITE);
			CloseHandle(eventHandle);
		}
	}

}