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

		void SetIsDynamic(bool IsDynamic) { PIsDynamic = IsDynamic; }
		void SetLightPosition(glm::vec3 Position);

		std::string GetLightName() { return PLightName; }
		Transform GetLightTransform() { return PLightTransform; }
		float GetLightRadius() { return PLightRaidus; }
		float GetLightStrength() { return PLightStrength; }
		PerCameraCBuffer GetPassCBuffer(){ return LightView->GetPassConstant(); }
		bool GetIsDynamic() { return PIsDynamic; }
	private:
		std::string PLightName;
		Transform PLightTransform;
		float PLightRaidus;
		float PLightStrength;

		bool PIsDynamic;
		
		POrthographicCamera* LightView;
	};
}