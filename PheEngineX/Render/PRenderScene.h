#pragma once
#include "pch.h"
#include "Engine/Core/Transform.h"
#include "Engine/Editor/PStaticMesh.h"
#include "DX12/PDescriptorHeap.h"
#include "GPUResource/PGPUMeshBuffer.h"
#include "GPUResource/PGPUCommonBuffer.h"
#include "PPrimitive.h"
#include "Engine/Editor/PLight.h"
#include "Engine/Scene/PNodeStaticMesh.h"
#include "Engine/Scene/PNodeLight.h"
#include "PRenderLight.h"

namespace Phe
{


	class PRenderScene
	{
	public:
		PRenderScene();
		~PRenderScene();

		void AddMeshBuffer(std::string StaticMeshName, PNodeStaticMesh* InNodeStaticMesh);
//		void AddPrimitive(std::string MeshBufferName, Transform MeshTransform, std::string MaterialName);
		void AddPrimitive(PNodeStaticMesh* InNodeStaticMesh, PMaterial* StaticMeshMaterial, void* TransformData);
		void AddMeshBufferAndPrimitive(PNodeStaticMesh* InNodeStaticMesh, PMaterial* StaticMeshMaterial, void* TransformData);
		void AddLight(PNodeLight* InNodeLight);
		void AddTexture(PTexture* Texture);
		void AddMaterial(PMaterial* Material);
		void ClearScene();
		void DestroyRenderScene();
		
		void UpdateSceneRadius(glm::vec3 Position);

		PGPUMeshBuffer* GetMeshBuffer(std::string MeshBufferName);
		std::vector<PPrimitive*> GetPrimitives() { return Primitives; }
		PRenderLight* GetRenderLight(std::string LightName);
		PRenderLight* GetMainRenderLight();
	private:

		UINT32 PrimitiveNumber;
		std::unordered_map<std::string, PGPUMeshBuffer*> PMeshBufferPool;
		std::unordered_map<std::string, PMaterial*> PMaterialPool;
		std::unordered_map<std::string, PPipeline*> PPipelinePool;
		std::unordered_map<PLight*, PGPUCommonBuffer*> PLightPool;
		std::unordered_map<std::string, PGPUTexture*> PTexturePool;
		std::vector<PShader*> PShaderPool;
		std::vector<PPrimitive*> Primitives;
		std::vector<PRenderLight*> PRenderLights;
	private:

	};
}