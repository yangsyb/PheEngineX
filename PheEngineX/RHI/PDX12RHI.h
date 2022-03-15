#pragma once
#include "pch.h"
#include "PRHI.h"

namespace Phe
{
	class PDX12RHI : public PRHI
	{
	public:
		virtual void InitRHI() override;
		virtual void BeginFrame() override;
		virtual void EndFrame() override;
		virtual void Flush() override;

		virtual void InitGraphicsPipeline() override;

		virtual void AddRenderResourceMesh() override;

		PDX12RHI();
	private:
		ComPtr<IDXGIFactory4> PDXGIFactory;

		ComPtr<ID3D12Device> PDevice;

		HANDLE PFenceEvent;
		ComPtr<ID3D12Fence> PFence;
		UINT64 PFenceValue;

		static const int SwapChainBufferCount = 3;
		float RTVColor[4] = { 0,0,0,0 };

		ComPtr<IDXGISwapChain> PSwapChain;
		ComPtr<ID3D12Resource> PSwapChainBuffer[SwapChainBufferCount];
		ComPtr<ID3D12Resource> PDepthStencilBuffer;
		int PCurrBackBuffer = 0;

		ComPtr<ID3D12CommandQueue> PCommandQueue;
		ComPtr<ID3D12CommandAllocator> PCommandAllocator;
		ComPtr<ID3D12GraphicsCommandList> PCommandList;

		UINT PRtvDescriptorSize;
		UINT PDsvDescriptorSize;
		UINT PCbvSrvUavDescriptorSize;
		ComPtr<ID3D12DescriptorHeap> PRtvHeap;
		ComPtr<ID3D12DescriptorHeap> PDsvHeap;

		CD3DX12_VIEWPORT PViewport;
		CD3DX12_RECT PScissorRect;


		//Pipeline
		ComPtr<ID3D12DescriptorHeap> CbvHeap;

		std::unique_ptr <UploadBuffer<PerObjectCBuffer>> PerObjCB;
		std::unique_ptr <UploadBuffer<PerCameraCBuffer>> PerCameraCB;
		std::unique_ptr <UploadBuffer<PerFrameCBuffer>> PerFrameCB;
		std::unique_ptr <UploadBuffer<PerMaterialCBuffer>> PerMaterialCB;

		UINT PerObjectCBufferByteSize;
		UINT PerCameraCBufferByteSize;
		UINT PerFrameCBufferByteSize;
		UINT PerMaterialCBufferByteSize;
	};
}