#pragma once

#include <fstream>
#include <json.h>
#include "Transform.h"

// static void DeserilizeJsonFile(const std::string FilePath, std::vector<std::shared_ptr<Phe::PStaticMesh>>& MeshList, std::vector<Transform>& MeshTransform)
// {
// 	Json::Reader reader;
// 	Json::Value root;
// 	std::ifstream in(FilePath, std::ifstream::binary);
// 	if (reader.parse(in, root))
// 	{
// 		for (auto it : root)
// 		{
// 			int index = 0;
// 			std::string MeshName;
// 			std::vector<float> Vertices;
// 			std::vector<UINT32> Indices;
// 			std::vector<float> TangentZs;
// 			Transform Trans;
// 
// 			MeshName = it["StaticMeshName"].asString();
// 
// 			for (auto iVertex : it["Vertices"])
// 			{
// 				Vertices.push_back(iVertex.asFloat());
// 			}
// 
// 			for (auto iIndex : it["Indices"])
// 			{
// 				Indices.push_back(iIndex.asInt());
// 			}
// 
// 			for (auto iTangent : it["TangentZ"])
// 			{
// 				TangentZs.push_back(iTangent.asFloat());
// 			}
// 			std::vector<float> Loc;
// 			for (auto iLocation : it["Location"])
// 			{
// 				Loc.push_back(iLocation.asFloat());
// 			}
// 			std::vector<float> Rot;
// 			for (auto iRotation : it["Rotation"])
// 			{
// 				Rot.push_back(iRotation.asFloat());
// 			}
// 			std::vector<float> Scl;
// 			for (auto iScale : it["Scale"])
// 			{
// 				Scl.push_back(iScale.asFloat());
// 			}
// 			Trans = Transform(glm::vec3{ Loc[0], Loc[1], Loc[2] }, glm::vec3{ Rot[0], Rot[1], Rot[2] }, glm::vec3{ Scl[0], Scl[1], Scl[2] });
// 			std::shared_ptr<Phe::PStaticMesh> Mesh = std::make_shared<Phe::PStaticMesh>(MeshName, Vertices, Indices, TangentZs);
// 			MeshList.push_back(Mesh);
// 			MeshTransform.push_back(Trans);
// 			index++;
// 		}
// 	}
// }

static void DeserilizeActorJsonFile(const std::string FilePath, std::vector<std::pair<std::string, Transform>>& MeshTransform)
{
	Json::Reader reader;
	Json::Value root;
	std::ifstream in(FilePath, std::ifstream::binary);
	if (reader.parse(in, root))
	{
		for (auto it : root)
		{
			std::string MeshName;
			Transform Trans;

			MeshName = it["StaticMeshName"].asString();

			std::vector<float> Loc;
			for (auto iLocation : it["Location"])
			{
				Loc.push_back(iLocation.asFloat());
			}
			std::vector<float> Rot;
			for (auto iRotation : it["Rotation"])
			{
				Rot.push_back(iRotation.asFloat());
			}
			std::vector<float> Scl;
			for (auto iScale : it["Scale"])
			{
				Scl.push_back(iScale.asFloat());
			}
			Trans = Transform(glm::vec3{ Loc[0], Loc[1], Loc[2] }, glm::vec3{ Rot[0], Rot[1], Rot[2] }, glm::vec3{ Scl[0], Scl[1], Scl[2] });
			MeshTransform.push_back({ MeshName, Trans });
		}
	}
	in.close();
}

static glm::vec4 VectorToMat(glm::vec3 V, glm::mat4 M)
{
	glm::vec4 Ret = V.x * M[0] + V.y * M[1] + V.z * M[2] + M[3];
	glm::vec4 w(Ret.w);
	return Ret/w;
}

static std::string GetIncrementID(std::string ID)
{
	UINT32 NumID;
	std::stringstream ss;
	std::stringstream ss1;
	ss << ID;
	ss >> NumID;
	NumID++;
	ss1 << NumID;
	std::string test = ss1.str();
	return ss1.str();
}
