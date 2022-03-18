#pragma once
#include "pch.h"
#include "Engine/Transform.h"
namespace Phe
{
	class PLight
	{
	public:
		PLight();
		~PLight();

	private:
		Transform PLightTransform;

	};
}