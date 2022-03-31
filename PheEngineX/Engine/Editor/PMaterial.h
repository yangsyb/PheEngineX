#pragma once
#include "pch.h"
#include "PShader.h"
#include "PTexture.h"
#include "GPUResource/PGPUTexture.h"

namespace Phe
{
	class PMaterial
	{
	public:
		PMaterial(std::string MName, std::string SName, std::vector<std::string> TNames, glm::vec4 BaseColor = glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3 FresnelR0 = glm::vec3(0.05f, 0.05f, 0.05f), float Roughness = 0.2f, float Metallic = 0.0f, float Specular = 0.0f);
		virtual ~PMaterial();
		bool operator==(const PMaterial* M) { return this->GetName() == M->PMaterialName; }

		std::string GetName() { return PMaterialName; }
		std::string GetShaderName() { return PShaderName; }
//		std::vector<std::string> GetTextureName() { return PTextureNames; }
		glm::vec4 GetBaseColor() { return PBaseColor; }
		glm::vec3 GetPFresnelR0() { return PFresnelR0; }
		float GetMetallic() { return PMetallic; }
		float GetSpecular() { return PSpecular; }
		float GetRoughness() { return PRoughness; }

		void AddTexture(std::string TextureName);
		void DeleteTextre(std::string TextureName);
		void SetBaseColor(glm::vec4 BaseColor) { PBaseColor = BaseColor; }
		void SetFresnelR0(glm::vec3 FresnelR0) { PFresnelR0 = FresnelR0; }
		void SetMetallic(float Metallic) { PMetallic = Metallic; }
		void SetSpecular(float Specular) { PSpecular = Specular; }
		void SetRoughness(float Roughness) { PRoughness = Roughness; }

		void CompileMaterial();
		UINT32 GetHandleOffset();
		PerMaterialCBuffer GetMaterialBuffer();
//		std::vector<PGPUTexture*> GetGPUTextureBuffer() { return MatTextureBuffer; }
//		void SetGPUTextureBuffer(std::vector<PGPUTexture*> MaterialTexture) { MatTextureBuffer = MaterialTexture; }

		std::unordered_map<std::string, PTexture*> GetTextures() { return MatTextures; }


	protected:
		std::string PMaterialName;
		std::string PShaderName;
//		std::vector<std::string> PTextureNames;

		glm::vec4 PBaseColor;
		glm::vec3 PFresnelR0;
		float PMetallic;
		float PSpecular;
		float PRoughness;

		std::unordered_map<std::string, PTexture*> MatTextures;
//		std::vector<PGPUTexture*> MatTextureBuffer;
		
	};

}