#pragma once
#include "pch.h"
#include "GPUResource/PGPUCommonBuffer.h"
#include "GPUResource/PGPURenderTarget.h"
#include "PRenderScene.h"
namespace Phe
{
	class PRHI;
	class PRenderScene;
	class PGPUTexture;
	class PTexture;

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
		void UpdateOrthoCamera(PerCameraCBuffer CameraCBuffer);
		void UpdateLight(PRenderLight* RenderLight, PerLightCBuffer LightCBuffer);
		void ShaderResourceBinding(PPrimitive* Primitive);
		void ShadowPass(PRenderScene* RenderScene);
		void IBLBRDFPass(PRenderScene* RenderScene);
		void ExportPass(PRenderScene* RenderScene);
		void SkyBoxPass(PRenderScene* RenderScene);

		void SetExportDepth() { NeedExportDepth = true; }
	private:
		void RenderCurrentScene(PRenderScene* RenderScene);
		PGPUCommonBuffer* PerCameraBuffer;
		PGPUCommonBuffer* PerOrthoCameraBuffer;

		PPipeline* PCurrentPipeline;
		PGPURenderTarget* PShadowMap;
		PGPURenderTarget* PIBLBRDFRenderTarget;
		PGPURenderTarget* PExportRenderTarget;

	private:
		bool NeedExportDepth = false;
	};
}