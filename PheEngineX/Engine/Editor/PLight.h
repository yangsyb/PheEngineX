#pragma once
#include "pch.h"
#include "Engine/Core/Transform.h"
#include "Engine/Scene/PNodeLight.h"
namespace Phe
{
	class PLight
	{
	public:
		PLight(std::string LightName);
		~PLight();

		void BindNodeLight(PNodeLight* NodeLight);

		void UpdateLightName(std::string LightName);
		void UpdateLightRadius(float LightRadius);
		void UpdateLightStrength(float LightStrength);
		void UpdateIsDynamic(bool IsDynamic);

		std::string GetLightName() { return PLightName; }
		float GetLightRadius() { return PLightRadius; }
		float GetLightStrength() { return PLightStrength; }
		bool GetIsDynamic() { return PIsDynamic; }
	private:
		std::string PLightName;
		float PLightRadius;
		float PLightStrength;
		bool PIsDynamic;
		
		std::unordered_multimap<std::string, PNodeLight*> LinkedNodeLight;
	};
}