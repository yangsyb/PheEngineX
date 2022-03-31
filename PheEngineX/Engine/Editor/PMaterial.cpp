#include "pch.h"
#include "PMaterial.h"
#include "RHI/PRHI.h"
#include "PAssetManager.h"
#include "Engine/Core/PTask.h"
#include "Render/PRenderThread.h"
namespace Phe
{



	PMaterial::PMaterial(std::string MName, std::string SName, std::vector<std::string> TNames, glm::vec4 BaseColor /*= glm::vec4(1.f, 1.f, 1.f, 1.f)*/, glm::vec3 FresnelR0 /*= glm::vec3(0.05f, 0.05f, 0.05f)*/, float Roughness /*= 0.2f*/, float Metallic /*= 0.0f*/, float Specular /*= 0.0f*/) :
		PMaterialName(MName), PShaderName(SName), PBaseColor(BaseColor), PFresnelR0(FresnelR0), PMetallic(Metallic), PSpecular(Specular), PRoughness(Roughness)
	{
		for(auto& TextureName : TNames)
		{
			MatTextures.insert({TextureName, PAssetManager::GetSingleton().GetTextureData(TextureName) });
		}
	}

	PMaterial::~PMaterial()
	{

	}

	void PMaterial::AddTexture(std::string TextureName)
	{
		if(MatTextures.count(TextureName) == 0)
		{
			MatTextures.insert({TextureName, PAssetManager::GetSingleton().GetTextureData(TextureName)});
		}
//		PRHI::Get()->AddTextureToMaterial(this, TextureName);
	}

	void PMaterial::DeleteTextre(std::string TextureName)
	{
//		PRHI::Get()->DeleteTexturefromMaterial(this, TextureName);
		if(MatTextures.count(TextureName)>0)
		{
			MatTextures.erase(TextureName);
		}
	}

	void PMaterial::CompileMaterial()
	{
		for(auto& Texture : MatTextures)
		{
			Texture.second->BindMaterial(this);
		}
		PTask* task = CreateTask(PTask, PRenderThread::Get()->GetRenderScene()->AddMaterial(this));
		PRenderThread::Get()->AddTask(task);
	//	PRHI::Get()->CompileMaterial(this);
	}

	UINT32 PMaterial::GetHandleOffset()
	{
		return MatTextures.begin()->second->GetGPUTexture()->GetHandleOffset();
	}

	PerMaterialCBuffer PMaterial::GetMaterialBuffer()
	{
		PerMaterialCBuffer Ret;
		Ret.BaseColor = PBaseColor;
		Ret.FresnelR0 = PFresnelR0;
		Ret.Roughness = PRoughness;
		return Ret;
	}

}