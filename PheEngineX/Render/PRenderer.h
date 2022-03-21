#pragma once
#include "pch.h"
#include "GPUResource/PGPUCommonBuffer.h"
#include "PRenderScene.h"
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

		void UpdatePrimitiveBuffer(PPrimitive* Primitive);
		void UpdateCamera(PerCameraCBuffer CameraCBuffer);
		void ShaderResourceBinding(PPrimitive* Primitive);
	private:
		void RenderCurrentScene(PRenderScene* RenderScene);
		PGPUCommonBuffer* PerCameraBuffer;
		PerCameraCBuffer CurrentCameraData;
	};
}