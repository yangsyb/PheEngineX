#include "pch.h"
#include "PRenderer.h"
#include "RHI/PRHI.h"
#include "PRenderThread.h"
#include "Engine/Editor/PShaderManager.h"

namespace Phe
{
	PRenderer::PRenderer() : PerCameraBuffer(nullptr), PShadowMap(nullptr), PCurrentPipeline(nullptr), PIBLBRDFRenderTarget(nullptr), PExportRenderTarget(nullptr), PerOrthoCameraBuffer(nullptr)
	{
		PRHI::Get()->InitRHI();
	}

	PRenderer::~PRenderer()
	{

	}

	void PRenderer::Initialize()
	{
		PRHI::Get()->InitGraphicsPipeline();
		PRHI::Get()->ResizeWindow(1920, 1080);
		PerCameraBuffer = PRHI::Get()->CreateCommonBuffer(sizeof(PerCameraCBuffer), 1);
		PerOrthoCameraBuffer = PRHI::Get()->CreateCommonBuffer(sizeof(PerCameraCBuffer), 1);
	}

	
	void PRenderer::RenderFrameBegin(PRenderScene* RenderScene)
	{
		PRHI::Get()->BeginFrame();
		PCurrentPipeline = nullptr;
		PRenderLight* MainRenderLight = nullptr;
		ExportPass(RenderScene);
		if(NeedExportDepth)
 		{
			PRHI::Get()->ReadBackRTBuffer(PExportRenderTarget->GetDepthStencilBuffer());
			NeedExportDepth = false;
		}
		if(MainRenderLight = RenderScene->GetMainRenderLight())
		{
			ShadowPass(RenderScene);
			return;
		}
		ReleasePtr(PShadowMap);
	}

	void PRenderer::RenderFrameEnd(PRenderScene* RenderScene)
	{
		PRHI::Get()->EndFrame();
	}

	void PRenderer::Render(PRenderScene* RenderScene)
	{
		PRHI::Get()->BeginRenderBackBuffer();
		RenderCurrentScene(RenderScene);
		PRHI::Get()->EndRenderBackBuffer();
	}

	void PRenderer::DestroyRenderer()
	{
		ReleasePtr(PerCameraBuffer);
		ReleasePtr(PerOrthoCameraBuffer);
		ReleasePtr(PShadowMap);
		ReleasePtr(PExportRenderTarget);
	}

	void PRenderer::UpdatePrimitiveBuffer(PPrimitive* Primitive)
	{

	}

	void PRenderer::UpdateCamera(PerCameraCBuffer CameraCBuffer)
	{
		std::shared_ptr<void> CameraData = std::make_shared<PerCameraCBuffer>(CameraCBuffer);
		PRHI::Get()->UpdateCommonBuffer(PerCameraBuffer, CameraData);
	}


	void PRenderer::UpdateOrthoCamera(PerCameraCBuffer CameraCBuffer)
	{
		std::shared_ptr<void> CameraData = std::make_shared<PerCameraCBuffer>(CameraCBuffer);
		PRHI::Get()->UpdateCommonBuffer(PerOrthoCameraBuffer, CameraData);
	}

	void PRenderer::UpdateLight(PRenderLight* RenderLight, PerLightCBuffer LightCBuffer)
	{
		RenderLight->UpdatePerLightBuffer(LightCBuffer);
	}

