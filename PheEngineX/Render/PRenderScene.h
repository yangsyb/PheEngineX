#pragma once
#include "pch.h"
#include "Engine/Core/Transform.h"
#include "Engine/Editor/PStaticMesh.h"
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
		void AddPrimitive(PNodeStaticMesh* InNodeStaticMesh, PMaterial* StaticMeshMaterial, Transform TransformData);
		void AddMeshBufferAndPrimitive(PNodeStaticMesh* InNodeStaticMesh, PMaterial* StaticMeshMaterial, Transform TransformData);
		void AddLight(PNodeLight* InNodeLight);
		void AddTexture(PTexture* Texture);
		void AddMaterial(PMaterial* Material);

		void DestroyRenderScene();
		
		void DestroyScenePrimitive(std::string PrimitiveId);
		void DestroySceneLight(std::string SceneLightId);
		void DestroySceneMaterial(std::string MaterialName);
		void DestroySceneTexture(std::string TextureName);

		PGPUMeshBuffer* GetMeshBuffer(std::string MeshBufferName);
		PHashMap<std::string, PPrimitive*> GetPrimitives() { return PPrimitivePool; }
		PRenderLight* GetRenderLight(std::string LightName);
		PRenderLight* GetMainRenderLight();
	private:
		UINT32 PrimitiveNumber;
		PHashMap<std::string, PGPUMeshBuffer*> PMeshBufferPool;
		PHashMap<std::string, PMaterial*> PMaterialPool;
//		PHashMap<std::string, PPipeline*> PPipelinePool;
		PHashMap<std::string, PVector<PPipeline*>> PPipelinePool;
		PHashMap<std::string, PGPUTexture*> PTexturePool;
		PHashMap<std::string, PPrimitive*> PPrimitivePool;
		PHashMap<std::string, PRenderLight*> PRenderLightPool;
		PVector<PShader*> PShaderPool;
	};
}