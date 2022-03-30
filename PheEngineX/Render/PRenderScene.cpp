#include "pch.h"
#include "PRenderScene.h"
#include "PRenderThread.h"
#include "Engine/Editor/PAssetManager.h"
#include "RHI/PRHI.h"
#include "Engine/Editor/PMaterial.h"
#include "Engine/Editor/PShaderManager.h"

namespace Phe
{

	PRenderScene::PRenderScene() : PrimitiveNumber(0)
	{

	}

	PRenderScene::~PRenderScene()
	{

	}

	void PRenderScene::AddMeshBuffer(std::string StaticMeshName, PNodeStaticMesh* InNodeStaticMesh)
	{
		if(PMeshBufferPool.count(StaticMeshName) == 0)
		{
			PStaticMesh* StaticMeshData = PAssetManager::GetSingleton().GetMeshData(InNodeStaticMesh->GetStaticMeshName());
			PGPUMeshBuffer* InMeshBuffer = PRHI::Get()->CreateMeshBuffer();
			InMeshBuffer->SetMeshBuffer(StaticMeshName, StaticMeshData);
			PRHI::Get()->UpdateMeshBuffer(InMeshBuffer);
			PMeshBufferPool.insert({ StaticMeshName , InMeshBuffer });
		}
	}

// 	void PRenderScene::AddPrimitive(std::string MeshBufferName, Transform MeshTransform, std::string MaterialName)
// 	{
// 		auto MaterialData = PAssetManager::GetSingleton().GetMaterialData(MaterialName);
//  		if(PMaterialPool.count(MaterialName) == 0)
//  		{
//  			PMaterial* NewMaterial = new PMaterial(MaterialName, MaterialData.ShaderName, MaterialData.TextureNames);
// 			NewMaterial->CompileMaterial();
//  			PMaterialPool.insert({ MaterialName , NewMaterial });
//  		}
// 		auto ptr = GetMeshBuffer(MeshBufferName);
// 		if (ptr)
// 		{
// 			PPrimitive* NewPrimitive = new PPrimitive();
// 			if (PPipelinePool.count(MaterialData.ShaderName) > 0)
// 			{
// 				NewPrimitive->SetPipeline(PPipelinePool.at(MaterialData.ShaderName));
// 			}
// 			else
// 			{
// 				auto Shader = PShaderManager::Get()->GetShaderByName(MaterialData.ShaderName);
// 				PShader* NewShader;
// 				if(!Shader)
// 				{
// 					NewShader = PRHI::Get()->CreateShader(MaterialData.ShaderName, L"Shaders\\color.hlsl");
// 				}
// 				else
// 				{
// 					NewShader = PRHI::Get()->CreateShader(MaterialData.ShaderName, Shader->GetShaderFilePath());
// 				}
// 				PPipeline* NewPipeline = PRHI::Get()->CreatePipeline(NewShader);
// 				PRHI::Get()->UpdatePipeline(NewPipeline);
// 				PPipelinePool.insert({ MaterialData.ShaderName, NewPipeline });
// 				NewPrimitive->SetPipeline(NewPipeline);
// 				PShaderPool.push_back(NewShader);
// 			}
// 			auto Obj = PRHI::Get()->CreateCommonBuffer(sizeof(PerObjectCBuffer), 1);
// 			auto Mat = PRHI::Get()->CreateCommonBuffer(sizeof(PerMaterialCBuffer), 1);
// 			NewPrimitive->SetPrimitiveRenderData(ptr, MeshTransform, Obj, Mat, PMaterialPool.at(MaterialName));
// 			Primitives.push_back(NewPrimitive);
// 		}
// 		PrimitiveNumber++;
// 	}

