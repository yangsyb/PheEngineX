#pragma once
#include "pch.h"
#include "Engine/Core/Transform.h"
namespace Phe
{
	class PNode;

	class PNodeFactory
	{
	public:
		template<class T>
		static T* CreateNode(PNode* Parent, std::string NodeID = std::string())
		{
			T* Node = new T(Parent);
			Node->SetID(NodeID);
			return Node;
		}
	};


#define DECLARE_NODE_WITH_CONSTRUCTOR(NodeName) \
		friend class PNodeFactory; \
	public: \
		PNode##NodeName(PNode * Parent);

	class PNode
	{
	public:
		PNode(PNode* Parent);
		virtual ~PNode();
		
		
		virtual void AddChild(PNode* Child);
		virtual void Remove();
		virtual void RemoveChild(PNode* Child);

		virtual void Tick();
		
		
		virtual void SetID(const std::string InNodeID) { PNodeID = InNodeID; }
		virtual void SetPostion(const glm::vec3 InPostion);
		virtual void SetRotation(const glm::vec3 InRotation);
		virtual void SetScale(const glm::vec3 InScale);
		virtual void SetTransform(const Transform InTransform);

		virtual PNode* GetNodeByID(const std::string ID);
		virtual std::string GetID() const { return PNodeID; }
		virtual PNode* GetNodeParent() { return PParent; }
		virtual PNode* GetNodeChild(UINT32 Index) { return Index<PChildren.size() ? PChildren[Index] : nullptr; }
		virtual UINT32 GetChildrenCount() { return UINT32(PChildren.size()); }
		virtual glm::vec3 GetRelativePosition() { return PRelativePostion; }
		virtual glm::vec3 GetRelativeRotaion() { return PRelativeRotation; }
		virtual glm::vec3 GetRelativeScale() { return PRelativeScale; }

		virtual Transform GetTransform() { return PTransform; }

		virtual PerObjectCBuffer* GetTransformBuffer() { return PTransformBufferData; }
	protected:
		std::string PNodeID;
		PNode* PParent;
		std::vector<PNode*> PChildren;

		glm::vec3 PRelativePostion;
		glm::vec3 PRelativeRotation;
		glm::vec3 PRelativeScale;

		Transform PTransform;

		PerObjectCBuffer* PTransformBufferData;
	};
}
