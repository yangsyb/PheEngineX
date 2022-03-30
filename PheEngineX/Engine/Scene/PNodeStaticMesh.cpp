#include "pch.h"
#include "PNodeStaticMesh.h"
#include "Render/PPrimitive.h"
namespace Phe
{
	PNodeStaticMesh::PNodeStaticMesh(PNode* Parent) : PNode(nullptr), PLinkedPrimitive(nullptr)
	{

	}

	void PNodeStaticMesh::SetLinkedPrimitive(PPrimitive* LinkedPrimitive)
	{
		PLinkedPrimitive = LinkedPrimitive;
	}

	void PNodeStaticMesh::SetStaticMeshName(std::string StaticMeshName)
	{
		PStaticMeshName = StaticMeshName;
	}

	PNodeStaticMesh::~PNodeStaticMesh()
	{

	}

}