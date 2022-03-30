#include "pch.h"
#include "PPrimitive.h"
#include "RHI/PRHI.h"

namespace Phe
{

	PPrimitive::PPrimitive() : PMeshBuffer(nullptr), PerMatBuffer(nullptr), PerObjBuffer(nullptr), Pipeline(nullptr), PrimitiveMaterial(nullptr)
	{

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


	void PPrimitive::UpdateCommonBuffer(PGPUCommonBuffer* Buffer, void* Data)
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