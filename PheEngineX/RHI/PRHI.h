#pragma once
#include "pch.h"
#include "GPUResource/PGPUMeshBuffer.h"
#include "GPUResource/PGPUTexture.h"
#include "Engine/PShader.h"
#include "GPUResource/PPipeline.h"
#include "GPUResource/PGPUCommonBuffer.h"
#include "Engine/PMaterial.h"

namespace Phe
{
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

		virtual void UpdateMeshBuffer(PGPUMeshBuffer* GpuMeshBuffer) = 0;


		virtual void BeginRenderBackBuffer() = 0;
		virtual void EndRenderBackBuffer() = 0;

		virtual void SetGraphicsPipeline(PPipeline* Pipeline) = 0;
		virtual void SetMeshBuffer(PGPUMeshBuffer* InMeshBuffer) = 0;
		virtual void DrawPrimitiveIndexedInstanced(UINT DrawIndexCount) = 0;

		virtual PGPUMeshBuffer* CreateMeshBuffer() = 0;
		virtual PShader* CreateShader(const std::string ShaderName, const std::wstring FilePath, std::string VS = "VS", std::string PS = "PS") = 0;
		virtual PPipeline* CreatePipeline(PShader* Shader) = 0;
		virtual PGPUCommonBuffer* CreateCommonBuffer(UINT32 InStructByteSize, UINT32 InElementsNum) = 0;
		virtual PGPUTexture* CreateTexture(std::string TextureName, std::wstring FileName) = 0;
		virtual void PrepareBufferHeap() = 0;
		virtual void SetPipeline(PPipeline* Pipeline) = 0;
		virtual void SetRenderResourceTable(std::string PropertyName, UINT32 HeapOffset) = 0;
		virtual void ReCompileMaterial(PMaterial* Material) = 0;
		

		virtual void Flush() = 0;

		static PRHI* RHI;
		PRHI();
		virtual ~PRHI();

	protected:
	};
}