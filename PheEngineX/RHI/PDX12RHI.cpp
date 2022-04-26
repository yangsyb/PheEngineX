#include "pch.h"
#include "PDX12RHI.h"
#include "Engine/Core/PEngine.h"
#include "DX12/PDX12GPUMeshBuffer.h"
#include "DX12/PDX12Shader.h"
#include "DX12/PDX12Pipeline.h"
#include "DX12/PDX12GPUCommonBuffer.h"
#include "DX12/PDX12GPUTexture.h"
#include "DX12/DDSTextureLoader.h"
#include "DX12/DDSTextureSaver.h"
#include "DX12/PDX12GPURenderTarget.h"

namespace Phe
{
	PDX12RHI::PDX12RHI() : PRHI(), PCbvSrvUavDescriptorSize(0), PDsvDescriptorSize(0), PFenceEvent(HANDLE()), PFenceValue(0), PRtvDescriptorSize(0), DX12ShaderManager(nullptr)
	{

	}



	PDX12RHI::~PDX12RHI()
	{
		// 		for (int index = 0; index < PSwapChainBuffer.size(); index++)
		// 		{
		// 			ReleasePtr(PSwapChainBuffer[index]);
		// 		}
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
		sd.BufferDesc.RefreshRate.Numerator = 70;
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
		PRtvHeap = std::make_unique<PDescriptorHeap>(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, PDevice);
		PDsvHeap = std::make_unique<PDescriptorHeap>(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, PDevice);

		// 		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
		// 		rtvHeapDesc.NumDescriptors = SwapChainBufferCount;
		// 		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		// 		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		// 		rtvHeapDesc.NodeMask = 0;
		// 		ThrowIfFailed(PDevice->CreateDescriptorHeap(
		// 			&rtvHeapDesc, IID_PPV_ARGS(PRtvHeap.GetAddressOf())));
		// 
		// 
		// 		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
		// 		dsvHeapDesc.NumDescriptors = 1;
		// 		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		// 		dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		// 		dsvHeapDesc.NodeMask = 0;
		// 		ThrowIfFailed(PDevice->CreateDescriptorHeap(
		// 			&dsvHeapDesc, IID_PPV_ARGS(PDsvHeap.GetAddressOf())));

		ScreenRT = CreateRenderTarget("BackBuffer");
		ScreenRT->AddColorBuffer(ScreenFrameNumber, P_TextureFormat::P_FORMAT_R8G8B8A8_UNORM);
		ScreenRT->AddDepthStencilBuffer(P_TextureFormat::P_FORMAT_D24_UNORM_S8_UINT);
		DX12ShaderManager = dynamic_cast<PDX12Shadermanager*>(PShaderManager::Get());
	}

	void PDX12RHI::BeginFrame()
	{
		PCommandAllocator->Reset();
		ResetCommandList();
		PrepareBufferHeap();
	}

	void PDX12RHI::EndFrame()
	{
		ExecuteCommandList();
//		Flush();
		ThrowIfFailed(PSwapChain->Present(1, 0));

		PCurrBackBuffer = (PCurrBackBuffer + 1) % SwapChainBufferCount;

		Flush();
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
		PCbvSrvUavHeap = std::make_unique<PDescriptorHeap>(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, PDevice);
	}



