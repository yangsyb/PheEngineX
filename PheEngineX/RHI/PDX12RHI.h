#pragma once
#include "pch.h"
#include "PRHI.h"
#include "DX12/PDescriptorHeap.h"
#include "DX12/PDX12ShaderManager.h"
#include "DX12/DxException.h"

namespace Phe
{
	class PDX12RHI : public PRHI
	{
	public:
		PDX12RHI();
		virtual void InitRHI() override;
		virtual void BeginFrame() override;
		virtual void EndFrame() override;
		virtual void Flush() override;

		virtual void InitGraphicsPipeline() override;

		virtual void ResizeWindow(UINT32 PWidth, UINT32 PHeight) override;

		virtual void UpdateMeshBuffer(PGPUMeshBuffer* GpuMeshBuffer) override;

		virtual void BeginRenderBackBuffer() override;
		virtual void EndRenderBackBuffer() override;

		virtual void SetGraphicsPipeline(PPipeline* Pipeline) override;
		virtual void SetMeshBuffer(PGPUMeshBuffer* InMeshBuffer) override;
		virtual void DrawPrimitiveIndexedInstanced(UINT DrawIndexCount) override;

		virtual PGPUMeshBuffer* CreateMeshBuffer() override;
		virtual PShader* CreateShader(const std::string ShaderName, const std::wstring FilePath, std::string VS = "VS", std::string PS = "PS") override;
		virtual PPipeline* CreatePipeline(PShader* Shader) override;
		virtual PGPUCommonBuffer* CreateCommonBuffer(UINT32 InStructByteSize, UINT32 InElementsNum);
		virtual PGPUTexture* CreateTexture(std::string TextureName, std::wstring FileName);
		virtual void PrepareBufferHeap() override;
		virtual void SetPipeline(PPipeline* Pipeline) override;
		virtual void SetRenderResourceTable(std::string PropertyName, UINT32 HeapOffset) override;
		virtual void ReCompileMaterial(PMaterial* Material) override;

	private:
		void ResetCommandList();
		void ExecuteCommandList();

		ID3D12Resource* CurrentBackBuffer() const
		{
			return PSwapChainBuffer[PCurrBackBuffer].Get();
		}

		D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const
		{
			return CD3DX12_CPU_DESCRIPTOR_HANDLE(
				PRtvHeap->GetCPUDescriptorHandleForHeapStart(),
				PCurrBackBuffer,
				PRtvDescriptorSize);
		}

		D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const
		{
			return PDsvHeap->GetCPUDescriptorHandleForHeapStart();
		}

		static ComPtr<ID3DBlob> CompileShader(const std::wstring& Filename, const D3D_SHADER_MACRO* Defines, const std::string& EntryPoint, const std::string& Target);
		std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();
		ComPtr<ID3D12Resource> CreateDefaultBuffer(const void* initData, UINT64 byteSize, Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer);

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
		std::unique_ptr<PDescriptorHeap> CbvSrvUavHeap;

		UINT PerObjectCBufferByteSize;
		UINT PerCameraCBufferByteSize;
		UINT PerFrameCBufferByteSize;
		UINT PerMaterialCBufferByteSize;

		PDX12Shadermanager* DX12ShaderManager;
	};
}