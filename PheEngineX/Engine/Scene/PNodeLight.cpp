#include "pch.h"
#include "PNodeLight.h"
#include "Render/PRenderLight.h"
#include "Engine/Editor/PLight.h"
#include "Render/PRenderThread.h"
namespace Phe
{
	PNodeLight::PNodeLight(PNode* Parent) : PNode(nullptr), PLinkedRenderLight(nullptr), PIsDynamic(false), PLightRaidus(0.f), PLightStrength(0.f), PLinkedLight(nullptr), PLightName(), PLightColor(glm::vec3(1.f, 1.f, 1.f))
	{
		LightView = new POrthographicCamera(4096.f, 4096.f);
	}

	void PNodeLight::SetAsLight(PLight* Light)
	{
		PLightName = Light->GetLightName();
		PLightRaidus = Light->GetLightRadius();
		PLightStrength = Light->GetLightStrength();
		PLightColor = Light->GetLightColor();
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

	void PNodeLight::BindLinkedLight(PLight* Light)
	{
		PLinkedLight = Light;
	}

	void PNodeLight::BindRenderLight(PRenderLight* RenderLight)
	{
		PLinkedRenderLight = RenderLight;
	}


	PNodeLight::~PNodeLight()
	{
		PLinkedLight->UnBindNodeLight(this);
		ReleasePtr(LightView);
		PString NodeId = GetID();
		PTask* Task = CreateTask(PTask, PRenderThread::Get()->GetRenderScene()->DestroySceneLight(NodeId));
		PRenderThread::Get()->AddTask(Task);
	}
}