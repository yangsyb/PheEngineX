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

	void PRenderLight::UpdateCameraBuffer(void* Data)
	{
		UpdateCommonBuffer(CameraBuffer, Data);
	}

	void PRenderLight::UpdateCommonBuffer(PGPUCommonBuffer* Buffer, void* Data)
	{
		PRHI::Get()->UpdateCommonBuffer(Buffer, Data);
	}

	void PRenderLight::DestroyRenderLight()
	{
		ReleasePtr(TransformBuffer);
		ReleasePtr(CameraBuffer);
	}

}