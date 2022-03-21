#pragma once
#include "pch.h"
#include "Engine/PShader.h"
#include "GPUResource/PGPUTexture.h"

namespace Phe
{
	class PMaterial
	{
	public:
		PMaterial(std::string MName, std::string SName, std::vector<std::string> TNames, glm::vec4 DiffuseAlbedo = glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3 FresnelR0 = glm::vec3(0.05f, 0.05f, 0.05f), float Roughness = 0.2f, float Metallic = 0.0f, float Specular = 0.0f);
		virtual ~PMaterial();
		bool operator==(const PMaterial* M) { return this->GetName() == M->PMaterialName; }

		std::string GetName() { return PMaterialName; }
		std::string GetShaderName() { return PShaderName; }
		std::vector<std::string> GetTextureName() { return PTextureNames; }
		glm::vec4 GetDiffuseAlbedo() { return PDiffuseAlbedo; }
		glm::vec3 GetPFresnelR0() { return PFresnelR0; }
		float GetMetallic() { return PMetallic; }
		float GetSpecular() { return PSpecular; }
		float GetRoughness() { return PRoughness; }

		void AddTexture(std::string TextureName);
		void DeleteTextre(std::string TextureName);
		void SetDiffuseAlbedo(glm::vec4 DA) { PDiffuseAlbedo = DA; }
		void SetFresnelR0(glm::vec3 FR) { PFresnelR0 = FR; }
		void SetMetallic(float M) { PMetallic = M; }
		void SetSpecular(float S) { PSpecular = S; }
		void SetRoughness(float R) { PRoughness = R; }

		void CompileMaterial();

		PerMaterialCBuffer GetMaterialBuffer();
		std::vector<PGPUTexture*> GetGPUTextureBuffer() { return MatTextureBuffer; }
		void SetGPUTextureBuffer(std::vector<PGPUTexture*> MaterialTexture) { MatTextureBuffer = MaterialTexture; }

	protected:
		std::string PMaterialName;
		std::string PShaderName;
		std::vector<std::string> PTextureNames;

		glm::vec4 PDiffuseAlbedo;
		glm::vec3 PFresnelR0;
		float PMetallic;
		float PSpecular;
		float PRoughness;

		std::vector<PGPUTexture*> MatTextureBuffer;
	};

}