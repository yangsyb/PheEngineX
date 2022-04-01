#include "pch.h"
#include "PRenderer.h"
#include "RHI/PRHI.h"
#include "PRenderThread.h"
#include "Engine/Editor/PShaderManager.h"

namespace Phe
{
	PRenderer::PRenderer() : PerCameraBuffer(nullptr), PShadowMap(nullptr), ShadowPipeline(nullptr)
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

	}

	
	void PRenderer::RenderFrameBegin(PRenderScene* RenderScene)
	{
		PRenderLight* MainRenderLight = nullptr;
		if(MainRenderLight = RenderScene->GetMainRenderLight())
		{
			PrepareShadowMap(RenderScene);
			return;
		}
		ReleasePtr(PShadowMap);
	}

	void PRenderer::RenderFrameEnd(PRenderScene* RenderScene)
	{

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
		ReleasePtr(PShadowMap);
		ReleasePtr(ShadowPipeline);
	}

	void PRenderer::UpdatePrimitiveBuffer(PPrimitive* Primitive)
	{
		if(Primitive->GetPerObjBuffer())
		{
//			auto PerObj = Primitive->GetTransform().GetBufferData();
//			PRHI::Get()->UpdateCommonBuffer(Primitive->GetPerObjBuffer(), &PerObj);
		}
		if(Primitive->GetPerMatBuffer())
		{
			auto PerMat = Primitive->GetMaterial()->GetMaterialBuffer();
			PRHI::Get()->UpdateCommonBuffer(Primitive->GetPerMatBuffer(), &PerMat);
		}
	}

	void PRenderer::UpdateCamera(PerCameraCBuffer* CameraCBuffer)
	{
		PRHI::Get()->UpdateCommonBuffer(PerCameraBuffer, CameraCBuffer);
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
//		PRHI::Get()->SetRenderResourceTable("Texture", Primitive->GetMaterial()->GetGPUTextureBuffer().at(0)->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("Texture", Primitive->GetMaterial()->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("PerObjectBuffer", Primitive->GetPerObjBuffer()->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("PerMaterialBuffer", Primitive->GetPerMatBuffer()->GetHandleOffset());
	}

	void PRenderer::PrepareShadowMap(PRenderScene* RenderScene)
	{
		PGPUCommonBuffer* MainLightBuffer = RenderScene->GetMainRenderLight()->GetCameraBuffer();
		if(!PShadowMap)
		{
			PShadowMap = PRHI::Get()->CreateRenderTarget("ShadowMap", 2048, 2048);
			PShadowMap->AddDepthStencilBuffer();
			PShadowMap->GetDepthStencilBuffer()->PRTTexture = PRHI::Get()->CreateTexture("ShadowMapTexture", PShadowMap->GetDepthStencilBuffer());
		}
   		PRHI::Get()->BeginRenderRTBuffer(PShadowMap->GetDepthStencilBuffer());
// 		if (!ShadowPipeline)
// 		{
// 			PShader* ShadowShader = PRHI::Get()->CreateShader("PositionShader", L"Shaders\\Position.hlsl", "VS", "");
// 			ShadowPipeline = PRHI::Get()->CreatePipeline(ShadowShader);
// 			PRHI::Get()->UpdatePipeline(ShadowPipeline, PShadowMap);
//   		}
  
  		PRHI::Get()->PrepareBufferHeap();
 		PRHI::Get()->SetRenderTarget(PShadowMap);

//		RenderScene->GetMainLight()->GetLightView()->RecalculateOrtho(RenderScene->GetSceneCenter(), RenderScene->GetSceneRadius());
//		auto LightData = RenderScene->GetMainLight()->GetPassCBuffer();
//		LightData.Time = PRenderThread::Get()->GetCurrentTotalTime();
//		LightData.ShadowTransform = RenderScene->GetMainLight()->GetVP();
//		PRHI::Get()->UpdateCommonBuffer(MainLightBuffer, &LightData);

  		auto CurrentDrawPrimitives = RenderScene->GetPrimitives();
   		for (auto Primitive : CurrentDrawPrimitives)
   		{
 			UpdatePrimitiveBuffer(Primitive.second);
//			PRHI::Get()->SetGraphicsPipeline(ShadowPipeline);
			auto PrimitivePipeline = Primitive.second->GetPipeline();
			if(PrimitivePipeline != PCurrentPipeline)
			{
				PRHI::Get()->SetGraphicsPipeline(PrimitivePipeline);
			}
 			PRHI::Get()->SetMeshBuffer(Primitive.second->GetMeshBuffer());
			PRHI::Get()->SetRenderResourceTable("PerCameraBuffer", MainLightBuffer->GetHandleOffset());
 			ShaderResourceBinding(Primitive.second);
 			PRHI::Get()->DrawPrimitiveIndexedInstanced(Primitive.second->GetMeshBuffer()->GetIndexCount());
   		}
   		PRHI::Get()->EndRenderRTBuffer(PShadowMap->GetDepthStencilBuffer());
		if(NeedExportDepth)
		{
			PRHI::Get()->ReadBackRTBuffer(PShadowMap->GetDepthStencilBuffer());
			NeedExportDepth = false;
		}
	}

	void PRenderer::RenderCurrentScene(PRenderScene* RenderScene)
	{
		PRHI::Get()->PrepareBufferHeap();
		auto CurrentDrawPrimitives = RenderScene->GetPrimitives();
		for(auto Primitive : CurrentDrawPrimitives)
		{
			UpdatePrimitiveBuffer(Primitive.second);
			auto PrimitivePipeline = Primitive.second->GetPipeline();
			if (PrimitivePipeline != PCurrentPipeline)
			{
				PRHI::Get()->SetGraphicsPipeline(PrimitivePipeline);
			}
			PRHI::Get()->SetMeshBuffer(Primitive.second->GetMeshBuffer());
			PRHI::Get()->SetRenderResourceTable("PerCameraBuffer", PerCameraBuffer->GetHandleOffset());
			ShaderResourceBinding(Primitive.second);
   			if(PShadowMap)
  			{
   				PRHI::Get()->SetRenderResourceTable("ShadowTexture", PShadowMap->GetDepthStencilBuffer()->PRTTexture->GetHandleOffset());
   			}
			PRHI::Get()->DrawPrimitiveIndexedInstanced(Primitive.second->GetMeshBuffer()->GetIndexCount());
		}
	}
}