#pragma once
#include "pch.h"
#include "PNode.h"


namespace Phe
{
	class PStaticMesh;
	class PPrimitive;
	class PNodeStaticMesh : public PNode
	{
		DECLARE_NODE_WITH_CONSTRUCTOR(StaticMesh);

	public:
		void SetLinkedPrimitive(PPrimitive* LinkedPrimitive);
		void SetStaticMeshName(std::string StaticMeshName);
		void BindLinkedStaticMesh(PStaticMesh* StaticMesh);
		void BindLinkedPrimitive(PPrimitive* LinkedPrimitive);
		std::string GetStaticMeshName() { return PStaticMeshName;}
		PPrimitive* GetLinkedPrimitive() { return PLinkedPrimitive; }
		virtual ~PNodeStaticMesh();
	private:
		std::string PStaticMeshName;

		PStaticMesh* PLinkedStaticMesh;
		PPrimitive* PLinkedPrimitive;
	};
}