	void PRenderScene::AddPrimitive(PNodeStaticMesh* InNodeStaticMesh, PMaterial* StaticMeshMaterial, void* TransformData)
	{
		std::string MaterialName = StaticMeshMaterial->GetName();
		if (PMaterialPool.count(MaterialName) == 0)
		{
			StaticMeshMaterial->CompileMaterial();
			PMaterialPool.insert({ MaterialName , StaticMeshMaterial });
		}
		auto ptr = GetMeshBuffer(InNodeStaticMesh->GetStaticMeshName());
		if (ptr)
		{
			PPrimitive* NewPrimitive = new PPrimitive();
			InNodeStaticMesh->SetLinkedPrimitive(NewPrimitive);
			if (PPipelinePool.count(StaticMeshMaterial->GetShaderName()) > 0)
			{
				NewPrimitive->SetPipeline(PPipelinePool.at(StaticMeshMaterial->GetShaderName()));
			}
			else
			{
				auto Shader = PShaderManager::Get()->GetShaderByName(StaticMeshMaterial->GetShaderName());
				PShader* NewShader;
				std::string ShaderName = StaticMeshMaterial->GetShaderName();
				if (!Shader)
				{
					NewShader = PRHI::Get()->CreateShader(ShaderName, L"Shaders\\color.hlsl");
				}
				else
				{
					NewShader = PRHI::Get()->CreateShader(ShaderName, Shader->GetShaderFilePath());
				}
				PPipeline* NewPipeline = PRHI::Get()->CreatePipeline(NewShader);
				PRHI::Get()->UpdatePipeline(NewPipeline);
				PPipelinePool.insert({ ShaderName, NewPipeline });
				NewPrimitive->SetPipeline(NewPipeline);
				PShaderPool.push_back(NewShader);
			}
			auto Obj = PRHI::Get()->CreateCommonBuffer(sizeof(PerObjectCBuffer), 1);
			auto Mat = PRHI::Get()->CreateCommonBuffer(sizeof(PerMaterialCBuffer), 1);
			PRHI::Get()->UpdateCommonBuffer(Obj, TransformData);
			NewPrimitive->SetPrimitiveRenderData(ptr, Obj, Mat, PMaterialPool.at(MaterialName));
			Primitives.push_back(NewPrimitive);
		}
		PrimitiveNumber++;
	}

	void PRenderScene::AddMeshBufferAndPrimitive(PNodeStaticMesh* InNodeStaticMesh, PMaterial* StaticMeshMaterial, void* TransformData)
	{
		auto StaticMeshName = InNodeStaticMesh->GetStaticMeshName();
		AddMeshBuffer(StaticMeshName, InNodeStaticMesh);
		AddPrimitive(InNodeStaticMesh, StaticMeshMaterial, TransformData);
	}


	void PRenderScene::AddLight(PNodeLight* InNodeLight)
	{
		PRenderLight* NewRenderLight = new PRenderLight();
		InNodeLight->SetLinkedLight(NewRenderLight);
		PRenderLights.push_back(NewRenderLight);
	}

	void PRenderScene::ClearScene()
	{
		for (int index = 0; index < Primitives.size(); index++)
		{
			Primitives[index]->DestroyPrimitive();
			ReleasePtr(Primitives[index]);
		}
		Primitives.clear();
	}

	void PRenderScene::DestroyRenderScene()
	{
		for(auto it : PMeshBufferPool)
		{
			ReleasePtr(it.second);
		}
		PMeshBufferPool.clear();
 		for(auto it : PMaterialPool)
 		{

			ReleasePtr(it.second);
 		}
 		PMaterialPool.clear();
		for(auto it : PPipelinePool)
		{
			ReleasePtr(it.second);
		}
		PPipelinePool.clear();
		for(int index = 0; index < Primitives.size(); index++)
		{
			Primitives[index]->DestroyPrimitive();
			ReleasePtr(Primitives[index]);
		}
		for(auto it : PLightPool)
		{
			delete it.first;
			ReleasePtr(it.second);
		}
		for(int index = 0; index < PShaderPool.size(); index++)
		{
			ReleasePtr(PShaderPool[index]);
		}
		Primitives.clear();
	}

	void PRenderScene::UpdateSceneRadius(glm::vec3 Position)
	{

	}

	PGPUMeshBuffer* PRenderScene::GetMeshBuffer(std::string MeshBufferName)
	{
		if (PMeshBufferPool.find(MeshBufferName) != PMeshBufferPool.end())
		{
			return PMeshBufferPool.at(MeshBufferName);
		}
		return nullptr;
	}

	PRenderLight* PRenderScene::GetRenderLight(std::string LightName)
	{
		return nullptr;
	}

	PRenderLight* PRenderScene::GetMainRenderLight()
	{
		if(PRenderLights.size())
		{
			return PRenderLights[0];
		}
		return nullptr;
	}

}

