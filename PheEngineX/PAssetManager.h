#pragma once
#include "pch.h"
#include "RHI/PTexture.h"
#include "RHI/PMaterial.h"

namespace Phe
{
	class PAssetManager : public Singleton<PAssetManager>
	{
	public:
		PAssetManager();
		~PAssetManager();

		void LoadJsonFile(const std::string FileName);
		void AddMeshData(std::string MeshName, PMeshDataStruct InMeshData);
		void AddTextureData(const std::string TextureName, const std::wstring TFileName);
		void AddMaterialData(std::string MaterialName, std::string ShaderName, std::string TextureName, glm::vec4 DiffuseAlbedo = glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3 FresnelR0 = glm::vec3(0.05f, 0.05f, 0.05f), float Roughness = 0.2f);

		PMeshDataStruct GetMeshData(std::string MeshName);
		std::shared_ptr<PTexture> GetTextureData(std::string TextureName);
		std::shared_ptr<PMaterial> GetMaterialData(std::string MaterialName);
	private:
		std::unordered_map<std::string, PMeshDataStruct> MeshData;
		std::unordered_map<std::string, std::shared_ptr<PTexture>> TextureData;
		std::unordered_map<std::string, std::shared_ptr<PMaterial>> MaterialData;
	};
}