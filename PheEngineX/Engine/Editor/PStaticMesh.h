#pragma once
#include "pch.h"
namespace Phe
{
	class PNodeStaticMesh;
	class PStaticMesh
	{
	public:
		PStaticMesh(std::string Name, std::vector<float> Vertices, std::vector<UINT16> Indices,std::vector<float> Normals, std::vector<float> Tangents, std::vector<float> UVs);
		~PStaticMesh();

		void BindNodeStaticMesh(PNodeStaticMesh* NodeStaticMesh);
		void UnBindNodeStaticMesh(PNodeStaticMesh* NodeStaticMesh);

		std::string GetName() { return PMeshName; }
		std::vector<float> GetVertices() { return PVertices; }
		std::vector<UINT16> GetIndices() { return PIndices; }
		std::vector<float> GetTangents() { return PTangents; }
		std::vector<float> GetNormals() { return PNormals; }
		std::vector<float> GetUVs() { return PUVs; }
	protected:
		std::string PMeshName;
		std::vector<float> PVertices;
		std::vector<UINT16> PIndices;
		std::vector<float> PNormals;
		std::vector<float> PTangents;
		std::vector<float> PUVs;

		std::unordered_map<std::string, PNodeStaticMesh*> LinkedNodeStaticMesh;
	};
}