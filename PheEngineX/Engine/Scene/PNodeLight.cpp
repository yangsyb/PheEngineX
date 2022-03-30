#include "pch.h"
#include "PNodeLight.h"
#include "Render/PRenderLight.h"
#include "Engine/Editor/PLight.h"
namespace Phe
{
	PNodeLight::PNodeLight(PNode* Parent) : PNode(nullptr), PLinkedRenderLight(nullptr)
	{
		LightView = new POrthographicCamera(4096.f, 4096.f);
//		LightView->SetTransform(PLightTransform);
	}

	void PNodeLight::SetAsLight(PLight* Light)
	{
		PLightName = Light->GetLightName();
		PLightRaidus = Light->GetLightRadius();
		PLightStrength = Light->GetLightStrength();
		PIsDynamic = Light->GetIsDynamic();
	}

	void PNodeLight::SetLightName(std::string LightName)
	{
		PLightName = LightName;
	}

	void PNodeLight::SetLightRadius(float LightRadius)
	{
		PLightRaidus = LightRadius;
	}

	void PNodeLight::SetLightStrength(float LightStrength)
	{
		PLightStrength = LightStrength;
	}

	void PNodeLight::SetIsDynamic(bool IsDynamic)
	{
		PIsDynamic = IsDynamic;
	}

	void PNodeLight::SetLinkedLight(PRenderLight* LinkedLight)
	{
		PLinkedRenderLight = LinkedLight;
	}

	void PNodeLight::SetPosition(glm::vec3 Position)
	{
		LightView->SetPosition(Position);
	}

	void PNodeLight::SetRotation(glm::vec3 Rotation)
	{
		LightView->SetRotation(Rotation);
	}

	PNodeLight::~PNodeLight()
	{
		ReleasePtr(PLinkedRenderLight);
	}
}