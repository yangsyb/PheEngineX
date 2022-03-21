#include "pch.h"
#include "PMaterial.h"
#include "RHI/PRHI.h"

namespace Phe
{



	PMaterial::PMaterial(std::string MName, std::string SName, std::vector<std::string> TNames, glm::vec4 DiffuseAlbedo /*= glm::vec4(1.f, 1.f, 1.f, 1.f)*/, glm::vec3 FresnelR0 /*= glm::vec3(0.05f, 0.05f, 0.05f)*/, float Roughness /*= 0.2f*/, float Metallic /*= 0.0f*/, float Specular /*= 0.0f*/) :
		PMaterialName(MName), PShaderName(SName), PTextureNames(TNames), PDiffuseAlbedo(DiffuseAlbedo), PFresnelR0(FresnelR0), PMetallic(Metallic), PSpecular(Specular), PRoughness(Roughness)
	{

	}

	PMaterial::~PMaterial()
	{

	}

	void PMaterial::AddTexture(std::string TextureName)
	{
		for (auto it : PTextureNames)
		{
			if (it == TextureName)
			{
				return;
			}
		}
		PTextureNames.push_back(TextureName);
		PRHI::Get()->AddTextureToMaterial(this, TextureName);
	}

	void PMaterial::DeleteTextre(std::string TextureName)
	{
		PRHI::Get()->DeleteTexturefromMaterial(this, TextureName);
		for (auto it = PTextureNames.begin(); it != PTextureNames.end(); it++)
		{
			if (*it == TextureName)
			{
				PTextureNames.erase(it);
			}
			return;
		}
	}

	void PMaterial::CompileMaterial()
	{
		PRHI::Get()->CompileMaterial(this);
	}

	PerMaterialCBuffer PMaterial::GetMaterialBuffer()
	{
		PerMaterialCBuffer Ret;
		Ret.DiffuseAlbedo = PDiffuseAlbedo;
		Ret.FresnelR0 = PFresnelR0;
		Ret.Roughness = PRoughness;
		return Ret;
	}

}