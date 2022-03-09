#include "pch.h"
#include "PAssetManager.h"
#include "PStaticMesh.h"
#include <fstream>
#include <json.h>

namespace Phe
{

	PAssetManager::PAssetManager()
	{
		MeshData.clear();
	}

	PAssetManager::~PAssetManager()
	{
		MeshData.clear();
	}

	void PAssetManager::LoadJsonFile(const std::string FileName)
	{
		Json::Reader reader;
		Json::Value root;
		std::ifstream in(FileName, std::ifstream::binary);
		if (reader.parse(in, root))
		{
			for (auto it : root)
			{
				std::string MeshName;
				std::vector<float> Vertices;
				std::vector<UINT32> Indices;
				std::vector<float> TangentZs;

				MeshName = it["StaticMeshName"].asString();

				for (auto iVertex : it["Vertices"])
				{
					Vertices.push_back(iVertex.asFloat());
				}

				for (auto iIndex : it["Indices"])
				{
					Indices.push_back(iIndex.asInt());
				}

				for (auto iTangent : it["TangentZ"])
				{
					TangentZs.push_back(iTangent.asFloat());
				}

				std::shared_ptr<Phe::PStaticMesh> Mesh = std::make_shared<Phe::PStaticMesh>(MeshName, Vertices, Indices, TangentZs);
				PMeshDataStruct MeshStruct;
				MeshStruct.Vertices = Vertices;
				MeshStruct.Indices = Indices;
				MeshStruct.Normal = TangentZs;
				MeshData.insert({ MeshName, MeshStruct });
			}
		}
	}


	void PAssetManager::AddMeshData(std::string MeshName, PMeshDataStruct InMeshData)
	{
		MeshData.insert({ MeshName, InMeshData });
	}

	PMeshDataStruct PAssetManager::GetMeshData(std::string MeshName)
	{
		if (MeshData.count(MeshName) > 0)
		{
			return MeshData[MeshName];
		}
		PMeshDataStruct Ret;
		return Ret;
	}

}
