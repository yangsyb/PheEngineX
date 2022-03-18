#pragma once
#include "pch.h"
#include "GPUResource/PGPUCommonBuffer.h"

namespace Phe
{
	class PRHI;
	class PRenderScene;

	class PRenderer
	{
	public:
		PRenderer();
		~PRenderer();

		void Initialize();
		void RenderFrameBegin(PRenderScene* RenderScene);
		void RenderFrameEnd(PRenderScene* RenderScene);
		void Render(PRenderScene* RenderScene);	
		void DestroyRenderer();

		void UpdateCamera(PerCameraCBuffer CameraCBuffer);
	private:
		void RenderCurrentScene(PRenderScene* RenderScene);
		PGPUCommonBuffer* PerCameraBuffer;
		PerCameraCBuffer CurrentCameraData;
	};
}