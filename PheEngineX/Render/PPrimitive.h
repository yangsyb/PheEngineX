#pragma once
#include "pch.h"
#include "Engine/Transform.h"
#include "GPUResource/PGPUMeshBuffer.h"
#include "GPUResource/PPipeline.h"
#include "GPUResource/PGPUCommonBuffer.h"
#include "Engine/PMaterial.h"

namespace Phe
{
	class PPrimitive
	{
	public:
		PPrimitive();
		~PPrimitive();

		void SetPrimitiveRenderData(PGPUMeshBuffer* MeshBuffer, Transform MeshTransform, PGPUCommonBuffer* ObjBuffer, PGPUCommonBuffer* MatBuffer, PMaterial* MaterialBuffer);
		void SetPipeline(PPipeline* pipe) { Pipeline = pipe;}
		PGPUMeshBuffer* GetMeshBuffer() { return PMeshBuffer; }
		Transform GetTransform() { return PTransform; }
		PPipeline* GetPipeline() { return Pipeline; }
		PGPUCommonBuffer* GetPerObjBuffer() { return PerObjBuffer; }
		PGPUCommonBuffer* GetPerMatBuffer() { return PerMatBuffer; }
		PMaterial* GetMaterial() { return PrimitiveMaterial; }

		void DestroyPrimitive();
	private:
		PGPUMeshBuffer* PMeshBuffer;
		Transform PTransform;
		PPipeline* Pipeline;
		PGPUCommonBuffer* PerObjBuffer;
		PGPUCommonBuffer* PerMatBuffer;

		PMaterial* PrimitiveMaterial;
	};
}