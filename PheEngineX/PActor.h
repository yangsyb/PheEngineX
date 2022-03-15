#pragma once
#include "pch.h"
#include "Transform.h"
#include "RHI/PMaterial.h"

namespace Phe
{
	class PActor
	{
	public:
		PActor();
		~PActor();
		void SetStaticMesh(std::string ActorName);
		void SetTransform(Transform ActorTransform);
		void SetMaterial(std::string MaterialName);

		std::string GetName() { return PActorName; }
		Transform GetTransform() { return PActorTransform; }
		std::string GetMaterialName() { return PMaterialName; }
	protected:
		std::string PActorName;
		Transform PActorTransform;
		std::string PMaterialName;
	};
}