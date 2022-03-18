#include "pch.h"
#include "PPrimitive.h"

namespace Phe
{

	PPrimitive::PPrimitive() : PMeshBuffer(nullptr), PerMatBuffer(nullptr), PerObjBuffer(nullptr), Pipeline(nullptr), PrimitiveMaterial(nullptr)
	{

	}

	PPrimitive::~PPrimitive()
	{

	}

	void PPrimitive::SetPrimitiveRenderData(PGPUMeshBuffer* MeshBuffer, Transform MeshTransform, PGPUCommonBuffer* ObjBuffer, PGPUCommonBuffer* MatBuffer, PMaterial* MaterialBuffer)
	{
		PMeshBuffer = MeshBuffer;
		PTransform = MeshTransform;
		PerObjBuffer = ObjBuffer;
		PerMatBuffer = MatBuffer;
		PrimitiveMaterial = MaterialBuffer;
	}


	void PPrimitive::DestroyPrimitive()
	{
 		PMeshBuffer = nullptr;
		delete PerObjBuffer;
		PerObjBuffer = nullptr;
		delete PerMatBuffer;
		PerMatBuffer = nullptr;
	}
}