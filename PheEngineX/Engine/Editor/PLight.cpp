#include "pch.h"
#include "PLight.h"

namespace Phe
{

	PLight::PLight(std::string LightName) : PLightName(LightName), PLightRadius(8), PLightStrength(10), PIsDynamic(false)
	{

	}


	PLight::~PLight()
	{
		for (auto LinkedNode : LinkedNodeLight)
		{
			ReleasePtr(LinkedNode.second);
		}
	}

	void PLight::BindNodeLight(PNodeLight* NodeLight)
	{
		LinkedNodeLight.insert({ NodeLight->GetID(), NodeLight });
	}


	void PLight::UnBindNodeLight(PNodeLight* NodeLight)
	{
		if (LinkedNodeLight.count(NodeLight->GetID()) > 0)
		{
			LinkedNodeLight.erase(NodeLight->GetID());
		}
	}

	void PLight::UpdateLightName(std::string LightName)
	{
		PLightName = LightName;
		for (auto NodeLight : LinkedNodeLight)
		{
			NodeLight.second->SetLightName(PLightName);
		}
	}

	void PLight::UpdateLightRadius(float LightRadius)
	{
		PLightRadius = LightRadius;
		for (auto NodeLight : LinkedNodeLight)
		{
			NodeLight.second->SetLightRadius(PLightRadius);
		}
	}

	void PLight::UpdateLightStrength(float LightStrength)
	{
		PLightStrength = LightStrength;
		for (auto NodeLight : LinkedNodeLight)
		{
			NodeLight.second->SetLightStrength(PLightStrength);
		}
	}

	void PLight::UpdateIsDynamic(bool IsDynamic)
	{
		PIsDynamic = IsDynamic;
		for (auto NodeLight : LinkedNodeLight)
		{
			NodeLight.second->SetIsDynamic(IsDynamic);
		}
	}
}