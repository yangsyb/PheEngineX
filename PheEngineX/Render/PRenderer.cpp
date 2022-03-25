#include "pch.h"
#include "PRenderer.h"
#include "RHI/PRHI.h"
#include "PRenderThread.h"
#include "Engine/PShaderManager.h"

namespace Phe
{
	PRenderer::PRenderer() : PerCameraBuffer(nullptr), CurrentCameraData(PerCameraCBuffer()), PShadowMap(nullptr)
	{
		PRHI::Get()->InitRHI();
	}

	PRenderer::~PRenderer()
	{
// 		ReleasePtr(PShadowMap);
// 		ReleasePtr(ShadowPipeline);
	}

	void PRenderer::Initialize()
	{
		PRHI::Get()->InitGraphicsPipeline();
		PRHI::Get()->ResizeWindow(1920, 1080);
	}

	
	void PRenderer::RenderFrameBegin(PRenderScene* RenderScene)
	{
		if(!PerCameraBuffer)
		{
			PerCameraBuffer = PRHI::Get()->CreateCommonBuffer(sizeof(PerCameraCBuffer), 1);
		}
		CurrentCameraData.Time = PRenderThread::Get()->GetCurrentTotalTime();

		if(RenderScene->GetMainLightBuffer())
		{
			PrepareShadowMap(RenderScene);
		}

		if (RenderScene->GetMainLight())
		{
			PLight* MainLight = RenderScene->GetMainLight();
			CurrentCameraData.ShadowTransform = RenderScene->GetMainLight()->GetPassCBuffer().ShadowTransform;

			if (MainLight->GetIsDynamic())
			{
				static bool IsLeft = true;
				glm::vec3 CurrentPositon = MainLight->GetLightTransform().GetPosition();
				if(IsLeft)
				{
					MainLight->SetLightPosition(glm::vec3(CurrentPositon.x, CurrentPositon.y-0.05, CurrentPositon.z));
					if(CurrentPositon.y-0.1<-55)
					{
						IsLeft = false;
					}
				}
				else
				{
					MainLight->SetLightPosition(glm::vec3(CurrentPositon.x, CurrentPositon.y + 0.05, CurrentPositon.z));
					if (CurrentPositon.y + 0.1 > 55)
					{
						IsLeft = true;
					}
				}
			}
		}
		PRHI::Get()->UpdateCommonBuffer(PerCameraBuffer, &CurrentCameraData);
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
			auto PerObj = Primitive->GetTransform().GetBufferData();
			PRHI::Get()->UpdateCommonBuffer(Primitive->GetPerObjBuffer(), &PerObj);
		}
		if(Primitive->GetPerMatBuffer())
		{
			auto PerMat = Primitive->GetMaterial()->GetMaterialBuffer();
			PRHI::Get()->UpdateCommonBuffer(Primitive->GetPerMatBuffer(), &PerMat);
		}
	}

	void PRenderer::UpdateCamera(PerCameraCBuffer CameraCBuffer)
	{
		CurrentCameraData.Proj = CameraCBuffer.Proj;
		CurrentCameraData.View = CameraCBuffer.View;
		CurrentCameraData.CameraLocation = CameraCBuffer.CameraLocation;
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
		PRHI::Get()->SetRenderResourceTable("Texture", Primitive->GetMaterial()->GetGPUTextureBuffer().at(0)->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("PerObjectBuffer", Primitive->GetPerObjBuffer()->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("PerMaterialBuffer", Primitive->GetPerMatBuffer()->GetHandleOffset());
	}

	void PRenderer::PrepareShadowMap(PRenderScene* RenderScene)
	{
		PGPUCommonBuffer* MainLightBuffer = RenderScene->GetMainLightBuffer();
		if(PShadowMap)
		{
			ReleasePtr(PShadowMap);
		}
  		PShadowMap = PRHI::Get()->CreateRenderTarget("ShadowMap", 2048, 2048);
 		PShadowMap->AddDepthStencilBuffer();
		PShadowMap->GetDepthStencilBuffer()->PRTTexture = PRHI::Get()->CreateTexture("ShadowMapTexture", PShadowMap->GetDepthStencilBuffer());
   		PRHI::Get()->BeginRenderRTBuffer(PShadowMap->GetDepthStencilBuffer());
// 		if (!ShadowPipeline)
// 		{
// 			PShader* ShadowShader = PRHI::Get()->CreateShader("PositionShader", L"Shaders\\Position.hlsl", "VS", "");
// 			ShadowPipeline = PRHI::Get()->CreatePipeline(ShadowShader);
// 			PRHI::Get()->UpdatePipeline(ShadowPipeline, PShadowMap);
//   		}
  
  		PRHI::Get()->PrepareBufferHeap();
 		PRHI::Get()->SetRenderTarget(PShadowMap);
  		auto CurrentDrawPrimitives = RenderScene->GetPrimitives();
   		for (auto Primitive : CurrentDrawPrimitives)
   		{
 			UpdatePrimitiveBuffer(Primitive);
			auto LightData = RenderScene->GetMainLight()->GetPassCBuffer();
			LightData.Time = PRenderThread::Get()->GetCurrentTotalTime();
			PRHI::Get()->UpdateCommonBuffer(MainLightBuffer, &LightData);
//			PRHI::Get()->SetGraphicsPipeline(ShadowPipeline);
			PRHI::Get()->SetGraphicsPipeline(Primitive->GetPipeline());
 			PRHI::Get()->SetMeshBuffer(Primitive->GetMeshBuffer());
			PRHI::Get()->SetRenderResourceTable("PerCameraBuffer", MainLightBuffer->GetHandleOffset());
 			ShaderResourceBinding(Primitive);
 			PRHI::Get()->DrawPrimitiveIndexedInstanced(Primitive->GetMeshBuffer()->GetIndexCount());
   		}
   		PRHI::Get()->EndRenderRTBuffer(PShadowMap->GetDepthStencilBuffer());
	}

	void PRenderer::RenderCurrentScene(PRenderScene* RenderScene)
	{
		PRHI::Get()->PrepareBufferHeap();
		auto CurrentDrawPrimitives = RenderScene->GetPrimitives();
		for(auto Primitive : CurrentDrawPrimitives)
		{
			UpdatePrimitiveBuffer(Primitive);
			PRHI::Get()->SetGraphicsPipeline(Primitive->GetPipeline());
			PRHI::Get()->SetMeshBuffer(Primitive->GetMeshBuffer());
			PRHI::Get()->SetRenderResourceTable("PerCameraBuffer", PerCameraBuffer->GetHandleOffset());
			ShaderResourceBinding(Primitive);
   			if(PShadowMap)
  			{
   				PRHI::Get()->SetRenderResourceTable("ShadowTexture", PShadowMap->GetDepthStencilBuffer()->PRTTexture->GetHandleOffset());
   			}
			PRHI::Get()->DrawPrimitiveIndexedInstanced(Primitive->GetMeshBuffer()->GetIndexCount());
		}
	}
}