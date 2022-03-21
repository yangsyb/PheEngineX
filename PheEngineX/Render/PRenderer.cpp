#include "pch.h"
#include "PRenderer.h"
#include "RHI/PRHI.h"
#include "PRenderThread.h"

namespace Phe
{
	PRenderer::PRenderer() : PerCameraBuffer(nullptr), CurrentCameraData(PerCameraCBuffer())
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
	}

	
	void PRenderer::RenderFrameBegin(PRenderScene* RenderScene)
	{
		if(!PerCameraBuffer)
		{
			PerCameraBuffer = PRHI::Get()->CreateCommonBuffer(sizeof(PerCameraCBuffer), 1);
		}
		CurrentCameraData.Time = PRenderThread::Get()->GetCurrentTotalTime();
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
		PRHI::Get()->SetRenderResourceTable("PerCameraBuffer", PerCameraBuffer->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("PerMaterialBuffer", Primitive->GetPerMatBuffer()->GetHandleOffset());
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
			ShaderResourceBinding(Primitive);
			
			PRHI::Get()->DrawPrimitiveIndexedInstanced(Primitive->GetMeshBuffer()->GetIndexCount());
		}
	}
}