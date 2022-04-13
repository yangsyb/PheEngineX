#include "pch.h"
#include "PRenderScene.h"
#include "PRenderThread.h"
#include "Engine/Editor/PAssetManager.h"
#include "RHI/PRHI.h"
#include "Engine/Editor/PMaterial.h"
#include "Engine/Editor/PShaderManager.h"

namespace Phe
{

	PRenderScene::PRenderScene() : PrimitiveNumber(0), PSkySphere(nullptr), PSkyBoxPipeline(nullptr)
	{
		for (auto Shader : PShaderManager::Get()->GetAllShaders())
		{
			PShader* NewShader = PRHI::Get()->CreateShader(Shader.second->GetShaderName(), Shader.second->GetShaderFilePath(), Shader.second->GetVSEntry(), Shader.second->GetPSEntry());
			PShaderPool.insert({ Shader.second->GetShaderName(), NewShader });
		}
	}

	PRenderScene::~PRenderScene()
	{
		ReleasePtr(PSkyBoxPipeline);
		PSkySphere->DestroyPrimitive();
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


	void PRenderScene::AddPrimitive(PNodeStaticMesh* InNodeStaticMesh, PMaterial* StaticMeshMaterial, Transform TransformData)
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
 				PVector<PPipeline*> RetPipelines;
 				RetPipelines.resize(static_cast<int>(PipelineType::PipelineCount));
 				std::string ShaderName = StaticMeshMaterial->GetShaderName();
 				for (int size = 0; size < static_cast<int>(PipelineType::PipelineCount); size++)
 				{
 					PPipeline* NewPipeline = nullptr;
 					switch (size)
 					{
   					case static_cast<int>(PipelineType::BasePipeline):
   						NewPipeline = PRHI::Get()->CreatePipeline(PShaderPool.at(ShaderName), P_RasterizerDesc(), P_BlendState(), P_DepthStencilState());
//						NewPipeline = PRHI::Get()->CreatePipeline(PShaderManager::Get()->GetShaderByName(ShaderName), P_RasterizerDesc(), P_BlendState(), P_DepthStencilState());
   						PRHI::Get()->UpdatePipeline(NewPipeline, LDR_FORMAT, DepthStencil_Format);
   						break;
   					case static_cast<int>(PipelineType::ShadowPipeline):
   						NewPipeline = PRHI::Get()->CreatePipeline(PShaderPool.at(ShaderName), P_RasterizerDesc(ShadowRasterizerDesc), P_BlendState(), P_DepthStencilState());
   						PRHI::Get()->UpdatePipeline(NewPipeline, LDR_FORMAT, DepthStencil_Format);
						break;
   					case static_cast<int>(PipelineType::SkyPipeline):
   						NewPipeline = PRHI::Get()->CreatePipeline(PShaderPool.at(ShaderName), P_RasterizerDesc(), P_BlendState(), P_DepthStencilState());
   						PRHI::Get()->UpdatePipeline(NewPipeline, LDR_FORMAT, DepthStencil_Format);
   						break;
   					case static_cast<int>(PipelineType::HDRPipeline):
   						NewPipeline = PRHI::Get()->CreatePipeline(PShaderPool.at(ShaderName), P_RasterizerDesc(), P_BlendState(), P_DepthStencilState());
   						PRHI::Get()->UpdatePipeline(NewPipeline, HDR_FORMAT, DepthStencil_Format);
   						break;
 					}
  					RetPipelines[size] = NewPipeline;
 				}
    				PPipelinePool.insert({ ShaderName, RetPipelines });
    				NewPrimitive->SetPipeline(RetPipelines);
			}
			auto Obj = PRHI::Get()->CreateCommonBuffer(sizeof(PerObjectCBuffer), 1);
			auto Mat = PRHI::Get()->CreateCommonBuffer(sizeof(PerMaterialCBuffer), 1);
			std::shared_ptr<void> TransData = std::make_shared<PerObjectCBuffer>(TransformData.GetPositionMat(), TransformData.GetRotaionMat(), TransformData.GetScaleMat());
			std::shared_ptr<void> MatData = std::make_shared<PerMaterialCBuffer>(StaticMeshMaterial->GetMaterialBuffer());
			PRHI::Get()->UpdateCommonBuffer(Obj, TransData);
			PRHI::Get()->UpdateCommonBuffer(Mat, MatData);
			NewPrimitive->SetPrimitiveRenderData(ptr, Obj, Mat, PMaterialPool.at(MaterialName));
			PPrimitivePool.insert({ InNodeStaticMesh->GetID(), NewPrimitive });
			PrimitiveNumber++;
 		}
	}


	void PRenderScene::AddSkySphere(PNodeStaticMesh* InNodeStaticMesh, Transform TransformData, PTexture* Texture)
	{
		if(!PSkySphere)
		{
			auto StaticMeshName = InNodeStaticMesh->GetStaticMeshName();
			AddMeshBuffer(StaticMeshName, InNodeStaticMesh);
			auto ptr = GetMeshBuffer(InNodeStaticMesh->GetStaticMeshName());
			PSkySphere = new PPrimitive();
			InNodeStaticMesh->SetLinkedPrimitive(PSkySphere);

//			PSkyShader = PRHI::Get()->CreateShader(PShaderManager::Get()->GetShaderByName("SkyShader")->GetShaderName(), PShaderManager::Get()->GetShaderByName("SkyShader")->GetShaderFilePath());
			PSkyBoxPipeline = PRHI::Get()->CreatePipeline(PShaderPool.at("SkyShader"), P_RasterizerDesc(), P_BlendState(), P_DepthStencilState());
			PRHI::Get()->UpdatePipeline(PSkyBoxPipeline, LDR_FORMAT, DepthStencil_Format);
			PSkySphere->SetPipeline(PipelineType::SkyPipeline, PSkyBoxPipeline);
			auto Obj = PRHI::Get()->CreateCommonBuffer(sizeof(PerObjectCBuffer), 1);
			std::shared_ptr<void> TransData = std::make_shared<PerObjectCBuffer>(TransformData.GetPositionMat(), TransformData.GetRotaionMat(), TransformData.GetScaleMat());
			PRHI::Get()->UpdateCommonBuffer(Obj, TransData);
			PSkySphere->SetPrimitiveRenderData(ptr, Obj);

			AddTexture(Texture, P_TextureType::P_TextureCube);
		}
	}

	void PRenderScene::AddMeshBufferAndPrimitive(PNodeStaticMesh* InNodeStaticMesh, PMaterial* StaticMeshMaterial, Transform TransformData)
	{
		auto StaticMeshName = InNodeStaticMesh->GetStaticMeshName();
		AddMeshBuffer(StaticMeshName, InNodeStaticMesh);
		AddPrimitive(InNodeStaticMesh, StaticMeshMaterial, TransformData);
	}


	void PRenderScene::AddLight(PNodeLight* InNodeLight)
	{
  		PRenderLight* NewRenderLight = new PRenderLight();
  		InNodeLight->SetLinkedLight(NewRenderLight);
		NewRenderLight->UpdatePerLightBuffer(InNodeLight->GetPerLightCBuffer());
  		PRenderLightPool.insert( {InNodeLight->GetID(), NewRenderLight} );
	}

	void PRenderScene::AddTexture(PTexture* Texture, P_TextureType TextureType)
	{
		std::string TextureName = Texture->GetTextureName();
		if(PTexturePool.count(TextureName) == 0)
		{
			PGPUTexture* NewGPUTexture = PRHI::Get()->CreateTexture(TextureName, Texture->GetTextureFileName(), TextureType);
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
				AddTexture(Texture.second, P_TextureType::P_Texture2D);
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
			for(auto& it2 : it.second)
			{
				ReleasePtr(it2);
			}
		}
		PPipelinePool.clear();

		for (auto& it : PShaderPool)
		{
			ReleasePtr(it.second);
		}
		PShaderPool.clear();
	}


	void PRenderScene::DestroyScenePrimitive(std::string PrimitiveId)
	{
		if(PPrimitivePool.count(PrimitiveId))
		{
			PPrimitivePool.at(PrimitiveId)->DestroyPrimitive();
			ReleasePtr(PPrimitivePool.at(PrimitiveId));
			PPrimitivePool.erase(PrimitiveId);
			return;
		}
	}

	void PRenderScene::DestroySceneLight(std::string SceneLightId)
	{
		if(PRenderLightPool.count(SceneLightId))
		{
			PRenderLightPool.at(SceneLightId)->DestroyRenderLight();
			ReleasePtr(PRenderLightPool.at(SceneLightId));
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
/*			ReleasePtr(PTexturePool.at(TextureName));*/
			PRHI::Get()->DestroyTexture(PTexturePool.at(TextureName));
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

