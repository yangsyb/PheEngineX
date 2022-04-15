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

	struct BloomData
	{
		PPipeline* HighLightPipeline;
		PPipeline* DownSamplePipeline;
		PPipeline* UpSamplePipeline;
		PPipeline* MergeSamplePipeline;
		PPipeline* ToneMapPipeline;

		PGPURenderTarget* PHighLightRenderTarget;
		PGPURenderTarget* PBloomDownRenderTarget;
		PGPURenderTarget* PBloomDownRenderTarget1;
		PGPURenderTarget* PBloomDownRenderTarget2;
		PGPURenderTarget* PBloomDownRenderTarget3;
		PGPURenderTarget* PBloomUpRenderTarget0;
		PGPURenderTarget* PBloomUpRenderTarget1;
		PGPURenderTarget* PBloomUpRenderTarget2;
		PGPURenderTarget* PBloomMergeRenderTarget;

		PGPUCommonBuffer* BloomDownBuffer;
		PGPUCommonBuffer* BloomDownBuffer1;
		PGPUCommonBuffer* BloomDownBuffer2;
		PGPUCommonBuffer* BloomDownBuffer3;
		PGPUCommonBuffer* BloomDownBuffer4;

		PGPUCommonBuffer* BloomUpBuffer;
		PGPUCommonBuffer* BloomUpBuffer1;
		PGPUCommonBuffer* BloomUpBuffer2;
		PGPUCommonBuffer* BloomUpBuffer3;

		PGPUCommonBuffer* ToneMapBuffer;
		
		BloomData() : HighLightPipeline(nullptr), DownSamplePipeline(nullptr), UpSamplePipeline(nullptr), MergeSamplePipeline(nullptr), ToneMapPipeline(nullptr),
			PHighLightRenderTarget(nullptr), PBloomDownRenderTarget(nullptr), PBloomDownRenderTarget1(nullptr), PBloomDownRenderTarget2(nullptr), PBloomDownRenderTarget3(nullptr),
			PBloomUpRenderTarget0(nullptr), PBloomUpRenderTarget1(nullptr), PBloomUpRenderTarget2(nullptr),
			PBloomMergeRenderTarget(nullptr),
			BloomDownBuffer(nullptr), BloomDownBuffer1(nullptr), BloomDownBuffer2(nullptr), BloomDownBuffer3(nullptr), BloomDownBuffer4(nullptr),
			BloomUpBuffer(nullptr), BloomUpBuffer1(nullptr), BloomUpBuffer2(nullptr), BloomUpBuffer3(nullptr),
			ToneMapBuffer(nullptr)
		{}
		~BloomData()
		{
			ReleasePtr(HighLightPipeline);
			ReleasePtr(DownSamplePipeline);
			ReleasePtr(UpSamplePipeline);
			ReleasePtr(MergeSamplePipeline);
			ReleasePtr(ToneMapPipeline);
			ReleasePtr(PHighLightRenderTarget);
			ReleasePtr(PBloomDownRenderTarget);
			ReleasePtr(PBloomDownRenderTarget1);
			ReleasePtr(PBloomDownRenderTarget2);
			ReleasePtr(PBloomDownRenderTarget3);
			ReleasePtr(PBloomUpRenderTarget0);
			ReleasePtr(PBloomUpRenderTarget1);
			ReleasePtr(PBloomUpRenderTarget2);
			ReleasePtr(PBloomMergeRenderTarget);
			ReleasePtr(BloomDownBuffer);
			ReleasePtr(BloomDownBuffer1);
			ReleasePtr(BloomDownBuffer2);
			ReleasePtr(BloomDownBuffer3);
			ReleasePtr(BloomDownBuffer4);
			ReleasePtr(BloomUpBuffer);
			ReleasePtr(BloomUpBuffer1);
			ReleasePtr(BloomUpBuffer2);
			ReleasePtr(BloomUpBuffer3);
			ReleasePtr(ToneMapBuffer);
		}

		void Initialize();
	};

	struct DOFData
	{
		PPipeline* DOFCOCPipeline;

		PGPURenderTarget* PDOFCOCRenderTarget;

		PGPUCommonBuffer* DOFCOCBuffer;

		DOFData() : PDOFCOCRenderTarget(nullptr),
			DOFCOCPipeline(nullptr),
			DOFCOCBuffer(nullptr)
		{}

		~DOFData()
		{
			ReleasePtr(DOFCOCPipeline);

			ReleasePtr(PDOFCOCRenderTarget);

			ReleasePtr(DOFCOCBuffer);
		}

		void Initialize();
	};

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
		void HDRPass(PRenderScene* RenderScene);
		void DOFPass();
		void BloomPass();
		void ToneMappingPass();

		void SetExportDepth() { NeedExportDepth = true; }
	private:
		void RenderCurrentScene(PRenderScene* RenderScene);
		PGPUCommonBuffer* PerCameraBuffer;
		PGPUCommonBuffer* PerOrthoCameraBuffer;

		PPipeline* PCurrentPipeline;

		PGPURenderTarget* PShadowMap;
		PGPURenderTarget* PHDRRenderTarget;
		PGPURenderTarget* PIBLBRDFRenderTarget;
		PGPURenderTarget* PExportRenderTarget;

		PGPUMeshBuffer* ScreenMeshBuffer;

		UINT32 ScreenWidth;
		UINT32 ScreenHeight;

		BloomData* Bloom;
		DOFData* DOF;


	private:
		bool NeedExportDepth = false;
	};
}