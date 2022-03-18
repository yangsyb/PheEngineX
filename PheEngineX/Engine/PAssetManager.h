#pragma once
#include "pch.h"

namespace Phe
{
	class PAssetManager : public Singleton<PAssetManager>
	{
	public:
		PAssetManager();
		~PAssetManager();

		void LoadJsonFile(const std::string FileName);
		void AddMeshData(std::string MeshName, PMeshDataStruct InMeshData);
		void AddTextureData(const std::string TextureName, PTextureDataStruct InTextureData);
		void AddTextureData(const std::string TextureName, const std::wstring TFileName);
		void AddMaterialData(const std::string MaterialName, PMaterialDataStruct InMaterialData);
		void AddMaterialData(const std::string MaterialName, std::string ShaderName, std::vector<std::string> TextureNames);

		PMeshDataStruct GetMeshData(std::string MeshName);
		PTextureDataStruct GetTextureData(std::string TextureName);
		PMaterialDataStruct GetMaterialData(std::string MaterialName);
	private:
		std::unordered_map<std::string, PMeshDataStruct> MeshData;
		std::unordered_map<std::string, PTextureDataStruct> TextureData;
		std::unordered_map<std::string, PMaterialDataStruct> MaterialData;
	};
}