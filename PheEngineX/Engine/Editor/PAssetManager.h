#pragma once
#include "pch.h"
#include "PStaticMesh.h"
#include "PTexture.h"
#include "PMaterial.h"
#include "PLight.h"

namespace Phe
{
	class PAssetManager : public Singleton<PAssetManager>
	{
	public:
		PAssetManager();
		~PAssetManager();

		void LoadJsonFile(const std::string FileName);
		void AddMeshData(const std::string MeshName, PStaticMesh* InStaticMesh);
		void AddMeshData(const std::string MeshName, std::vector<float> PVertices, std::vector<UINT16> PIndices, std::vector<float> PTangents, std::vector<float> PUVs);
		void AddTextureData(const std::string TextureName, PTexture* InTexture);
		void AddTextureData(const std::string TextureName, const std::wstring TFileName);
		void AddMaterialData(const std::string MaterialName, PMaterial* InMaterial);
		void AddMaterialData(const std::string MaterialName, std::string ShaderName, std::vector<std::string> TextureNames);
		void AddLightData(const std::string LightName);

		PStaticMesh* GetMeshData(std::string MeshName);
		PTexture* GetTextureData(std::string TextureName);
		PMaterial* GetMaterialData(std::string MaterialName);
		PLight* GetLightData(std::string LightName);
	private:

		std::unordered_map<std::string, PStaticMesh*> MeshData;
		std::unordered_map<std::string, PTexture*> TextureData;
		std::unordered_map<std::string, PMaterial*> MaterialData;

		std::unordered_map<std::string, PLight*> LightData;
	};
}