	void PDX12RHI::ResizeWindow(UINT32 PWidth, UINT32 PHeight)
	{
		Flush();

		ResetCommandList();

		for (int i = 0; i < SwapChainBufferCount; ++i)
		{
			DX12RTBuffer* RTBuffer = static_cast<DX12RTBuffer*>(ScreenRT->GetColorBuffer().at(i));
			RTBuffer->PResource->ResetResource();
			PRHI::Get()->ResetRTBuffer(RTBuffer);
		}
		DX12RTBuffer* DSBuffer = static_cast<DX12RTBuffer*>(ScreenRT->GetDepthStencilBuffer());
		DSBuffer->PResource->ResetResource();
		PRHI::Get()->ResetRTBuffer(DSBuffer);

		ThrowIfFailed(PSwapChain->ResizeBuffers(
			SwapChainBufferCount,
			PWidth, PHeight,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

		PCurrBackBuffer = 0;

		for (UINT i = 0; i < SwapChainBufferCount; i++)
		{
			DX12RTBuffer* RTBuffer = static_cast<DX12RTBuffer*>(ScreenRT->GetColorBuffer().at(i));
			ComPtr<ID3D12Resource> Res;
			ThrowIfFailed(PSwapChain->GetBuffer(i, IID_PPV_ARGS(&Res)));
			RTBuffer->PResource->SetResource(Res);
			auto RtvAllocation = PRtvHeap->Allocate(1);
			RTBuffer->PHandleOffset = RtvAllocation.second;
			PDevice->CreateRenderTargetView(RTBuffer->PResource->GetResource().Get(), nullptr, RtvAllocation.first);
		}

		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.Texture2D.MipSlice = 0;

		auto DsvAllocation = PDsvHeap->Allocate(1);
		DSBuffer->PHandleOffset = DsvAllocation.second;
		PDevice->CreateDepthStencilView(DSBuffer->PResource->GetResource().Get(), &dsvDesc, DsvAllocation.first);

		// 		auto PresourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(DSBuffer->PResource->GetResource().Get(),
		// 			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
		auto PresourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(DSBuffer->PResource->GetResource().Get(),
			D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);
		PCommandList->ResourceBarrier(1, &PresourceBarrier);


		ExecuteCommandList();

		Flush();

		PViewport.TopLeftX = 0;
		PViewport.TopLeftY = 0;
		PViewport.Width = static_cast<float>(PWidth);
		PViewport.Height = static_cast<float>(PHeight);
		PViewport.MinDepth = 0.0f;
		PViewport.MaxDepth = 1.0f;

		PScissorRect = CD3DX12_RECT(0, 0, PWidth, PHeight);
	}

	PGPUMeshBuffer* PDX12RHI::CreateMeshBuffer()
	{
		return new PDX12GPUMeshBuffer();
	}

	void PDX12RHI::UpdateMeshBuffer(PGPUMeshBuffer* GpuMeshBuffer)
	{
		ResetCommandList();
		PDX12GPUMeshBuffer* DX12GpuMeshBuffer = static_cast<PDX12GPUMeshBuffer*>(GpuMeshBuffer);

		// 		UINT32 VertexBufferSize = GpuMeshBuffer->GetVertexBufferByteSize();
		// 		UINT32 IndexBufferSize = sizeof(UINT16);
		// 		UINT32 VBSize = VertexBufferSize* GpuMeshBuffer->GetVertexByteStride();
		// 		UINT32 IBSize = IndexBufferSize* GpuMeshBuffer->GetIndexCount();
		// 
		// 		auto VDefaultHeapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		// 		auto IDefaultHeapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		// 		auto VResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(VBSize);
		// 		auto IResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(IBSize);
		// 
		// 		DX12GpuMeshBuffer->VertexBuffer.CreateResource(PDevice.Get(), &VDefaultHeapProperty, D3D12_HEAP_FLAG_NONE, &VResourceDesc, D3D12_RESOURCE_STATE_COMMON);
		// 		DX12GpuMeshBuffer->IndexBuffer.CreateResource(PDevice.Get(), &IDefaultHeapProperty, D3D12_HEAP_FLAG_NONE, &IResourceDesc, D3D12_RESOURCE_STATE_COMMON);
		// 
		// 		auto VUploadHeapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		// 		auto IUploadHeapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		// 		PDevice->CreateCommittedResource(&VUploadHeapProperty, D3D12_HEAP_FLAG_NONE, &VResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&VertexBufferUpload));
		// 		PDevice->CreateCommittedResource(&IUploadHeapProperty, D3D12_HEAP_FLAG_NONE, &IResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&IndexBufferUpload));
		// 
		// 		char* Vertexbuffer = new char[VBSize];
		// 		char* Indexbuffer = new char[IBSize];
		// 		std::unique_ptr<char> VdataPtrGuard(Vertexbuffer);
		// 		std::unique_ptr<char> IdataPtrGuard(Indexbuffer);
		// 
		// 		UINT offset = 0;
		// 		auto vertBufferCopy = [&](char* buffer, const char* ptr, UINT size, UINT& offset) -> void
		// 		{
		// 			if (ptr)
		// 			{
		// 				for (UINT i = 0; i < GpuMeshBuffer->GetVertexCount(); ++i)
		// 				{
		// 					memcpy(buffer + size_t(i) * sizeof(PVertex) + offset, ptr + size_t(size) * i, size);
		// 				}
		// 				offset += size;
		// 			}
		// 		};
		// 		vertBufferCopy(Vertexbuffer, reinterpret_cast<const char*>(GpuMeshBuffer->GetVertexVector().data()), sizeof(PVertex), offset);
		// 
		// 		D3D12_SUBRESOURCE_DATA ResourceData = {};
		// 		ResourceData.pData = Vertexbuffer;
		// 		ResourceData.RowPitch = VertexBufferSize;
		// 		ResourceData.SlicePitch = ResourceData.RowPitch;
		// 
		// 		auto Resourcebarrier = CD3DX12_RESOURCE_BARRIER::Transition(DX12GpuMeshBuffer->VertexBuffer.GetResource().Get(),
		// 			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
		// 		PCommandList->ResourceBarrier(1, &Resourcebarrier);
		// 		UpdateSubresources<1>(PCommandList.Get(), DX12GpuMeshBuffer->VertexBuffer.GetResource().Get(), VertexBufferUpload.Get(), 0, 0, 1, &ResourceData);
		// 		Resourcebarrier = CD3DX12_RESOURCE_BARRIER::Transition(DX12GpuMeshBuffer->VertexBuffer.GetResource().Get(),
		// 			D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
		// 		PCommandList->ResourceBarrier(1, &Resourcebarrier);
		// 		DX12GpuMeshBuffer->VertexBufferView.BufferLocation = DX12GpuMeshBuffer->VertexBuffer.GetResource().Get()->GetGPUVirtualAddress();
		// 		DX12GpuMeshBuffer->VertexBufferView.StrideInBytes = DX12GpuMeshBuffer->GetVertexByteStride();
		// 		DX12GpuMeshBuffer->VertexBufferView.SizeInBytes = DX12GpuMeshBuffer->GetVertexBufferByteSize();
		// 		//////////////////////////////////////////////////////////////////////////
		// 
		// 		offset = 0;
		// 		auto IndBufferCopy = [&](char* buffer, const char* ptr, UINT size, UINT& offset) -> void
		// 		{
		// 			if (ptr)
		// 			{
		// 				for (UINT i = 0; i < GpuMeshBuffer->GetVertexCount(); ++i)
		// 				{
		// 					memcpy(buffer + size_t(i) * sizeof(UINT16) + offset, ptr + size_t(size) * i, size);
		// 				}
		// 				offset += size;
		// 			}
		// 		};
		// 		IndBufferCopy(Indexbuffer, reinterpret_cast<const char*>(GpuMeshBuffer->GetIndexVector().data()), sizeof(UINT16), offset);
		// 
		// 		D3D12_SUBRESOURCE_DATA IResourceData = {};
		// 		IResourceData.pData = Indexbuffer;
		// 		IResourceData.RowPitch = IndexBufferSize;
		// 		IResourceData.SlicePitch = IResourceData.RowPitch;
		// 
		// 		Resourcebarrier = CD3DX12_RESOURCE_BARRIER::Transition(DX12GpuMeshBuffer->IndexBuffer.GetResource().Get(),
		// 			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
		// 		PCommandList->ResourceBarrier(1, &Resourcebarrier);
		// 		UpdateSubresources<1>(PCommandList.Get(), DX12GpuMeshBuffer->IndexBuffer.GetResource().Get(), IndexBufferUpload.Get(), 0, 0, 1, &IResourceData);
		// 		Resourcebarrier = CD3DX12_RESOURCE_BARRIER::Transition(DX12GpuMeshBuffer->IndexBuffer.GetResource().Get(),
		// 			D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
		// 		PCommandList->ResourceBarrier(1, &Resourcebarrier);
		// 		DX12GpuMeshBuffer->IndexBufferView.BufferLocation = DX12GpuMeshBuffer->IndexBuffer.GetResource().Get()->GetGPUVirtualAddress();
		// 		DX12GpuMeshBuffer->IndexBufferView.Format = DXGI_FORMAT_R16_UINT;
		// 		DX12GpuMeshBuffer->IndexBufferView.SizeInBytes = DX12GpuMeshBuffer->GetVertexBufferByteSize();
		char* buffer = new char[GpuMeshBuffer->GetBufferSize()];
		std::unique_ptr<char> dataPtrGuard(buffer);
		UINT offset = 0;
		auto vertBufferCopy = [&](char* buffer, const char* ptr, UINT size, UINT& offset) -> void
		{
			if (ptr)
			{
				for (UINT i = 0; i < GpuMeshBuffer->GetVertexCount(); ++i)
				{
					memcpy(buffer + size_t(i) * GpuMeshBuffer->GetVertexByteStride() + offset, ptr + size_t(size) * i, size);
				}
				offset += size;
			}
		};
		vertBufferCopy(buffer, reinterpret_cast<const char*>(GpuMeshBuffer->GetVertexVector().data()), sizeof(PVertex), offset);

		char* indexBufferStart = buffer + GpuMeshBuffer->GetVertexBufferByteSize();
		memcpy(indexBufferStart, GpuMeshBuffer->GetIndexVector().data(), GpuMeshBuffer->GetIndexCount() * sizeof(UINT16));

		DX12GpuMeshBuffer->Buffer = CreateDefaultBuffer(buffer, GpuMeshBuffer->GetBufferSize(), DX12GpuMeshBuffer->BufferUpload);

		DX12GpuMeshBuffer->VertexBufferView.BufferLocation = DX12GpuMeshBuffer->Buffer->GetGPUVirtualAddress();
		DX12GpuMeshBuffer->VertexBufferView.StrideInBytes = DX12GpuMeshBuffer->GetVertexByteStride();
		DX12GpuMeshBuffer->VertexBufferView.SizeInBytes = DX12GpuMeshBuffer->GetVertexBufferByteSize();
		DX12GpuMeshBuffer->IndexBufferView.BufferLocation = DX12GpuMeshBuffer->Buffer->GetGPUVirtualAddress() + DX12GpuMeshBuffer->GetVertexBufferByteSize();
		DX12GpuMeshBuffer->IndexBufferView.Format = DXGI_FORMAT_R16_UINT;
		DX12GpuMeshBuffer->IndexBufferView.SizeInBytes = DX12GpuMeshBuffer->GetIndexBufferByteSize();
		ExecuteCommandList();
		Flush();
	}

	void PDX12RHI::BeginRenderBackBuffer()
	{
		PIXBeginEvent(PCommandList.Get(), 0, L"BackBuffer");
		PCommandList->RSSetViewports(1, &PViewport);
		PCommandList->RSSetScissorRects(1, &PScissorRect);

		auto PresourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
			D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		PCommandList->ResourceBarrier(1, &PresourceBarrier);

		PCommandList->ClearRenderTargetView(CurrentBackBufferView(), RTVColor, 0, nullptr);
		PCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

		auto Prtv = CurrentBackBufferView();
		auto Pdsv = DepthStencilView();
		//		PCommandList->OMSetRenderTargets(1, &Prtv, true, &Pdsv);
		PCommandList->OMSetRenderTargets(1, &Prtv, false, &Pdsv);
	}

	void PDX12RHI::EndRenderBackBuffer()
	{
		auto PresourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		PCommandList->ResourceBarrier(1, &PresourceBarrier);
		PIXEndEvent(PCommandList.Get());
// 		ExecuteCommandList();
// 
// 
// 		ThrowIfFailed(PSwapChain->Present(1, 0));
// 
// 		PCurrBackBuffer = (PCurrBackBuffer + 1) % SwapChainBufferCount;
// 
// 		Flush();
	}

	void PDX12RHI::BeginRenderRenderTarget(PGPURenderTarget* RenderTarget, std::wstring PassName)
	{
		PIXBeginEvent(PCommandList.Get(), 0, PassName.c_str());
		if(RenderTarget->GetColorBuffer().size() > 0)
		{
			for(auto ColorBuffer : RenderTarget->GetColorBuffer())
			{
				DX12RTBuffer* InRtBuffer = static_cast<DX12RTBuffer*>(ColorBuffer);
//				CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(InRtBuffer->PResource->GetResource().Get(), D3D12_RESOURCE_STATE_COMMON, InRtBuffer->PType == RTBufferType::RTColorBuffer ? D3D12_RESOURCE_STATE_RENDER_TARGET : D3D12_RESOURCE_STATE_DEPTH_WRITE);
				CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(InRtBuffer->PResource->GetResource().Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_RENDER_TARGET);
				PCommandList->ResourceBarrier(1, &ResourceBarrier);
			}
		}
		if(RenderTarget->GetDepthStencilBuffer())
		{
			DX12RTBuffer* InRtBuffer = static_cast<DX12RTBuffer*>(RenderTarget->GetDepthStencilBuffer());
//			CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(InRtBuffer->PResource->GetResource().Get(), D3D12_RESOURCE_STATE_GENERIC_READ, InRtBuffer->PType == RTBufferType::RTColorBuffer ? D3D12_RESOURCE_STATE_RENDER_TARGET : D3D12_RESOURCE_STATE_DEPTH_WRITE);
			CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(InRtBuffer->PResource->GetResource().Get(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);
			PCommandList->ResourceBarrier(1, &ResourceBarrier);
		}
	}

	void PDX12RHI::EndRenderRenderTarget(PGPURenderTarget* RenderTarget)
	{
		if (RenderTarget->GetColorBuffer().size() > 0)
		{
			for (auto ColorBuffer : RenderTarget->GetColorBuffer())
			{
				DX12RTBuffer* InRtBuffer = static_cast<DX12RTBuffer*>(ColorBuffer);
//				CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(InRtBuffer->PResource->GetResource().Get(), InRtBuffer->PType == RTBufferType::RTColorBuffer ? D3D12_RESOURCE_STATE_RENDER_TARGET : D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
				CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(InRtBuffer->PResource->GetResource().Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COMMON);
				PCommandList->ResourceBarrier(1, &ResourceBarrier);
			}
		}
		if (RenderTarget->GetDepthStencilBuffer())
		{
			DX12RTBuffer* InRtBuffer = static_cast<DX12RTBuffer*>(RenderTarget->GetDepthStencilBuffer());
//			CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(InRtBuffer->PResource->GetResource().Get(), InRtBuffer->PType == RTBufferType::RTColorBuffer ? D3D12_RESOURCE_STATE_RENDER_TARGET : D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
			CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(InRtBuffer->PResource->GetResource().Get(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
			PCommandList->ResourceBarrier(1, &ResourceBarrier);
		}
		PIXEndEvent(PCommandList.Get());
	}

// 	void PDX12RHI::BeginRenderRTBuffer(RTBuffer* RtBuffer)
// 	{
// //		ResetCommandList();
// 		DX12RTBuffer* InRtBuffer = static_cast<DX12RTBuffer*>(RtBuffer);
// 		CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(InRtBuffer->PResource->GetResource().Get(), D3D12_RESOURCE_STATE_GENERIC_READ, InRtBuffer->PType == RTBufferType::RTColorBuffer ? D3D12_RESOURCE_STATE_RENDER_TARGET : D3D12_RESOURCE_STATE_DEPTH_WRITE);
// 		PCommandList->ResourceBarrier(1, &ResourceBarrier);
// 	}
// 
// 	void PDX12RHI::EndRenderRTBuffer(RTBuffer* RtBuffer)
// 	{
// 		DX12RTBuffer* InRtBuffer = static_cast<DX12RTBuffer*>(RtBuffer);
// 		CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(InRtBuffer->PResource->GetResource().Get(), InRtBuffer->PType == RTBufferType::RTColorBuffer ? D3D12_RESOURCE_STATE_RENDER_TARGET : D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
// 		PCommandList->ResourceBarrier(1, &ResourceBarrier);
// //		ExecuteCommandList();
// //		Flush();
// 	}

	void PDX12RHI::SetGraphicsPipeline(PPipeline* Pipeline)
	{
		PDX12Pipeline* InDX12Pipeline = static_cast<PDX12Pipeline*>(Pipeline);
		PCommandList->SetPipelineState(InDX12Pipeline->GetPipelineState().Get());
		PCommandList->SetGraphicsRootSignature(static_cast<PDX12Shader*>(InDX12Pipeline->GetShader())->GetRootSignature().Get());
	}

	void PDX12RHI::SetMeshBuffer(PGPUMeshBuffer* InMeshBuffer)
	{
		PDX12GPUMeshBuffer* InDX12MeshBuffer = static_cast<PDX12GPUMeshBuffer*>(InMeshBuffer);
		PCommandList->IASetVertexBuffers(0, 1, &InDX12MeshBuffer->VertexBufferView);
		PCommandList->IASetIndexBuffer(&InDX12MeshBuffer->IndexBufferView);
		PCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}


	void PDX12RHI::DrawInstanced(UINT DrawIndexCount)
	{
		PCommandList->DrawInstanced(DrawIndexCount, 1, 0, 0);
	}

	void PDX12RHI::DrawPrimitiveIndexedInstanced(UINT DrawIndexCount)
	{
		PCommandList->DrawIndexedInstanced(DrawIndexCount, 1, 0, 0, 0);
	}

	Phe::PShader* PDX12RHI::CreateShader(const std::string ShaderName, const std::wstring FilePath, std::string VS /*= "VS"*/, std::string PS /*= "PS"*/)
	{
		PDX12Shader* NewShader = new PDX12Shader(ShaderName, FilePath, VS, PS);
		if (VS != "")
		{
			auto VSByteCode = CompileShader(FilePath, nullptr, VS, "vs_5_1");
			NewShader->SetVS(VSByteCode);
		}
		if (PS != "")
		{
			auto PSByteCode = CompileShader(FilePath, nullptr, PS, "ps_5_1");
			NewShader->SetPS(PSByteCode);
		}
		//		auto VSByteCode = CompileShader(FilePath, nullptr, VS, "vs_5_1");
		//		auto PSByteCode = CompileShader(FilePath, nullptr, PS, "ps_5_1");
		//		NewShader->SetVS(VSByteCode);
		//		NewShader->SetPS(PSByteCode);
		ComPtr<ID3D12RootSignature> NewRootSignature;

		std::vector<CD3DX12_ROOT_PARAMETER> rootParameters;
		std::vector<CD3DX12_DESCRIPTOR_RANGE> texTables;
		std::vector<CD3DX12_DESCRIPTOR_RANGE> constantTables;

		auto Params = NewShader->GetParams();
		constantTables.reserve(Params.size());
		texTables.reserve(Params.size());
		for (auto& shaderParam : Params)
		{
			CD3DX12_ROOT_PARAMETER rootParameter;
			switch (shaderParam.type)
			{
			case ShaderParamType::ConstantBuffer:
				rootParameter.InitAsConstantBufferView(shaderParam.baseRegister, shaderParam.registerSpace);
				break;
			case ShaderParamType::CBVDescriptorHeap:
				CD3DX12_DESCRIPTOR_RANGE constantTable;
				constantTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, shaderParam.descriptorNums, shaderParam.baseRegister, shaderParam.registerSpace);
				constantTables.push_back(constantTable);
				rootParameter.InitAsDescriptorTable(1, &constantTables.back());
				break;
			case ShaderParamType::SRVDescriptorHeap:
				CD3DX12_DESCRIPTOR_RANGE texTable;
				texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, shaderParam.descriptorNums, shaderParam.baseRegister, shaderParam.registerSpace);
				texTables.push_back(texTable);
				rootParameter.InitAsDescriptorTable(1, &texTables.back());
				break;
			}
			rootParameters.push_back(rootParameter);
		}

		auto staticSamplers = GetStaticSamplers();
		CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(UINT(rootParameters.size()), rootParameters.data(), (UINT)staticSamplers.size(), staticSamplers.data(), D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		ComPtr<ID3DBlob> serializedRootSig = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;
		HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
			serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

		if (errorBlob != nullptr)
		{
			::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		}
		ThrowIfFailed(PDevice->CreateRootSignature(
			0,
			serializedRootSig->GetBufferPointer(),
			serializedRootSig->GetBufferSize(),
			IID_PPV_ARGS(NewRootSignature.GetAddressOf())));
		NewShader->SetRootSignature(NewRootSignature);
		DX12ShaderManager->AddCompiledShader(NewShader);

		return NewShader;
	}

	PPipeline* PDX12RHI::CreatePipeline(PShader* Shader, P_RasterizerDesc Raster, P_BlendState Blend, P_DepthStencilState DepthStencil)
	{
		return new PDX12Pipeline(Shader, Raster, Blend, DepthStencil);
	}

	Phe::PGPUCommonBuffer* PDX12RHI::CreateCommonBuffer(UINT32 InStructByteSize, UINT32 InElementsNum)
	{
		ResetCommandList();
		PDX12GPUCommonBuffer* NewGPUCommonBuffer = new PDX12GPUCommonBuffer(InStructByteSize, InElementsNum, 0);
		auto HeapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(size_t(NewGPUCommonBuffer->GetStructureSizeInBytes()) * InElementsNum);
		PDevice->CreateCommittedResource(&HeapProperty, D3D12_HEAP_FLAG_NONE, &ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&NewGPUCommonBuffer->PBuffer));
		NewGPUCommonBuffer->PBuffer->Map(0, nullptr, reinterpret_cast<void**>(&NewGPUCommonBuffer->PMappedData));

		auto Pair = PCbvSrvUavHeap->Allocate(InElementsNum);
		NewGPUCommonBuffer->BufferHandle = Pair.first;
		NewGPUCommonBuffer->SetHandleOffset(Pair.second);
		for (size_t index = 0; index < InElementsNum; index++)
		{
			D3D12_GPU_VIRTUAL_ADDRESS PerAddress = NewGPUCommonBuffer->PBuffer->GetGPUVirtualAddress();
			D3D12_CONSTANT_BUFFER_VIEW_DESC CbvDesc;
			PerAddress += index * NewGPUCommonBuffer->GetStructureSizeInBytes();
			CbvDesc.BufferLocation = PerAddress;
			CbvDesc.SizeInBytes = NewGPUCommonBuffer->GetStructureSizeInBytes();
			PDevice->CreateConstantBufferView(&CbvDesc, NewGPUCommonBuffer->BufferHandle);
			
		}
		ExecuteCommandList();
		Flush();
		return NewGPUCommonBuffer;
	}

	Phe::PGPUTexture* PDX12RHI::CreateTexture(std::string TextureName, std::wstring FileName, P_TextureType TextureType)
	{
		ResetCommandList();
		PDX12GPUTexture* NewGPUTexture = new PDX12GPUTexture(TextureName, FileName, TextureType);
		ComPtr<ID3D12Resource> PResource = nullptr;
		ComPtr<ID3D12Resource> UploadHeap = nullptr;
		DirectX::CreateDDSTextureFromFile12(PDevice.Get(), PCommandList.Get(), FileName.c_str(), PResource, UploadHeap);
		NewGPUTexture->SetPResource(PResource);
		NewGPUTexture->SetPUploadHeap(UploadHeap);
		auto Pair = PCbvSrvUavHeap->Allocate(1);
		NewGPUTexture->SetHandleOffset(Pair.second);
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		switch (TextureType)
		{
		case Phe::P_TextureType::P_Texture2D:
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.Format = PResource->GetDesc().Format;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = PResource->GetDesc().MipLevels;
			srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
			break;
		case Phe::P_TextureType::P_TextureCube:
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.Format = PResource->GetDesc().Format;;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = PResource->GetDesc().MipLevels;
			srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
			break;
		}
		PDevice->CreateShaderResourceView(PResource.Get(), &srvDesc, Pair.first);
// 		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
// 		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
// 		srvDesc.Format = PResource->GetDesc().Format;
// 		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
// 		srvDesc.Texture2D.MostDetailedMip = 0;
// 		srvDesc.Texture2D.MipLevels = PResource->GetDesc().MipLevels;
// 		srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
// 		PDevice->CreateShaderResourceView(PResource.Get(), &srvDesc, Pair.first);

		ExecuteCommandList();
		Flush();
		return NewGPUTexture;
	}

	Phe::PGPUTexture* PDX12RHI::CreateTexture(std::string TextureName, RTBuffer* InRTBuffer, P_TextureType TextureType)
	{
//		ResetCommandList();
		PDX12GPUTexture* NewGPUTexture = new PDX12GPUTexture(TextureName, L"", TextureType);
		DX12RTBuffer* InDX12RTBuffer = static_cast<DX12RTBuffer*>(InRTBuffer);
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = SwitchSupportFormat(InRTBuffer->GetTextureFormat());//DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
		srvDesc.Texture2D.PlaneSlice = 0;
		auto Pair = PCbvSrvUavHeap->Allocate(1);
		NewGPUTexture->SetHandleOffset(Pair.second);
		PDevice->CreateShaderResourceView(InDX12RTBuffer->PResource->GetResource().Get(), &srvDesc, Pair.first);
//		ExecuteCommandList();
//		Flush();
		return NewGPUTexture;
	}

	RTBuffer* PDX12RHI::CreateRTBuffer(RTBufferType Type, UINT32 Width, UINT32 Height, P_TextureFormat Format)
	{
		DX12RTBuffer* NewRTBuffer = new DX12RTBuffer(Type, Width, Height, Format);
		auto HeapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		NewRTBuffer->PResource->CreateResource(PDevice.Get(), &HeapProperty, D3D12_HEAP_FLAG_NONE, &NewRTBuffer->PResourceDescriptor, NewRTBuffer->InitialState, &NewRTBuffer->POptClear);
		if (Type == RTBufferType::RTColorBuffer)
		{
			D3D12_RENDER_TARGET_VIEW_DESC RTVDesc = {};
			RTVDesc.Format = SwitchFormat(Format);
			RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			RTVDesc.Texture2D.PlaneSlice = 0;

			auto RtvAllocation = PRtvHeap->Allocate(1);
			NewRTBuffer->PHandleOffset = RtvAllocation.second;
			PDevice->CreateRenderTargetView(NewRTBuffer->PResource->GetResource().Get(), &RTVDesc, RtvAllocation.first);
		}
		if (Type == RTBufferType::RTDepthStencilBuffer)
		{
			D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
			dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
			dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			dsvDesc.Texture2D.MipSlice = 0;

			auto DsvAllocation = PDsvHeap->Allocate(1);
			NewRTBuffer->PHandleOffset = DsvAllocation.second;

			PDevice->CreateDepthStencilView(NewRTBuffer->PResource->GetResource().Get(), &dsvDesc, DsvAllocation.first);
		}
		return NewRTBuffer;
	}

	PGPURenderTarget* PDX12RHI::CreateRenderTarget(std::string RenderTargetName, UINT32 Width, UINT32 Height)
	{
		return new PDX12GPURenderTarget(RenderTargetName, Width, Height);
	}

	void PDX12RHI::ResetRTBuffer(RTBuffer* RtBuffer)
	{
		DX12RTBuffer* InRTBuffer = static_cast<DX12RTBuffer*>(RtBuffer);
		auto HeapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		InRTBuffer->PResource->CreateResource(PDevice.Get(), &HeapProperty, D3D12_HEAP_FLAG_NONE, &InRTBuffer->PResourceDescriptor, InRTBuffer->InitialState, &InRTBuffer->POptClear);
		if (RtBuffer->PType == RTBufferType::RTColorBuffer)
		{
			PRtvHeap->Deallocate(RtBuffer->PHandleOffset, 1);
			return;
		}
		if (RtBuffer->PType == RTBufferType::RTDepthStencilBuffer)
		{
			PDsvHeap->Deallocate(RtBuffer->PHandleOffset, 1);
			return;
		}
	}

	void PDX12RHI::SetRenderTarget(PGPURenderTarget* RenderTarget)
	{
		PDX12GPURenderTarget* InRenderTarget = static_cast<PDX12GPURenderTarget*>(RenderTarget);
 		InRenderTarget->PViewport = { 0.0f, 0.0f, (float)InRenderTarget->GetWidth(), (float)InRenderTarget->GetHeight(), 0.0f, 1.0f };
 		InRenderTarget->PScissorRect = { 0,0,long(InRenderTarget->GetWidth()), long(InRenderTarget->GetHeight()) };

//		InRenderTarget->PViewport = { 0.0f, 0.0f, 1920.f, 1080.f, 0.0f, 1.0f };
//		InRenderTarget->PScissorRect = { 0,0,1920, 1080 };

		PCommandList->RSSetViewports(1, &InRenderTarget->PViewport);
		PCommandList->RSSetScissorRects(1, &InRenderTarget->PScissorRect);

		auto RTColorBuffer = InRenderTarget->GetColorBuffer();
		auto RTDSBuffer = InRenderTarget->GetDepthStencilBuffer();
		const D3D12_CPU_DESCRIPTOR_HANDLE* Rtv = nullptr;
		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> RtvDescriptors;
		if (RTColorBuffer.size() > 0)
		{
			RtvDescriptors.reserve(RTColorBuffer.size());
			for (auto ColorBuffer : RTColorBuffer)
			{
				D3D12_CPU_DESCRIPTOR_HANDLE Handle = PRtvHeap->GetCurrentHeap()->GetCPUDescriptorHandleForHeapStart();
				Handle.ptr += size_t(PRtvDescriptorSize) * ColorBuffer->PHandleOffset;
				RtvDescriptors.push_back(Handle);
			}
			Rtv = RtvDescriptors.data();
		}
		const D3D12_CPU_DESCRIPTOR_HANDLE* Dsv = nullptr;
		if (RTDSBuffer)
		{
			D3D12_CPU_DESCRIPTOR_HANDLE Handle = PDsvHeap->GetCurrentHeap()->GetCPUDescriptorHandleForHeapStart();
			Handle.ptr += size_t(PDsvDescriptorSize) * RTDSBuffer->PHandleOffset;
			Dsv = &Handle;
		}
		PCommandList->OMSetRenderTargets(UINT(RTColorBuffer.size()), Rtv, false, Dsv);
		if(RtvDescriptors.size()>0)
		{
			for(size_t index = 0; index< RTColorBuffer.size(); index++)
			{
				PCommandList->ClearRenderTargetView(RtvDescriptors[index], RTVColor, 0 , nullptr);
			}
		}
		if (Dsv)
		{
			PCommandList->ClearDepthStencilView(*Dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
		}

	}

	void PDX12RHI::PrepareBufferHeap()
	{
		ID3D12DescriptorHeap* DescriptorHeaps[] = { PCbvSrvUavHeap->GetCurrentHeap() };
		PCommandList->SetDescriptorHeaps(_countof(DescriptorHeaps), DescriptorHeaps);
	}

	void PDX12RHI::UpdatePipeline(PPipeline* Pipeline, PGPURenderTarget* RenderTarget)
	{
		PDX12Pipeline* InDX12Pipeline = static_cast<PDX12Pipeline*>(Pipeline);
		PDX12Shader* InDX12Shader = static_cast<PDX12Shader*>(Pipeline->GetShader());
		PDX12GPURenderTarget* InDX12RenderTarget = static_cast<PDX12GPURenderTarget*>(RenderTarget);
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PsoDesc;
		ZeroMemory(&PsoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
		std::vector<D3D12_INPUT_ELEMENT_DESC> InputLayout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TANGENTY", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 44, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXTCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 60, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
		};

		PsoDesc.InputLayout = { InputLayout.data(), (UINT)InputLayout.size() };

//		InDX12Shader->SetPSODesc(&PsoDesc);

		if (auto PVS = InDX12Shader->GetVS())
		{
			PsoDesc.VS =
			{
				reinterpret_cast<BYTE*>(PVS->GetBufferPointer()),
				PVS->GetBufferSize()
			};
		}
		if (auto PPS = InDX12Shader->GetPS())
		{
			PsoDesc.PS =
			{
				reinterpret_cast<BYTE*>(PPS->GetBufferPointer()),
				PPS->GetBufferSize()
			};
		}

		PsoDesc.pRootSignature = InDX12Shader->GetRootSignature().Get();
		PsoDesc.RasterizerState = InDX12Pipeline->PRasterizerState;
		PsoDesc.RasterizerState.FrontCounterClockwise = true;
		PsoDesc.DepthStencilState = InDX12Pipeline->PDepthStencilState;
		PsoDesc.BlendState = InDX12Pipeline->PBlendState;



		PsoDesc.NumRenderTargets = InDX12RenderTarget->GetColorBuffer().size() > 0 ? 1 : 0;
//		PsoDesc.RTVFormats[0] = InDX12RenderTarget->GetColorBuffer().size() > 0 ? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_UNKNOWN;
		if(InDX12RenderTarget->GetColorBuffer().size())
		{
			PsoDesc.RTVFormats[0] = InDX12RenderTarget->GetColorBuffer().size() ? SwitchFormat(InDX12RenderTarget->GetColorBuffer().at(0)->GetTextureFormat()) : DXGI_FORMAT_UNKNOWN;
		}
		PsoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;


		PsoDesc.SampleMask = UINT_MAX;
		PsoDesc.SampleDesc.Count = 1;
		PsoDesc.SampleDesc.Quality = 0;
		PsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		ComPtr<ID3D12PipelineState> PSO;
		ThrowIfFailed(PDevice->CreateGraphicsPipelineState(&PsoDesc, IID_PPV_ARGS(&PSO)));
		InDX12Pipeline->SetPipelineState(PSO);
	}

	void PDX12RHI::UpdatePipeline(PPipeline* Pipeline, P_TextureFormat RtvFormat, P_TextureFormat DsvFormat)
	{
		PDX12Pipeline* InDX12Pipeline = static_cast<PDX12Pipeline*>(Pipeline);
		PDX12Shader* InDX12Shader = static_cast<PDX12Shader*>(Pipeline->GetShader());
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PsoDesc;
		ZeroMemory(&PsoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
		std::vector<D3D12_INPUT_ELEMENT_DESC> InputLayout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TANGENTY", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 44, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXTCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 60, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
		};

		PsoDesc.InputLayout = { InputLayout.data(), (UINT)InputLayout.size() };
		
		if (auto PVS = InDX12Shader->GetVS())
		{
			PsoDesc.VS =
			{
				reinterpret_cast<BYTE*>(PVS->GetBufferPointer()),
				PVS->GetBufferSize()
			};
		}
		if (auto PPS = InDX12Shader->GetPS())
		{
			PsoDesc.PS =
			{
				reinterpret_cast<BYTE*>(PPS->GetBufferPointer()),
				PPS->GetBufferSize()
			};
		}

		PsoDesc.pRootSignature = InDX12Shader->GetRootSignature().Get();
		PsoDesc.RasterizerState = InDX12Pipeline->PRasterizerState;
		PsoDesc.RasterizerState.FrontCounterClockwise = true;
		PsoDesc.DepthStencilState = InDX12Pipeline->PDepthStencilState;
		PsoDesc.BlendState = InDX12Pipeline->PBlendState;


		PsoDesc.NumRenderTargets = 1;
		PsoDesc.RTVFormats[0] = SwitchFormat(RtvFormat);
		PsoDesc.DSVFormat = SwitchFormat(DsvFormat);

		PsoDesc.SampleMask = UINT_MAX;
		PsoDesc.SampleDesc.Count = 1;
		PsoDesc.SampleDesc.Quality = 0;
		PsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		ComPtr<ID3D12PipelineState> PSO;
		ThrowIfFailed(PDevice->CreateGraphicsPipelineState(&PsoDesc, IID_PPV_ARGS(&PSO)));
		InDX12Pipeline->SetPipelineState(PSO);
	}

	void PDX12RHI::UpdateCommonBuffer(PGPUCommonBuffer* CommonBuffer, std::shared_ptr<void> Data)
	{
		CommonBuffer->ReleaseCurrentData();
		CommonBuffer->AllocateData(0, Data);
	}

	void PDX12RHI::Set32BitConstant(PPipeline* InPipeline, std::string PropertyName, std::shared_ptr<void> data)
	{
		PDX12Shader* InDX12Shader = static_cast<PDX12Shader*>(InPipeline->GetShader());
		auto ShaderParam = InDX12Shader->GetParams().at(DX12ShaderManager->PropertyToID(PropertyName));
		PCommandList->SetGraphicsRoot32BitConstants(ShaderParam.GetBaseRegister(), 2, data.get(), 0);
	}

	void PDX12RHI::SetRenderResourceTable(std::string PropertyName, UINT32 HeapOffset)
	{
		auto CbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(PCbvSrvUavHeap->GetCurrentHeap()->GetGPUDescriptorHandleForHeapStart());
		CbvHandle.Offset(HeapOffset, PCbvSrvUavDescriptorSize);

		PCommandList->SetGraphicsRootDescriptorTable(DX12ShaderManager->PropertyToID(PropertyName), CbvHandle);
	}

// 	void PDX12RHI::CompileMaterial(PMaterial* Material)
// 	{
// 		std::vector<PGPUTexture*> RetGPUTextureBuffer;
// 		auto TextureNameVector = Material->GetTextureName();
// 		for (auto TextureName : TextureNameVector)
// 		{
// 			bool IsFindInPool = false;
// 			for (auto T : TextureRefPool)
// 			{
// 				if (T.first->GetTextureName() == TextureName)
// 				{
// 					T.second += 1;
// 					RetGPUTextureBuffer.push_back(T.first);
// 					IsFindInPool = true;
// 					break;
// 				}
// 			}
// 			if (!IsFindInPool)
// 			{
// 				auto TextureData = PAssetManager::GetSingleton().GetTextureData(TextureName);
// 				auto NewTexture = PRHI::Get()->CreateTexture(TextureName, TextureData->GetTextureFileName());
// 				TextureRefPool.insert({ NewTexture, 1 });
// 				RetGPUTextureBuffer.push_back(NewTexture);
// 			}
// 
// 		}
// 		Material->SetGPUTextureBuffer(RetGPUTextureBuffer);
//	}

// 	void PDX12RHI::AddTextureToMaterial(PMaterial* Material, std::string TextureName)
// 	{
// 		auto CurrentTextureBuffer = Material->GetGPUTextureBuffer();
// 		for (auto T : TextureRefPool)
// 		{
// 			if (T.first->GetTextureName() == TextureName)
// 			{
// 				T.second += 1;
// 				CurrentTextureBuffer.push_back(T.first);
// 				return;
// 			}
// 		}
// 		auto TextureData = PAssetManager::GetSingleton().GetTextureData(TextureName);
// 		auto NewTexture = PRHI::Get()->CreateTexture(TextureName, TextureData->GetTextureFileName());
// 		TextureRefPool.insert({ NewTexture, 1 });
// 		CurrentTextureBuffer.push_back(NewTexture);
// 		return;
//	}

	void PDX12RHI::DeleteTexturefromMaterial(PMaterial* Material, std::string TextureName)
	{
// 		for (auto T : TextureRefPool)
// 		{
// 			if (T.first->GetTextureName() == TextureName)
// 			{
// 				T.second -= 1;
// 				if (T.second == 0)
// 				{
// 					TextureRefPool.erase(T.first);
// 					DestroyTexture(T.first);
// 				}
// 				break;
// 			}
// 		}
// 		auto CurrentTextureBuffer = Material->GetGPUTextureBuffer();
// 
// 		for (auto it = CurrentTextureBuffer.begin(); it != CurrentTextureBuffer.end(); it++)
// 		{
// 			if ((*it)->GetTextureName() == TextureName)
// 			{
// 				CurrentTextureBuffer.erase(it);
// 				return;
// 			}
// 		}
	}

	void PDX12RHI::DestroyPrimitive(PPrimitive* Primitive)
	{
		auto PerObjBuffer = Primitive->GetPerObjBuffer();
		if(PerObjBuffer)
		{
			PCbvSrvUavHeap->Deallocate(PerObjBuffer->GetHandleOffset(), PerObjBuffer->GetElements());
		}
		auto PerMatBuffer = Primitive->GetPerMatBuffer();
		if(PerMatBuffer)
		{
			PCbvSrvUavHeap->Deallocate(PerMatBuffer->GetHandleOffset(), PerMatBuffer->GetElements());
		}
	}

	void PDX12RHI::DestroyTexture(PGPUTexture* Texture)
	{
		PCbvSrvUavHeap->Deallocate(Texture->GetHandleOffset(), 1);

 		ReleasePtr(Texture);
	}

	void PDX12RHI::DestroyMaterial(PMaterial* Material)
	{

	}

	void PDX12RHI::DestroyRTBuffer(RTBuffer* RtBuffer)
	{
		if(RtBuffer->PType == RTBufferType::RTColorBuffer)
		{
			PRtvHeap->Deallocate(RtBuffer->PHandleOffset, 1);
		}
		if(RtBuffer->PType == RTBufferType::RTDepthStencilBuffer)
		{
			PDsvHeap->Deallocate(RtBuffer->PHandleOffset, 1);
		}
	}


	void PDX12RHI::DestroyGPUCommonBuffer(PGPUCommonBuffer* GPUCommonBuffer)
	{
		PCbvSrvUavHeap->Deallocate(GPUCommonBuffer->GetHandleOffset(), 1);
		ReleasePtr(GPUCommonBuffer);
	}

	//Please Ensure the RTBuffer State is Generic_Read!!
	void PDX12RHI::ReadBackRTBuffer(RTBuffer* InRTBuffer)
	{
		DX12RTBuffer* InDX12RTBuffer = static_cast<DX12RTBuffer*>(InRTBuffer);

		HRESULT hr = SaveDDSTextureToFile(PCommandQueue.Get(), InDX12RTBuffer->PResource->GetResource().Get(), L"Depth.dds");
	}

	void PDX12RHI::ReadBackTexture(PGPUTexture* InTexture)
	{
		PDX12GPUTexture* InDX12Texture = static_cast<PDX12GPUTexture*>(InTexture);
		HRESULT hr = SaveDDSTextureToFile(PCommandQueue.Get(), InDX12Texture->GetPResource()->GetResource().Get(), L"Depth.dds");
	}

	void PDX12RHI::ResetCommandList()
	{
		ThrowIfFailed(PCommandList->Reset(PCommandAllocator.Get(), nullptr));
	}

	void PDX12RHI::ExecuteCommandList()
	{
		ThrowIfFailed(PCommandList->Close());
		ID3D12CommandList* cmdsLists[] = { PCommandList.Get() };
		PCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	}

	Microsoft::WRL::ComPtr<ID3DBlob> PDX12RHI::CompileShader(const std::wstring& Filename, const D3D_SHADER_MACRO* Defines, const std::string& EntryPoint, const std::string& Target)
	{
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
		ComPtr<ID3DBlob> ByteCode;
		ComPtr<ID3DBlob> Errors;
		D3DCompileFromFile(Filename.c_str(), Defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			EntryPoint.c_str(), Target.c_str(), compileFlags, 0, &ByteCode, &Errors);
		return ByteCode;
	}

	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 7> PDX12RHI::GetStaticSamplers()
	{
		// Applications usually only need a handful of samplers.  So just define them all up front
		// and keep them available as part of the root signature.  

		const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
			0, // shaderRegister
			D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
			D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

		const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
			1, // shaderRegister
			D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

		const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
			2, // shaderRegister
			D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
			D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

		const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
			3, // shaderRegister
			D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

		const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
			4, // shaderRegister
			D3D12_FILTER_ANISOTROPIC, // filter
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
			0.0f,                             // mipLODBias
			8);                               // maxAnisotropy

		const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
			5, // shaderRegister
			D3D12_FILTER_ANISOTROPIC, // filter
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
			0.0f,                              // mipLODBias
			8);                                // maxAnisotropy

		const CD3DX12_STATIC_SAMPLER_DESC shadowSampler(
			6, // shaderRegister
			D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT, // filter
			D3D12_TEXTURE_ADDRESS_MODE_BORDER,  // addressU
			D3D12_TEXTURE_ADDRESS_MODE_BORDER,  // addressV
			D3D12_TEXTURE_ADDRESS_MODE_BORDER,  // addressW
			0.0f,                               // mipLODBias
			16,                                 // maxAnisotropy
			D3D12_COMPARISON_FUNC_LESS_EQUAL,
			D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK);

		return {
			pointWrap,
			pointClamp,
			linearWrap,
			linearClamp,
			anisotropicWrap,
			anisotropicClamp,
			shadowSampler
		};
	}

	Microsoft::WRL::ComPtr<ID3D12Resource> PDX12RHI::CreateDefaultBuffer(const void* initData, UINT64 byteSize, Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer)
	{
		ComPtr<ID3D12Resource> defaultBuffer;

		// Create the actual default buffer resource.
		auto HeapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		auto ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
		ThrowIfFailed(PDevice->CreateCommittedResource(
			&HeapProperty,
			D3D12_HEAP_FLAG_NONE,
			&ResourceDesc,
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(defaultBuffer.GetAddressOf())));

		HeapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
		ThrowIfFailed(PDevice->CreateCommittedResource(
			&HeapProperty,
			D3D12_HEAP_FLAG_NONE,
			&ResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(uploadBuffer.GetAddressOf())));

		D3D12_SUBRESOURCE_DATA subResourceData = {};
		subResourceData.pData = initData;
		subResourceData.RowPitch = byteSize;
		subResourceData.SlicePitch = subResourceData.RowPitch;

		auto Resourcebarrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
		PCommandList->ResourceBarrier(1, &Resourcebarrier);
		UpdateSubresources<1>(PCommandList.Get(), defaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, &subResourceData);
		Resourcebarrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
		PCommandList->ResourceBarrier(1, &Resourcebarrier);

		return defaultBuffer;
	}
}