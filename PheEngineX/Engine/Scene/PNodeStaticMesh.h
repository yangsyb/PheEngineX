#pragma once
#include "pch.h"
#include "PNode.h"


namespace Phe
{
	class PPrimitive;
	class PNodeStaticMesh : public PNode
	{
		DECLARE_NODE_WITH_CONSTRUCTOR(StaticMesh);

	public:
		void SetLinkedPrimitive(PPrimitive* LinkedPrimitive);
		void SetStaticMeshName(std::string StaticMeshName);
		std::string GetStaticMeshName() { return PStaticMeshName;}
		PPrimitive* GetLinkedPrimitive() { return PLinkedPrimitive; }
		virtual ~PNodeStaticMesh();
	private:
		std::string PStaticMeshName;
		PPrimitive* PLinkedPrimitive;
	};
}