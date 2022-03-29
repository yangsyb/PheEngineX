#include "pch.h"
#include "PNode.h"

namespace Phe
{

	PNode::PNode(PNode* Parent) : PParent(nullptr), PRelativePostion(glm::vec3(0, 0, 0)), PRelativeRotation(glm::vec3(0, 0, 0)), PRelativeScale(glm::vec3(1, 1, 1))
	{
		if(Parent)
		{
			PParent->AddChild(Parent);
		}
	}

	PNode::~PNode()
	{
		for(auto Node : PChildren)
		{
			ReleasePtr(Node);
		}
	}

	void PNode::AddChild(PNode* Child)
	{
		if(Child && (Child != this))
		{
			Child->Remove();
			PChildren.push_back(Child);
			Child->PParent = this;
		}
	}

	void PNode::Remove()
	{
		if(PParent)
		{
			PParent->RemoveChild(this);
		}
	}

	void PNode::RemoveChild(PNode* Child)
	{
		auto NodeIt = PChildren.begin();
		for(;NodeIt < PChildren.end(); NodeIt++)
		{
			if(*NodeIt == Child)
			{
				(*NodeIt)->PParent = nullptr;
				PChildren.erase(NodeIt);
				return;
			}
		}
	}

	void PNode::Tick()
	{
		for(auto ChildIt : PChildren)
		{
			ChildIt->Tick();
		}
	}

	void PNode::SetPostion(const glm::vec3 InPostion)
	{
		PRelativePostion = InPostion;
	}

	void PNode::SetRotation(const glm::vec3 InRotation)
	{
		PRelativeRotation = InRotation;
	}

	void PNode::SetScale(const glm::vec3 InScale)
	{
		PRelativeScale = InScale;
	}

	PNode* PNode::GetNodeByID(const std::string ID)
	{
		if(ID == PNodeID)
		{
			return this;
		}

		for(auto NodeIt : PChildren)
		{
			if(NodeIt->GetID() == ID)
			{
				return NodeIt;
			}
		}

		return nullptr;
	}

}