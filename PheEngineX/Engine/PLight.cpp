#include "pch.h"
#include "PLight.h"

namespace Phe
{
	
	PLight::PLight(std::string InName, Transform InTransform /*= (Transform(glm::vec3(0,0,0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)))*/, float InRadius /*= 0.f*/, float InLightStrength /*= 0.f*/) :
		PLightTransform(InTransform), PLightRaidus(InRadius), PLightStrength(InLightStrength), PLightName(InName), PIsDynamic(false)
	{
		LightView = new POrthographicCamera(4096.f, 4096.f);
		LightView->SetTransform(PLightTransform);
//		LightView->SetDirection(glm::vec3(0, 0, 0) - PLightTransform.GetPosition());
	}

	PLight::~PLight()
	{
		ReleasePtr(LightView);
	}

	void PLight::SetLightPosition(glm::vec3 Position)
	{
		PLightTransform.Translation = Position;
		LightView->SetTransform(PLightTransform);
//		LightView->SetDirection(glm::vec3(0,0,0) - Position);
	}

}