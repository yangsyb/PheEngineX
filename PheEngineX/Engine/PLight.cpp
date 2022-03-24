#include "pch.h"
#include "PLight.h"

namespace Phe
{
	
	PLight::PLight(std::string InName, Transform InTransform /*= (Transform(glm::vec3(0,0,0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)))*/, float InRadius /*= 0.f*/, float InLightStrength /*= 0.f*/) :
		PLightTransform(InTransform), PLightRaidus(InRadius), PLightStrength(InLightStrength), PLightName(InName)
	{
		LightView = new POrthographicCamera(1024.f, 1024.f);
		LightView->SetTransform(PLightTransform);
	}

	PLight::~PLight()
	{
		ReleasePtr(LightView);
	}

}