#pragma once
#include "pch.h"
#include "Engine/Transform.h"
#include "Engine/PCamera.h"
namespace Phe
{
	class PLight
	{
	public:
		PLight(std::string InName, Transform InTransform = (Transform(glm::vec3(0,0,0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1))), float InRadius = 0.f, float InLightStrength = 0.f);
		~PLight();

		bool operator==(const PLight* L) { return this->GetLightName() == L->PLightName; }

		std::string GetLightName() { return PLightName; }
		Transform GetLightTransform() { return PLightTransform; }
		float GetLightRadius() { return PLightRaidus; }
		float GetLightStrength() { return PLightStrength; }
		PerCameraCBuffer GetPassCBuffer(){ return LightView->GetPassConstant(); }

	private:
		std::string PLightName;
		Transform PLightTransform;
		float PLightRaidus;
		float PLightStrength;
		
		POrthographicCamera* LightView;
	};
}