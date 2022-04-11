#include "pch.h"
#include "PRenderLight.h"
#include "RHI/PRHI.h"

namespace Phe
{
	
	PRenderLight::PRenderLight()
	{
/*		TransformBuffer = PRHI::Get()->CreateCommonBuffer(sizeof(PerObjectCBuffer), 1);*/
		CameraBuffer = PRHI::Get()->CreateCommonBuffer(sizeof(PerCameraCBuffer), 1);
		PerLightBuffer = PRHI::Get()->CreateCommonBuffer(sizeof(PerLightCBuffer), 1);
	}

	PRenderLight::~PRenderLight()
	{

	}

	void PRenderLight::UpdateCameraBuffer(PerCameraCBuffer Data)
	{
		std::shared_ptr<void> CameraData = std::make_shared<PerCameraCBuffer>(Data);
		UpdateCommonBuffer(CameraBuffer, CameraData);
	}


	void PRenderLight::UpdatePerLightBuffer(PerLightCBuffer Data)
	{
		std::shared_ptr<void> LightData = std::make_shared<PerLightCBuffer>(Data);
		UpdateCommonBuffer(PerLightBuffer, LightData);
	}

	void PRenderLight::UpdateCommonBuffer(PGPUCommonBuffer* Buffer, std::shared_ptr<void> Data)
	{
		PRHI::Get()->UpdateCommonBuffer(Buffer, Data);
	}

	void PRenderLight::DestroyRenderLight()
	{
// 		ReleasePtr(CameraBuffer);
// 		ReleasePtr(PerLightBuffer);
		PRHI::Get()->DestroyGPUCommonBuffer(CameraBuffer);
		PRHI::Get()->DestroyGPUCommonBuffer(PerLightBuffer);
	}

}