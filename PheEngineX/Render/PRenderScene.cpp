#include "pch.h"
#include "PRenderScene.h"
#include "PRenderThread.h"
#include "Engine/PAssetManager.h"
#include "RHI/PRHI.h"
#include "Engine/PMaterial.h"
#include "Engine/PShaderManager.h"

namespace Phe
{

	PRenderScene::PRenderScene() : PrimitiveNumber(0), PSceneCenter(glm::vec3(0,0,0)), PSceneRadius(35)
	{

	}

	PRenderScene::~PRenderScene()
	{

	}

	void PRenderScene::AddMeshBuffer(std::string StaticMeshName, PMeshDataStruct StaticMeshData)
	{
		auto it = PMeshBufferPool.find(StaticMeshName);
		if (it == PMeshBufferPool.end())
		{
			PGPUMeshBuffer* InMeshBuffer = PRHI::Get()->CreateMeshBuffer();
			InMeshBuffer->SetMeshBuffer(StaticMeshName, StaticMeshData);
			PRHI::Get()->UpdateMeshBuffer(InMeshBuffer);
			PMeshBufferPool.insert({ StaticMeshName , InMeshBuffer });
		}
	}

	void PRenderScene::AddPrimitive(std::string MeshBufferName, Transform MeshTransform, std::string MaterialName)
	{
		auto MaterialData = PAssetManager::GetSingleton().GetMaterialData(MaterialName);
 		if(PMaterialPool.count(MaterialName) == 0)
 		{
 			PMaterial* NewMaterial = new PMaterial(MaterialName, MaterialData.ShaderName, MaterialData.TextureNames);
			NewMaterial->CompileMaterial();
 			PMaterialPool.insert({ MaterialName , NewMaterial });
 		}
		auto ptr = GetMeshBuffer(MeshBufferName);
		if (ptr)
		{
			PPrimitive* NewPrimitive = new PPrimitive();
			if (PPipelinePool.count(MaterialData.ShaderName) > 0)
			{
				NewPrimitive->SetPipeline(PPipelinePool.at(MaterialData.ShaderName));
			}
			else
			{
				auto Shader = PShaderManager::Get()->GetShaderByName(MaterialData.ShaderName);
				PShader* NewShader;
				if(!Shader)
				{
					NewShader = PRHI::Get()->CreateShader(MaterialData.ShaderName, L"Shaders\\color.hlsl");
				}
				else
				{
					NewShader = PRHI::Get()->CreateShader(MaterialData.ShaderName, Shader->GetShaderFilePath());
				}
				PPipeline* NewPipeline = PRHI::Get()->CreatePipeline(NewShader);
				PRHI::Get()->UpdatePipeline(NewPipeline);
				PPipelinePool.insert({ MaterialData.ShaderName, NewPipeline });
				NewPrimitive->SetPipeline(NewPipeline);
				PShaderPool.push_back(NewShader);
			}
			auto Obj = PRHI::Get()->CreateCommonBuffer(sizeof(PerObjectCBuffer), 1);
			auto Mat = PRHI::Get()->CreateCommonBuffer(sizeof(PerMaterialCBuffer), 1);
			NewPrimitive->SetPrimitiveRenderData(ptr, MeshTransform, Obj, Mat, PMaterialPool.at(MaterialName));
			Primitives.push_back(NewPrimitive);
		}
		PrimitiveNumber++;
	}

	void PRenderScene::AddMeshBufferAndPrimitive(std::string StaticMeshName, PMeshDataStruct StaticMeshData, Transform MeshTransform, std::string MaterialName)
	{
		AddMeshBuffer(StaticMeshName, StaticMeshData);
		AddPrimitive(StaticMeshName, MeshTransform, MaterialName);
	}

	void PRenderScene::AddLight(std::string LightName, PLightDataStruct LightData)
	{
		PGPUCommonBuffer* LightCommonBuffer = PRHI::Get()->CreateCommonBuffer(sizeof(PerCameraCBuffer), 1);
		PLight* Light = new PLight(LightName, Transform(LightData.Position, LightData.Rotation, LightData.Scale), LightData.LightRadius, LightData.LightStrength);
		PLightPool.insert({ Light, LightCommonBuffer});
	}

	void PRenderScene::SetLightDynamic(std::string LightName)
	{
		for(auto it : PLightPool)
		{
			if(it.first->GetLightName() == LightName)
			{
				it.first->SetIsDynamic(!it.first->GetIsDynamic());
				return;
			}
		}
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

	PGPUCommonBuffer* PRenderScene::GetLightCommonBuffer(std::string LightName)
	{
		for(auto it : PLightPool)
		{
			if(it.first->GetLightName() == LightName)
			{
				return it.second;
			}
		}
		return nullptr;
	}

	PGPUCommonBuffer* PRenderScene::GetMainLightBuffer()
	{
		if(PLightPool.size()>0)
		{
			return PLightPool.begin()->second;
		}
		return nullptr;
	}

	Phe::PLight* PRenderScene::GetMainLight()
	{
		if (PLightPool.size() > 0)
		{
			return PLightPool.begin()->first;
		}
		return nullptr;
	}

}

