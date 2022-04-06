#include "pch.h"
#include "PRenderLight.h"
#include "RHI/PRHI.h"

namespace Phe
{
	
	PRenderLight::PRenderLight()
	{
		TransformBuffer = PRHI::Get()->CreateCommonBuffer(sizeof(PerObjectCBuffer), 1);
		CameraBuffer = PRHI::Get()->CreateCommonBuffer(sizeof(PerCameraCBuffer), 1);
	}

	PRenderLight::~PRenderLight()
	{

	}

	void PRenderLight::UpdateCameraBuffer(PerCameraCBuffer Data)
	{
		std::shared_ptr<void> CameraData = std::make_shared<PerCameraCBuffer>(Data);
		UpdateCommonBuffer(CameraBuffer, CameraData);
	}

	void PRenderLight::UpdateCommonBuffer(PGPUCommonBuffer* Buffer, std::shared_ptr<void> Data)
	{
		PRHI::Get()->UpdateCommonBuffer(Buffer, Data);
	}

	void PRenderLight::DestroyRenderLight()
	{
		ReleasePtr(TransformBuffer);
		ReleasePtr(CameraBuffer);
	}

}