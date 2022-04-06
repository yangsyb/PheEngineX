#pragma once
#include "pch.h"
#include "GPUResource/PGPUMeshBuffer.h"
#include "GPUResource/PGPUTexture.h"
#include "Engine/Editor/PShader.h"
#include "Engine/Editor/PPipeline.h"
#include "GPUResource/PGPUCommonBuffer.h"
#include "Engine/Editor/PMaterial.h"
#include "Render/PPrimitive.h"
#include "GPUResource/PGPURenderTarget.h"

namespace Phe
{
	static UINT32 ScreenFrameNumber = 3;
	class PRHI
	{
	public:
		static PRHI* Get();
		static void CreateRHI();
		static void DestroyRHI();

		virtual void InitRHI() = 0;
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void InitGraphicsPipeline() = 0;
		virtual void ResizeWindow(UINT32 PWidth, UINT32 PHeight) = 0;

		virtual PGPUMeshBuffer* CreateMeshBuffer() = 0;
		virtual PShader* CreateShader(const std::string ShaderName, const std::wstring FilePath, std::string VS = "VS", std::string PS = "PS") = 0;
		virtual PPipeline* CreatePipeline(PShader* Shader) = 0;
		virtual PGPUCommonBuffer* CreateCommonBuffer(UINT32 InStructByteSize, UINT32 InElementsNum) = 0;
		virtual PGPUTexture* CreateTexture(std::string TextureName, RTBuffer* InRTBuffer) = 0;
		virtual PGPUTexture* CreateTexture(std::string TextureName, std::wstring FileName) = 0;
		virtual RTBuffer* CreateRTBuffer(RTBufferType Type, UINT32 Width, UINT32 Height) = 0;
		virtual PGPURenderTarget* CreateRenderTarget(std::string RenderTargetName = "Default", UINT32 Width = 1920, UINT32 Height = 1080) = 0;

		virtual void ResetRTBuffer(RTBuffer* RtBuffer) = 0;
		virtual void SetRenderTarget(PGPURenderTarget* RenderTarget) = 0;

		virtual void UpdateMeshBuffer(PGPUMeshBuffer* GpuMeshBuffer) = 0;
		virtual void UpdatePipeline(PPipeline* Pipeline) = 0;
		virtual void UpdatePipeline(PPipeline* Pipeline, PGPURenderTarget* RenderTarget) = 0;
		virtual void UpdateCommonBuffer(PGPUCommonBuffer* CommonBuffer, std::shared_ptr<void> Data) = 0;

		virtual void BeginRenderBackBuffer() = 0;
		virtual void EndRenderBackBuffer() = 0;
		virtual void BeginRenderRTBuffer(RTBuffer* RtBuffer) = 0;
		virtual void EndRenderRTBuffer(RTBuffer* RtBuffer) = 0;
		virtual void PrepareBufferHeap() = 0;
		virtual void SetGraphicsPipeline(PPipeline* Pipeline) = 0;
		virtual void SetMeshBuffer(PGPUMeshBuffer* InMeshBuffer) = 0;
		virtual void SetRenderResourceTable(std::string PropertyName, UINT32 HeapOffset) = 0;
		virtual void DrawPrimitiveIndexedInstanced(UINT DrawIndexCount) = 0;

// 		virtual void CompileMaterial(PMaterial* Material) = 0;
// 		virtual void AddTextureToMaterial(PMaterial* Material, std::string TextureName) = 0;
		virtual void DeleteTexturefromMaterial(PMaterial* Material, std::string TextureName) = 0;
		virtual void DestroyPrimitive(PPrimitive* Primitive) = 0;
		virtual void DestroyTexture(PGPUTexture* Texture, bool CheckMap = false) = 0;
		virtual void DestroyMaterial(PMaterial* Material) = 0;
		virtual void DestroyRTBuffer(RTBuffer* RtBuffer) = 0;
		
		//Read Back
		virtual void ReadBackRTBuffer(RTBuffer* InRTBuffer) = 0;
		virtual void ReadBackTexture(PGPUTexture* InTexture) = 0;

		virtual void Flush() = 0;

		static PRHI* RHI;
		PRHI();
		virtual ~PRHI();

	protected:
		static PGPURenderTarget* ScreenRT;
	};
}