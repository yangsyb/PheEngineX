#include "pch.h"
#include "PRenderer.h"
#include "RHI/PRHI.h"
#include "PRenderScene.h"
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
		PerCameraBuffer->AllocateData(0, &CurrentCameraData);
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
		delete PerCameraBuffer;
		PerCameraBuffer = nullptr;
	}

	void PRenderer::UpdateCamera(PerCameraCBuffer CameraCBuffer)
	{
		CurrentCameraData.Proj = CameraCBuffer.Proj;
		CurrentCameraData.View = CameraCBuffer.View;
		CurrentCameraData.CameraLocation = CameraCBuffer.CameraLocation;
	}

	void PRenderer::RenderCurrentScene(PRenderScene* RenderScene)
	{
		PRHI::Get()->PrepareBufferHeap();
		auto CurrentDrawPrimitives = RenderScene->GetPrimitives();
		for(auto Primitive : CurrentDrawPrimitives)
		{
			auto PerObj = Primitive->GetTransform().GetBufferData();
			Primitive->GetPerObjBuffer()->AllocateData(0, &PerObj);
			auto PerMat = Primitive->GetMaterial()->GetMaterialBuffer();
			Primitive->GetPerMatBuffer()->AllocateData(0, &PerMat);
			PRHI::Get()->SetGraphicsPipeline(Primitive->GetPipeline());
			PRHI::Get()->SetMeshBuffer(Primitive->GetMeshBuffer());

			PRHI::Get()->SetRenderResourceTable("Texture", Primitive->GetMaterial()->GetGPUTextureBuffer().at(0)->GetHandleOffset());
			PRHI::Get()->SetRenderResourceTable("PerObjectBuffer", Primitive->GetPerObjBuffer()->GetHandleOffset());
			PRHI::Get()->SetRenderResourceTable("PerCameraBuffer", PerCameraBuffer->GetHandleOffset());
			PRHI::Get()->SetRenderResourceTable("PerMaterialBuffer", Primitive->GetPerMatBuffer()->GetHandleOffset());
			
			PRHI::Get()->DrawPrimitiveIndexedInstanced(Primitive->GetMeshBuffer()->GetIndexCount());
		}
	}
}