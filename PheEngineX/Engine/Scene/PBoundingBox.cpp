#include "pch.h"
#include "PBoundingBox.h"

namespace Phe
{

	PBoundingBox::PBoundingBox() : PBoundingCenter(glm::vec3(0.f, 0.f, 0.f)), PBoundingExtent(glm::vec3(0.f, 0.f, 0.f)), PBoundingSize(glm::vec3(0.f, 0.f, 0.f))
	{

	}

	PBoundingBox::~PBoundingBox()
	{

	}



}