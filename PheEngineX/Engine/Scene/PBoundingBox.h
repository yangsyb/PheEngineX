#pragma once
#include "pch.h"
#include "PRay.h"

namespace Phe
{
	class PBoundingBox
	{
	public:
		PBoundingBox();
		~PBoundingBox();

		static PBoundingBox Union(PBoundingBox BoundingBoxA, PBoundingBox BoundingBoxB);

		bool Intersect(PRay* Ray)
		{
			for (int index = 0; index < 3; index++)
			{
				float InverseRayDirection = 1 / Ray->PRayDirection[index];
				float Near = (PBoundingCenter[index] - PBoundingExtent[index] - Ray->PRayOrigin[index]) * InverseRayDirection;
				float Far = (PBoundingCenter[index] + PBoundingExtent[index] - Ray->PRayOrigin[index]) * InverseRayDirection;

				if (Near > Far) std::swap(Near, Far);

				
			}
		}

	private:
		glm::vec3 PBoundingCenter;
		glm::vec3 PBoundingExtent;
		glm::vec3 PBoundingSize;

	};
}