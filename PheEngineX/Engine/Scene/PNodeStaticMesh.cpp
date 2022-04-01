#include "pch.h"
#include "PNodeStaticMesh.h"
#include "Render/PPrimitive.h"
#include "Engine/Editor/PStaticMesh.h"
#include "Render/PRenderThread.h"
namespace Phe
{
	PNodeStaticMesh::PNodeStaticMesh(PNode* Parent) : PNode(nullptr), PLinkedPrimitive(nullptr), PLinkedStaticMesh(nullptr), PStaticMeshName()
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

	void PNodeStaticMesh::BindLinkedStaticMesh(PStaticMesh* StaticMesh)
	{
		PLinkedStaticMesh = StaticMesh;
	}

	void PNodeStaticMesh::BindLinkedPrimitive(PPrimitive* LinkedPrimitive)
	{
		PLinkedPrimitive = LinkedPrimitive;
	}

	PNodeStaticMesh::~PNodeStaticMesh()
	{
		PLinkedStaticMesh->UnBindNodeStaticMesh(this);
		PString NodeId = GetID();
		PTask* Task = CreateTask(PTask, PRenderThread::Get()->GetRenderScene()->DestroyScenePrimitive(NodeId));
		PRenderThread::Get()->AddTask(Task);
	}

}