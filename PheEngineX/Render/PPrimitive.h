#pragma once
#include "pch.h"
#include "Engine/Editor/PPipeline.h"
#include "GPUResource/PGPUCommonBuffer.h"
#include "Engine/Editor/PMaterial.h"
#include "GPUResource/PGPUMeshBuffer.h"

namespace Phe
{

	class PPrimitive
	{
	public:
		PPrimitive();
		~PPrimitive();

		void SetPrimitiveRenderData(PGPUMeshBuffer* MeshBuffer, PGPUCommonBuffer* ObjBuffer, PGPUCommonBuffer* MatBuffer, PMaterial* MaterialBuffer);
//		void SetPipeline(PPipeline* Pipe) { Pipeline = Pipe; }
		void SetPipeline(PipelineType PType, PPipeline* Pipeline);
		void SetPipeline(PVector<PPipeline*> Pipelines) { PPipelines = Pipelines; }
		PGPUMeshBuffer* GetMeshBuffer() { return PMeshBuffer; }
//		PPipeline* GetPipeline() { return Pipeline; }
		PPipeline* GetPipeline(PipelineType PType) { return PPipelines[static_cast<int>(PType)]; }
		PGPUCommonBuffer* GetPerObjBuffer() { return PerObjBuffer; }
		PGPUCommonBuffer* GetPerMatBuffer() { return PerMatBuffer; }
		PMaterial* GetMaterial() { return PrimitiveMaterial; }

		void UpdateCommonBuffer(PGPUCommonBuffer* Buffer, void* Data);

		void DestroyPrimitive();
	private:
		PGPUMeshBuffer* PMeshBuffer;
//		PPipeline* Pipeline;
		PVector<PPipeline*> PPipelines;
		PGPUCommonBuffer* PerObjBuffer;
		PGPUCommonBuffer* PerMatBuffer;
		PMaterial* PrimitiveMaterial;
	};
}