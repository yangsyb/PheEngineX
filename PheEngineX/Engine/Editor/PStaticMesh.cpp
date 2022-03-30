#include "pch.h"
#include "PStaticMesh.h"
namespace Phe
{
	// Raw StaticMesh Data2
	// Managed By AssetManager
	PStaticMesh::PStaticMesh(std::string Name, std::vector<float> Vertices, std::vector<UINT16> Indices, std::vector<float> Tangents, std::vector<float> UVs) : PMeshName(Name), PVertices(Vertices), PIndices(Indices), PTangents(Tangents), PUVs(UVs)
	{

	}

	PStaticMesh::~PStaticMesh()
	{

	}

	void PStaticMesh::BindNodeStaticMesh(PNodeStaticMesh* NodeStaticMesh)
	{
		LinkedNodeStaticMesh.insert({ NodeStaticMesh->GetID(), NodeStaticMesh });
	}

}