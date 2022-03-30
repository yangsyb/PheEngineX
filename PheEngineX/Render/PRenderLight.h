#pragma once
#include "pch.h"
#include "GPUResource/PGPUCommonBuffer.h"

namespace Phe
{
	class PRenderLight
	{
	public:
		PRenderLight();
		~PRenderLight();

		PGPUCommonBuffer* GetTransformBuffer() { return TransformBuffer; }
		PGPUCommonBuffer* GetCameraBuffer() { return CameraBuffer; }

		void UpdateCameraBuffer(void* Data);
		void UpdateCommonBuffer(PGPUCommonBuffer* Buffer, void* Data);
		void DestroyRenderLight();

	private:
		PGPUCommonBuffer* TransformBuffer;
		PGPUCommonBuffer* CameraBuffer;
	};
}