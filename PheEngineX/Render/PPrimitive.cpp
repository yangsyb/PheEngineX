#include "pch.h"
#include "PPrimitive.h"
#include "RHI/PRHI.h"

namespace Phe
{

	PPrimitive::PPrimitive() : PMeshBuffer(nullptr), PerMatBuffer(nullptr), PerObjBuffer(nullptr), PrimitiveMaterial(nullptr)
	{
		PPipelines.resize(static_cast<int>(PipelineType::PipelineCount));
	}

	PPrimitive::~PPrimitive()
	{

	}

	void PPrimitive::SetPrimitiveRenderData(PGPUMeshBuffer* MeshBuffer, PGPUCommonBuffer* ObjBuffer, PGPUCommonBuffer* MatBuffer, PMaterial* MaterialBuffer)
	{
		PMeshBuffer = MeshBuffer;
		PerObjBuffer = ObjBuffer;
		PerMatBuffer = MatBuffer;
		PrimitiveMaterial = MaterialBuffer;
	}


	void PPrimitive::SetPipeline(PipelineType PType, PPipeline* Pipeline)
	{
		PPipelines[static_cast<int>(PType)] = Pipeline;
	}

	void PPrimitive::UpdateCommonBuffer(PGPUCommonBuffer* Buffer, std::shared_ptr<void> Data)
	{
		PRHI::Get()->UpdateCommonBuffer(Buffer, Data);
	}

	void PPrimitive::DestroyPrimitive()
	{
		PRHI::Get()->DestroyPrimitive(this);
 		PMeshBuffer = nullptr;
		ReleasePtr(PerObjBuffer);
		ReleasePtr(PerMatBuffer);
	}
}