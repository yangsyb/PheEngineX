#pragma once
#include "pch.h"
#include "GPUResource/PGPUCommonBuffer.h"
#include "GPUResource/PGPURenderTarget.h"
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
		void PrepareShadowMap(PRenderScene* RenderScene);
	private:
		void RenderCurrentScene(PRenderScene* RenderScene);
		PGPUCommonBuffer* PerCameraBuffer;
		PerCameraCBuffer CurrentCameraData;

		PGPURenderTarget* PShadowMap;
		PPipeline* ShadowPipeline;
	};
}