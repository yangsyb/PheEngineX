#pragma once
#include "pch.h"
#include "Transform.h"
namespace Phe
{
	class PStaticMesh
	{
	public:
		PStaticMesh(std::string Name, std::vector<float> Vertices, std::vector<UINT16> Indices, std::vector<float> Tangents, std::vector<float> UVs);
		~PStaticMesh();

		std::string GetName() { return PMeshName; }
		std::vector<float> GetVertices() { return PVertices; }
		std::vector<UINT16> GetIndices() { return PIndices; }
		std::vector<float> GetTangents() { return PTangents; }
		std::vector<float> GetUVs() { return PUVs; }
	protected:
		std::string PMeshName;
		std::vector<float> PVertices;
		std::vector<UINT16> PIndices;
		std::vector<float> PTangents;
		std::vector<float> PUVs;
	};
}