	void PRenderer::ShaderResourceBinding(PPrimitive* Primitive)
	{
// 		auto Shader = Primitive->GetPipeline()->GetShader();
// 		auto ShaderParameters = Shader->GetShaderParameters();
// 		for(auto Parameters : ShaderParameters)
// 		{
// 			switch (Parameters.type)
// 			{
// 			case ShaderParamType::ConstantBuffer:
// 				break;
// 			case ShaderParamType::CBVDescriptorHeap:
// 				PRHI::Get()->SetRenderResourceTable(Parameters.name, Shader->GetResourceBindingByPrimitiveName(Primitive->GetPrimitiveId()).at(Parameters.name));
// 				break;
// 			case ShaderParamType::SRVDescriptorHeap:
// 				PRHI::Get()->SetRenderResourceTable(Parameters.name, Shader->GetResourceBindingByPrimitiveName(Primitive->GetPrimitiveId()).at(Parameters.name));
// 				break;
// 			}
// 		}

		PRHI::Get()->SetRenderResourceTable("Texture", Primitive->GetMaterial()->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("PerObjectBuffer", Primitive->GetPerObjBuffer()->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("PerMaterialBuffer", Primitive->GetPerMatBuffer()->GetHandleOffset());
	}

	void PRenderer::ShadowPass(PRenderScene* RenderScene)
	{
		PGPUCommonBuffer* MainLightBuffer = RenderScene->GetMainRenderLight()->GetCameraBuffer();
		if(!PShadowMap)
		{
			PShadowMap = PRHI::Get()->CreateRenderTarget("ShadowMap", 2048, 2048);
			PShadowMap->AddDepthStencilBuffer();
			PShadowMap->GetDepthStencilBuffer()->PRTTexture = PRHI::Get()->CreateTexture("ShadowMapTexture", PShadowMap->GetDepthStencilBuffer());
		}
   		PRHI::Get()->BeginRenderRTBuffer(PShadowMap->GetDepthStencilBuffer());
  
 		PRHI::Get()->SetRenderTarget(PShadowMap);

  		auto CurrentDrawPrimitives = RenderScene->GetPrimitives();
   		for (auto Primitive : CurrentDrawPrimitives)
   		{
// 			UpdatePrimitiveBuffer(Primitive.second);
			auto PrimitivePipeline = Primitive.second->GetPipeline(PipelineType::ShadowPipeline);
  			if(PrimitivePipeline != PCurrentPipeline)
  			{
  				PRHI::Get()->SetGraphicsPipeline(PrimitivePipeline);
  				PCurrentPipeline = PrimitivePipeline;
  			}
 			PRHI::Get()->SetMeshBuffer(Primitive.second->GetMeshBuffer());
			PRHI::Get()->SetRenderResourceTable("PerCameraBuffer", MainLightBuffer->GetHandleOffset());
 			ShaderResourceBinding(Primitive.second);
 			PRHI::Get()->DrawPrimitiveIndexedInstanced(Primitive.second->GetMeshBuffer()->GetIndexCount());
   		}
   		PRHI::Get()->EndRenderRTBuffer(PShadowMap->GetDepthStencilBuffer());
// 		if(NeedExportDepth)
// 		{
// 			PRHI::Get()->ReadBackRTBuffer(PShadowMap->GetDepthStencilBuffer());
// 			//PRHI::Get()->ReadBackTexture(RenderScene->GetFirstTexture());
// 			NeedExportDepth = false;
// 		}
	}


	void PRenderer::IBLBRDFPass(PRenderScene* RenderScene)
	{
		PIBLBRDFRenderTarget = PRHI::Get()->CreateRenderTarget("IBLBRDF", 1024, 1024);
		PIBLBRDFRenderTarget->AddColorBuffer(1);
		PIBLBRDFRenderTarget->GetColorBuffer(1)->PRTTexture = PRHI::Get()->CreateTexture("IBLBRDFTexture", PIBLBRDFRenderTarget->GetColorBuffer(1));
		PRHI::Get()->BeginRenderRTBuffer(PIBLBRDFRenderTarget->GetColorBuffer(1));
		PRHI::Get()->SetRenderTarget(PIBLBRDFRenderTarget);

		PRHI::Get()->EndRenderRTBuffer(PIBLBRDFRenderTarget->GetColorBuffer(1));
	}


	void PRenderer::ExportPass(PRenderScene* RenderScene)
	{
		if (!PExportRenderTarget)
		{
			PExportRenderTarget = PRHI::Get()->CreateRenderTarget("Export", 1024, 1024);
			PExportRenderTarget->AddDepthStencilBuffer();
			PExportRenderTarget->GetDepthStencilBuffer()->PRTTexture = PRHI::Get()->CreateTexture("ExportDepthTexture", PExportRenderTarget->GetDepthStencilBuffer());
		}
		PRHI::Get()->BeginRenderRTBuffer(PExportRenderTarget->GetDepthStencilBuffer());

		PRHI::Get()->SetRenderTarget(PExportRenderTarget);

		auto CurrentDrawPrimitives = RenderScene->GetPrimitives();
		for (auto Primitive : CurrentDrawPrimitives)
		{
			auto PrimitivePipeline = Primitive.second->GetPipeline(PipelineType::ShadowPipeline);
			if (PrimitivePipeline != PCurrentPipeline)
			{
				PRHI::Get()->SetGraphicsPipeline(PrimitivePipeline);
				PCurrentPipeline = PrimitivePipeline;
			}
			PRHI::Get()->SetMeshBuffer(Primitive.second->GetMeshBuffer());
			PRHI::Get()->SetRenderResourceTable("PerCameraBuffer", PerOrthoCameraBuffer->GetHandleOffset());
			ShaderResourceBinding(Primitive.second);
			PRHI::Get()->DrawPrimitiveIndexedInstanced(Primitive.second->GetMeshBuffer()->GetIndexCount());
		}
		PRHI::Get()->EndRenderRTBuffer(PExportRenderTarget->GetDepthStencilBuffer());
	}

	void PRenderer::RenderCurrentScene(PRenderScene* RenderScene)
	{
		auto CurrentDrawPrimitives = RenderScene->GetPrimitives();
		for(auto Primitive : CurrentDrawPrimitives)
		{
//			UpdatePrimitiveBuffer(Primitive.second);
			auto PrimitivePipeline = Primitive.second->GetPipeline(PipelineType::BasePipeline);
  			if (PrimitivePipeline != PCurrentPipeline)
  			{
  				PRHI::Get()->SetGraphicsPipeline(PrimitivePipeline);
  				PCurrentPipeline = PrimitivePipeline;
  			}
			PRHI::Get()->SetMeshBuffer(Primitive.second->GetMeshBuffer());
			PRHI::Get()->SetRenderResourceTable("PerCameraBuffer", PerCameraBuffer->GetHandleOffset());
			ShaderResourceBinding(Primitive.second);
   			if(PShadowMap)
  			{
   				PRHI::Get()->SetRenderResourceTable("ShadowTexture", PShadowMap->GetDepthStencilBuffer()->PRTTexture->GetHandleOffset());
				PRHI::Get()->SetRenderResourceTable("PerLightBuffer", RenderScene->GetMainRenderLight()->GetPerLightBuffer()->GetHandleOffset());
   			}
			PRHI::Get()->DrawPrimitiveIndexedInstanced(Primitive.second->GetMeshBuffer()->GetIndexCount());
		}
	}
}