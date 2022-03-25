#pragma once
#include "pch.h"
#include "Engine/Transform.h"
#include "Engine/PStaticMesh.h"
#include "DX12/PDescriptorHeap.h"
#include "GPUResource/PGPUMeshBuffer.h"
#include "GPUResource/PGPUCommonBuffer.h"
#include "PPrimitive.h"
#include "Engine/PLight.h"

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
		void AddLight(std::string LightName, PLightDataStruct LightData);
		void SetLightDynamic(std::string LightName);
		void ClearScene();
		void DestroyRenderScene();
		
		PGPUMeshBuffer* GetMeshBuffer(std::string MeshBufferName);
		std::vector<PPrimitive*> GetPrimitives() { return Primitives; }
		PGPUCommonBuffer* GetLightCommonBuffer(std::string LightName);
		PGPUCommonBuffer* GetMainLightBuffer();
		PLight* GetMainLight();
	private:
		UINT32 PrimitiveNumber;
		std::unordered_map<std::string, PGPUMeshBuffer*> PMeshBufferPool;
		std::unordered_map<std::string, PMaterial*> PMaterialPool;
		std::unordered_map<std::string, PPipeline*> PPipelinePool;
		std::unordered_map<PLight*, PGPUCommonBuffer*> PLightPool;
		std::vector<PShader*> PShaderPool;
		std::vector<PPrimitive*> Primitives;
	private:

	};
}