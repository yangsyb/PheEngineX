#pragma once
#include "pch.h"
#include "PRHI.h"
#include "DX12/PDescriptorHeap.h"
#include "DX12/PDX12ShaderManager.h"
#include "DX12/DxException.h"
#include "DX12/PDX12GPURenderTarget.h"
namespace Phe
{
	class PDX12RHI : public PRHI
	{
	public:
		PDX12RHI();
		virtual ~PDX12RHI();
		virtual void InitRHI() override;
		virtual void BeginFrame() override;
		virtual void EndFrame() override;
		virtual void InitGraphicsPipeline() override;
		virtual void ResizeWindow(UINT32 PWidth, UINT32 PHeight) override;

		virtual PGPUMeshBuffer* CreateMeshBuffer() override;
		virtual PShader* CreateShader(const std::string ShaderName, const std::wstring FilePath, std::string VS = "VS", std::string PS = "PS") override;
		virtual PPipeline* CreatePipeline(PShader* Shader) override;
		virtual PGPUCommonBuffer* CreateCommonBuffer(UINT32 InStructByteSize, UINT32 InElementsNum) override;
		virtual PGPUTexture* CreateTexture(std::string TextureName, RTBuffer* InRTBuffer, P_TextureType TextureType) override;
		virtual PGPUTexture* CreateTexture(std::string TextureName, std::wstring FileName, P_TextureType TextureType) override;
		virtual RTBuffer* CreateRTBuffer(RTBufferType Type, UINT32 Width, UINT32 Height) override;
		virtual PGPURenderTarget* CreateRenderTarget(std::string RenderTargetName = "Default", UINT32 Width = 1920, UINT32 Height = 1080) override;

		virtual void ResetRTBuffer(RTBuffer* RtBuffer) override;
		virtual void SetRenderTarget(PGPURenderTarget* RenderTarget) override;

		virtual void UpdateMeshBuffer(PGPUMeshBuffer* GpuMeshBuffer) override;
		virtual void UpdatePipeline(PPipeline* Pipeline) override;
		virtual void UpdatePipeline(PPipeline* Pipeline, PGPURenderTarget* RenderTarget) override;
		virtual void UpdateCommonBuffer(PGPUCommonBuffer* CommonBuffer, std::shared_ptr<void> Data) override;

		virtual void BeginRenderBackBuffer() override;
		virtual void EndRenderBackBuffer() override;
		virtual void BeginRenderRTBuffer(RTBuffer* RtBuffer) override;
		virtual void EndRenderRTBuffer(RTBuffer* RtBuffer) override;
		virtual void PrepareBufferHeap() override;
		virtual void SetGraphicsPipeline(PPipeline* Pipeline) override;
		virtual void SetMeshBuffer(PGPUMeshBuffer* InMeshBuffer) override;
		virtual void SetRenderResourceTable(std::string PropertyName, UINT32 HeapOffset) override;
		virtual void DrawPrimitiveIndexedInstanced(UINT DrawIndexCount) override;

// 		virtual void CompileMaterial(PMaterial* Material) override;
// 		virtual void AddTextureToMaterial(PMaterial* Material, std::string TextureName) override;
		virtual void DeleteTexturefromMaterial(PMaterial* Material, std::string TextureName) override;
		virtual void DestroyPrimitive(PPrimitive* Primitive) override;
		virtual void DestroyTexture(PGPUTexture* Texture) override;
		virtual void DestroyMaterial(PMaterial* Material) override;
		virtual void DestroyRTBuffer(RTBuffer* RtBuffer) override;
		virtual void DestroyGPUCommonBuffer(PGPUCommonBuffer* GPUCommonBuffer) override;

		virtual void ReadBackRTBuffer(RTBuffer* InRTBuffer) override;
		virtual void ReadBackTexture(PGPUTexture* InTexture) override;

		virtual void Flush() override;
	private:
		void ResetCommandList();
		void ExecuteCommandList();

		ID3D12Resource* CurrentBackBuffer() const
 		{
// 			return PSwapChainBuffer[PCurrBackBuffer].Get();
//			return static_cast<PDX12GPURenderTarget*>(PSwapChainBuffer[PCurrBackBuffer])->GetGpuResrouce()->GetResource().Get();
			return static_cast<DX12RTBuffer*>(ScreenRT->GetColorBuffer().at(PCurrBackBuffer))->PResource->GetResource().Get();
		}

		D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const
		{
			return CD3DX12_CPU_DESCRIPTOR_HANDLE(
				PRtvHeap->GetCurrentHeap()->GetCPUDescriptorHandleForHeapStart(),
				PCurrBackBuffer,
				PRtvDescriptorSize);
		}

		D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const
		{
			return PDsvHeap->GetCurrentHeap()->GetCPUDescriptorHandleForHeapStart();
		}

		static ComPtr<ID3DBlob> CompileShader(const std::wstring& Filename, const D3D_SHADER_MACRO* Defines, const std::string& EntryPoint, const std::string& Target);
		std::array<const CD3DX12_STATIC_SAMPLER_DESC, 7> GetStaticSamplers();
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
//  		ComPtr<ID3D12Resource> PSwapChainBuffer[SwapChainBufferCount];
//  		ComPtr<ID3D12Resource> PDepthStencilBuffer;
		
// 		std::vector<PGPURenderTarget*> PSwapChainBuffer;

 		ComPtr<ID3D12Resource> PDepthStencilBuffer;

		int PCurrBackBuffer = 0;

		ComPtr<ID3D12CommandQueue> PCommandQueue;
		ComPtr<ID3D12CommandAllocator> PCommandAllocator;
		ComPtr<ID3D12GraphicsCommandList> PCommandList;

		UINT PRtvDescriptorSize;
		UINT PDsvDescriptorSize;
		UINT PCbvSrvUavDescriptorSize;
//		ComPtr<ID3D12DescriptorHeap> PRtvHeap;
//		ComPtr<ID3D12DescriptorHeap> PDsvHeap;
		std::unique_ptr<PDescriptorHeap> PRtvHeap;
		std::unique_ptr<PDescriptorHeap> PDsvHeap;

		CD3DX12_VIEWPORT PViewport;
		CD3DX12_RECT PScissorRect;


		//Pipeline
		std::unique_ptr<PDescriptorHeap> PCbvSrvUavHeap;

		PDX12Shadermanager* DX12ShaderManager;
	};
}