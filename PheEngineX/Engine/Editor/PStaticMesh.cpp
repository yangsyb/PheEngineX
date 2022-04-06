#include "pch.h"
#include "PStaticMesh.h"
#include "Engine/Scene/PNodeStaticMesh.h"
namespace Phe
{
	// Raw StaticMesh Data2
	// Managed By AssetManager
	PStaticMesh::PStaticMesh(std::string Name, std::vector<float> Vertices, std::vector<UINT16> Indices, std::vector<float> Normals, std::vector<float> Tangents, std::vector<float> UVs) : PMeshName(Name), PVertices(Vertices), PIndices(Indices), PTangents(Tangents), PUVs(UVs), PNormals(Normals)
	{

	}

	PStaticMesh::~PStaticMesh()
	{
		for(auto& LinkedNode : LinkedNodeStaticMesh)
		{
			ReleasePtr(LinkedNode.second);
		}
	}

	void PStaticMesh::BindNodeStaticMesh(PNodeStaticMesh* NodeStaticMesh)
	{
		LinkedNodeStaticMesh.insert({ NodeStaticMesh->GetID(), NodeStaticMesh });
	}

	void PStaticMesh::UnBindNodeStaticMesh(PNodeStaticMesh* NodeStaticMesh)
	{
		if(LinkedNodeStaticMesh.count(NodeStaticMesh->GetID()) > 0)
		{
			LinkedNodeStaticMesh.erase(NodeStaticMesh->GetID());
		}
	}
}