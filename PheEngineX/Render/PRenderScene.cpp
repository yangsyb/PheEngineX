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
		AddMaterial(StaticMeshMaterial);

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
  			PPrimitivePool.insert({ InNodeStaticMesh->GetID(), NewPrimitive});
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
		PRenderLightPool.insert( {InNodeLight->GetID(), NewRenderLight} );
	}

	void PRenderScene::AddTexture(PTexture* Texture)
	{
		std::string TextureName = Texture->GetTextureName();
		if(PTexturePool.count(TextureName) == 0)
		{
			PGPUTexture* NewGPUTexture = PRHI::Get()->CreateTexture(TextureName, Texture->GetTextureFileName());
			Texture->BindGPUTexture(NewGPUTexture);
			PTexturePool.insert({ TextureName, NewGPUTexture });
		}
	}

	void PRenderScene::AddMaterial(PMaterial* Material)
	{
		if(PMaterialPool.count(Material->GetName()) == 0)
		{
//			PMaterial* NewMaterial = new PMaterial(Material);
			auto Textures = Material->GetTextures();
			for(auto& Texture : Textures)
			{
				AddTexture(Texture.second);
				Texture.second->BindMaterial(Material);
			}
			PMaterialPool.insert( {Material->GetName(), Material } );
		}
	}

	void PRenderScene::DestroyRenderScene()
	{
		for(auto& it : PMeshBufferPool)
		{
			ReleasePtr(it.second);
		}
		PMeshBufferPool.clear();
		for(auto& it : PPipelinePool)
		{
			ReleasePtr(it.second);
		}
		PPipelinePool.clear();

// 		for(auto& it : PPrimitivePool)
// 		{
// 			it.second->DestroyPrimitive();
// 			ReleasePtr(it.second);
// 		}
// 		PPrimitivePool.clear();


		for(int index = 0; index < PShaderPool.size(); index++)
		{
			ReleasePtr(PShaderPool[index]);
		}
		PShaderPool.clear();
	}


	void PRenderScene::DestroyScenePrimitive(std::string PrimitiveId)
	{
		if(PPrimitivePool.count(PrimitiveId))
		{
			PPrimitivePool.at(PrimitiveId)->DestroyPrimitive();
			PPrimitivePool.erase(PrimitiveId);
			return;
		}
	}

	void PRenderScene::DestroySceneLight(std::string SceneLightId)
	{
		if(PRenderLightPool.count(SceneLightId))
		{
			PRenderLightPool.at(SceneLightId)->DestroyRenderLight();
			PRenderLightPool.erase(SceneLightId);
			return;
		}
	}

	void PRenderScene::DestroySceneMaterial(std::string MaterialName)
	{
		if(PMaterialPool.count(MaterialName) > 0)
		{
			PMaterialPool.erase(MaterialName);
			return;
		}
	}

	void PRenderScene::DestroySceneTexture(std::string TextureName)
	{
		if(PTexturePool.count(TextureName) > 0)
		{
			ReleasePtr(PTexturePool.at(TextureName));
			PTexturePool.erase(TextureName);
			return;
		}
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
		if(PRenderLightPool.size())
		{
			return PRenderLightPool.begin()->second;
		}
		return nullptr;
	}

}

