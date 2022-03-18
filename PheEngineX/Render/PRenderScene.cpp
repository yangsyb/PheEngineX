#include "pch.h"
#include "PRenderScene.h"
#include "PRenderThread.h"
#include "Engine/PAssetManager.h"
#include "RHI/PRHI.h"
#include "Engine/PMaterial.h"
#include "Engine/PShaderManager.h"


namespace Phe
{

	PRenderScene::PRenderScene() : PrimitiveNumber(0)
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
			NewMaterial->ReCompileMaterial();
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
				PRHI::Get()->SetPipeline(NewPipeline);
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

	void PRenderScene::ClearScene()
	{
		for (int index = 0; index < Primitives.size(); index++)
		{
			Primitives[index]->DestroyPrimitive();
			delete Primitives[index];
			Primitives[index] = nullptr;
		}
		Primitives.clear();
	}

	void PRenderScene::DestroyRenderScene()
	{
		for(auto it : PMeshBufferPool)
		{
			delete it.second;
			it.second = nullptr;
		}
		PMeshBufferPool.clear();
 		for(auto it : PMaterialPool)
 		{
 			delete it.second;
 			it.second = nullptr;
 		}
 		PMaterialPool.clear();
		for(auto it : PPipelinePool)
		{
			delete it.second;
			it.second = nullptr;
		}
		PPipelinePool.clear();
		for(int index = 0; index < Primitives.size(); index++)
		{
			Primitives[index]->DestroyPrimitive();
			delete Primitives[index];
			Primitives[index] = nullptr;
		}
		for(int index = 0; index < PShaderPool.size(); index++)
		{
			delete PShaderPool[index];
			PShaderPool[index] = nullptr;
		}
		Primitives.clear();
	}

	PGPUMeshBuffer* PRenderScene::GetMeshBuffer(std::string MeshBufferName)
	{
		if (PMeshBufferPool.find(MeshBufferName) != PMeshBufferPool.end())
		{
			return PMeshBufferPool.at(MeshBufferName);
		}
		return nullptr;
	}

}

