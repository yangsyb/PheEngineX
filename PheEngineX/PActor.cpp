#include "pch.h"
#include "PActor.h"

namespace Phe
{

	PActor::PActor() : PActorName("box"), PActorTransform(Transform{ glm::vec3(0,0,0), glm::vec4(0,0,0,1), glm::vec3(1,1,1) })
	{

	}

	PActor::~PActor()
	{

	}

	void PActor::SetStaticMesh(std::string ActorName)
	{
		PActorName = ActorName;
	}

	void PActor::SetTransform(Transform ActorTransform)
	{
		PActorTransform = ActorTransform;
	}

	void PActor::SetMaterial(std::string MaterialName)
	{
		PMaterialName = MaterialName;
	}


}