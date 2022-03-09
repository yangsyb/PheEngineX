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
		PMeshDataStruct GetMeshData(std::string MeshName);

	private:
		std::unordered_map<std::string, PMeshDataStruct> MeshData;
	};
}