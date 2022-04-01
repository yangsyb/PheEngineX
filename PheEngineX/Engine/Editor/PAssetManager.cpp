#include "pch.h"
#include "PAssetManager.h"
#include "PStaticMesh.h"
#include <fstream>
#include <json.h>

namespace Phe
{

	PAssetManager::PAssetManager()
	{

	}

	PAssetManager::~PAssetManager()
	{
		for (auto& Mesh : MeshData)
		{
			ReleasePtr(Mesh.second);
		}
		for (auto& Light : LightData)
		{
			ReleasePtr(Light.second);
		}
		for (auto& Material : MaterialData)
		{
			ReleasePtr(Material.second);
		}
		for (auto& Texture : TextureData)
		{
			ReleasePtr(Texture.second);
		}
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
				std::vector<UINT16> Indices;
				std::vector<float> TangentZs;
				std::vector<float> UVs;

				MeshName = it["StaticMeshName"].asString();
				if (MeshData.count(MeshName) == 0)
				{
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

					PStaticMesh* Mesh = new PStaticMesh(MeshName, Vertices, Indices, TangentZs, UVs);
					MeshData.insert({ MeshName, Mesh });
				}
			}
		}
	}


	void PAssetManager::AddMeshData(std::string MeshName, PStaticMesh* InStaticMesh)
	{
		if (MeshData.count(MeshName) == 0)
		{
			MeshData.insert({ MeshName, InStaticMesh });
		}
	}

	void PAssetManager::AddMeshData(const std::string MeshName, std::vector<float> PVertices, std::vector<UINT16> PIndices, std::vector<float> PTangents, std::vector<float> PUVs)
	{
		if (MeshData.count(MeshName) == 0)
		{
			PStaticMesh* Mesh = new PStaticMesh(MeshName, PVertices, PIndices, PTangents, PUVs);
			MeshData.insert({ MeshName, Mesh });
		}
	}

	void PAssetManager::AddTextureData(const std::string TextureName, PTexture* InTexture)
	{
		if (TextureData.count(TextureName) == 0)
		{
			TextureData.insert({ TextureName, InTexture });
		}
	}


	void PAssetManager::AddTextureData(const std::string TextureName, const std::wstring TFileName)
	{
		if (TextureData.count(TextureName) == 0)
		{
			PTexture* NewTexture = new PTexture(TextureName, TFileName);
			TextureData.insert({ TextureName, NewTexture });
		}
	}

	void PAssetManager::AddMaterialData(const std::string MaterialName, PMaterial* InMaterial)
	{
		if (MaterialData.count(MaterialName) == 0)
		{
			MaterialData.insert({ MaterialName , InMaterial });
		}
	}

	void PAssetManager::AddMaterialData(const std::string MaterialName, std::string ShaderName, std::vector<std::string> TextureNames)
	{
		if (MaterialData.count(MaterialName) == 0)
		{
			PMaterial* NewMaterial = new PMaterial(MaterialName, ShaderName, TextureNames);
			MaterialData.insert({ MaterialName , NewMaterial });
		}
	}

	void PAssetManager::AddLightData(const std::string LightName)
	{
		if (LightData.count(LightName) == 0)
		{
			PLight* NewLight = new PLight(LightName);
			LightData.insert({ LightName, NewLight });
		}
	}

	PStaticMesh* PAssetManager::GetMeshData(std::string MeshName)
	{
		if (MeshData.count(MeshName) > 0)
		{
			return MeshData[MeshName];
		}
		return nullptr;
	}

	PTexture* PAssetManager::GetTextureData(std::string TextureName)
	{
		if (TextureData.count(TextureName) > 0)
		{
			return TextureData[TextureName];
		}
		return nullptr;
	}

	PMaterial* PAssetManager::GetMaterialData(std::string MaterialName)
	{
		if (MaterialData.count(MaterialName) > 0)
		{
			return MaterialData[MaterialName];
		}
		return nullptr;
	}

	PLight* PAssetManager::GetLightData(std::string LightName)
	{
		if (LightData.count(LightName) > 0)
		{
			return LightData[LightName];
		}
		return nullptr;
	}

}
