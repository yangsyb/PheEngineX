#pragma once
#include "pch.h"

namespace Phe
{
	class PRay
	{
		friend class PBoundingBox;
	public:
		PRay();
		~PRay();
	private:
		glm::vec3 PRayOrigin;
		glm::vec3 PRayDirection;
	};
}