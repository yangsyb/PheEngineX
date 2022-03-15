#include "pch.h"
#include "PAssetManager.h"
#include "PStaticMesh.h"
#include "RHI/PShaderManager.h"
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
				std::vector<float> UVs;

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

				for (auto uv : it["UV"])
				{
					UVs.push_back(uv.asFloat());
				}

				std::shared_ptr<Phe::PStaticMesh> Mesh = std::make_shared<Phe::PStaticMesh>(MeshName, Vertices, Indices, TangentZs, UVs);
				PMeshDataStruct MeshStruct;
				MeshStruct.Vertices = Vertices;
				MeshStruct.Indices = Indices;
				MeshStruct.Normal = TangentZs;
				MeshStruct.UVs = UVs;
				MeshData.insert({ MeshName, MeshStruct });
			}
		}
	}


	void PAssetManager::AddMeshData(std::string MeshName, PMeshDataStruct InMeshData)
	{
		MeshData.insert({ MeshName, InMeshData });
	}

	void PAssetManager::AddTextureData(const std::string TextureName, const std::wstring TFileName)
	{
		std::shared_ptr<PTexture> InTexture = std::make_shared<PTexture>(TextureName, TFileName);
		TextureData.insert({ TextureName, InTexture });
	}

	void PAssetManager::AddMaterialData(std::string MaterialName, std::string ShaderName, std::string TextureName, glm::vec4 DiffuseAlbedo /*= glm::vec4(1.f, 1.f, 1.f, 1.f)*/, glm::vec3 FresnelR0 /*= glm::vec3(0.05f, 0.05f, 0.05f)*/, float Roughness /*= 0.2f*/)
	{
		std::shared_ptr<PMaterial> InMaterial = std::make_shared<PMaterial>(MaterialName, PShaderManager::GetSingleton().GetShader(ShaderName), TextureName, DiffuseAlbedo, FresnelR0, Roughness);
		MaterialData.insert({ MaterialName , InMaterial });
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

	std::shared_ptr<Phe::PTexture> PAssetManager::GetTextureData(std::string TextureName)
	{
		if (TextureData.count(TextureName) > 0)
		{
			return TextureData[TextureName];
		}
		return nullptr;
	}

	std::shared_ptr<Phe::PMaterial> PAssetManager::GetMaterialData(std::string MaterialName)
	{
		if (MaterialData.count(MaterialName) > 0)
		{
			return MaterialData[MaterialName];
		}
		return nullptr;
	}

}
