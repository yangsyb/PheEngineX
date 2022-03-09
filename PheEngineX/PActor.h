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
		void SetMaterial(std::shared_ptr<PMaterial> ActorMaterial);

	protected:
		std::string PActorName;
		Transform PActorTransform;
		std::shared_ptr<PMaterial> PActorMaterial;
	};
}