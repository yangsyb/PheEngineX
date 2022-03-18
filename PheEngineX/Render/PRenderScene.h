#pragma once
#include "pch.h"
#include "Engine/Transform.h"
#include "Engine/PStaticMesh.h"
#include "DX12/PDescriptorHeap.h"
#include "GPUResource/PGPUMeshBuffer.h"
#include "GPUResource/PGPUCommonBuffer.h"
#include "PPrimitive.h"

namespace Phe
{


	class PRenderScene
	{
	public:
		PRenderScene();
		~PRenderScene();

		void AddMeshBuffer(std::string StaticMeshName, PMeshDataStruct StaticMeshData);
		void AddPrimitive(std::string MeshBufferName, Transform MeshTransform, std::string MaterialName);
		void AddMeshBufferAndPrimitive(std::string StaticMeshName, PMeshDataStruct StaticMeshData, Transform MeshTransform, std::string MaterialName);
		void ClearScene();
		void DestroyRenderScene();
		
		PGPUMeshBuffer* GetMeshBuffer(std::string MeshBufferName);
		std::vector<PPrimitive*> GetPrimitives() { return Primitives; }
	private:
		UINT32 PrimitiveNumber;
		std::unordered_map<std::string, PGPUMeshBuffer*> PMeshBufferPool;
		std::unordered_map<std::string, PMaterial*> PMaterialPool;
		std::unordered_map<std::string, PPipeline*> PPipelinePool;
		std::vector<PShader*> PShaderPool;
		std::vector<PPrimitive*> Primitives;
	private:

	};
}