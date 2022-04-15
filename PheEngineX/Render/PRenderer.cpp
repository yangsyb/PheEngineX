#include "pch.h"
#include "PRenderer.h"
#include "RHI/PRHI.h"
#include "PRenderThread.h"
#include "Engine/Editor/PShaderManager.h"
#include "GPUResource/PGPUTexture.h"
#include "Engine/Editor/PTexture.h"
#include "Engine/Editor/PAssetManager.h"
#include "Engine/Core/PEngine.h"
namespace Phe
{

	void BloomData::Initialize()
	{
		HighLightPipeline = PRHI::Get()->CreatePipeline(PShaderManager::Get()->GetCompiledShaderByName("HighLightShader"), P_RasterizerDesc(), P_BlendState(), P_DepthStencilState());
		DownSamplePipeline = PRHI::Get()->CreatePipeline(PShaderManager::Get()->GetCompiledShaderByName("BloomDown"), P_RasterizerDesc(), P_BlendState(), P_DepthStencilState());
		UpSamplePipeline = PRHI::Get()->CreatePipeline(PShaderManager::Get()->GetCompiledShaderByName("BloomUp"), P_RasterizerDesc(), P_BlendState(), P_DepthStencilState());
		MergeSamplePipeline = PRHI::Get()->CreatePipeline(PShaderManager::Get()->GetCompiledShaderByName("BloomMerge"), P_RasterizerDesc(), P_BlendState(), P_DepthStencilState());
		ToneMapPipeline = PRHI::Get()->CreatePipeline(PShaderManager::Get()->GetCompiledShaderByName("ToneMap"), P_RasterizerDesc(), P_BlendState(), P_DepthStencilState());

		BloomDownBuffer = PRHI::Get()->CreateCommonBuffer(sizeof(PINT2), 1);
		BloomDownBuffer1 = PRHI::Get()->CreateCommonBuffer(sizeof(PINT2), 1);
		BloomDownBuffer2 = PRHI::Get()->CreateCommonBuffer(sizeof(PINT2), 1);
		BloomDownBuffer3 = PRHI::Get()->CreateCommonBuffer(sizeof(PINT2), 1);
		BloomDownBuffer4 = PRHI::Get()->CreateCommonBuffer(sizeof(PINT2), 1);
		BloomUpBuffer = PRHI::Get()->CreateCommonBuffer(sizeof(PINT2), 1);
		BloomUpBuffer1 = PRHI::Get()->CreateCommonBuffer(sizeof(PINT2), 1);
		BloomUpBuffer2 = PRHI::Get()->CreateCommonBuffer(sizeof(PINT2), 1);
		BloomUpBuffer3 = PRHI::Get()->CreateCommonBuffer(sizeof(PINT2), 1);
		ToneMapBuffer = PRHI::Get()->CreateCommonBuffer(sizeof(PINT2), 1);
	}


	void DOFData::Initialize()
	{
		DOFCOCPipeline = PRHI::Get()->CreatePipeline(PShaderManager::Get()->GetCompiledShaderByName("DOFCOC"), P_RasterizerDesc(), P_BlendState(), P_DepthStencilState());

		DOFCOCBuffer = PRHI::Get()->CreateCommonBuffer(sizeof(DOFDataStruct), 1);
	}




	PRenderer::PRenderer() : PerCameraBuffer(nullptr), PShadowMap(nullptr), PCurrentPipeline(nullptr), PIBLBRDFRenderTarget(nullptr), PExportRenderTarget(nullptr), PerOrthoCameraBuffer(nullptr), ScreenMeshBuffer(nullptr),
		PHDRRenderTarget(nullptr), Bloom(new BloomData()), DOF(new DOFData()), ScreenWidth(0), ScreenHeight(0)
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

		ScreenMeshBuffer = PRHI::Get()->CreateMeshBuffer();
		ScreenMeshBuffer->SetScreenMeshBuffer();
		PRHI::Get()->UpdateMeshBuffer(ScreenMeshBuffer);

		Bloom->Initialize();
		DOF->Initialize();
	}


	void PRenderer::RenderFrameBegin(PRenderScene* RenderScene)
	{
		PRHI::Get()->BeginFrame();
		PCurrentPipeline = nullptr;
		ScreenWidth = PEngine::GetSingleton().GetWindow()->GetWidth();
		ScreenHeight = PEngine::GetSingleton().GetWindow()->GetHeight();
	}

	void PRenderer::RenderFrameEnd(PRenderScene* RenderScene)
	{
		PRHI::Get()->EndFrame();
	}

	void PRenderer::Render(PRenderScene* RenderScene)
	{
		PRenderLight* MainRenderLight = nullptr;
		ExportPass(RenderScene);
		if (NeedExportDepth)
		{
			PRHI::Get()->ReadBackRTBuffer(PExportRenderTarget->GetDepthStencilBuffer());
			NeedExportDepth = false;
		}
		if (MainRenderLight = RenderScene->GetMainRenderLight())
		{
			ShadowPass(RenderScene);
			HDRPass(RenderScene);
			DOFPass();
			BloomPass();

			PRHI::Get()->BeginRenderBackBuffer();
			ToneMappingPass();
			PRHI::Get()->EndRenderBackBuffer();
			return;
		}
		ReleasePtr(PShadowMap);
	}

	void PRenderer::DestroyRenderer()
	{
		ReleasePtr(PerCameraBuffer);
		ReleasePtr(PerOrthoCameraBuffer);
		ReleasePtr(PShadowMap);
		ReleasePtr(PExportRenderTarget);
		ReleasePtr(PHDRRenderTarget);
		ReleasePtr(ScreenMeshBuffer);
		ReleasePtr(Bloom);
		ReleasePtr(DOF);
	}


	// 	void PRenderer::UpdateTextureCube()
	// 	{
	// 		PTexture* SkyTEXT = PAssetManager::GetSingleton().GetTextureData("SkyBoxTexture");
	// 		PSkyTexture = PRHI::Get()->CreateTexture(SkyTEXT->GetTextureName(), SkyTEXT->GetTextureFileName(), P_TextureType::P_TextureCube);
	// 	}

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
		if (!PShadowMap)
		{
			PShadowMap = PRHI::Get()->CreateRenderTarget("ShadowMap", 2048, 2048);
			PShadowMap->AddDepthStencilBuffer(DepthStencil_Format);
			PShadowMap->GetDepthStencilBuffer()->PRTTexture = PRHI::Get()->CreateTexture("ShadowMapTexture", PShadowMap->GetDepthStencilBuffer(), P_TextureType::P_Texture2D);
		}
		//   		PRHI::Get()->BeginRenderRTBuffer(PShadowMap->GetDepthStencilBuffer());
		PRHI::Get()->BeginRenderRenderTarget(PShadowMap, L"ShadowPass");

		PRHI::Get()->SetRenderTarget(PShadowMap);

		auto CurrentDrawPrimitives = RenderScene->GetPrimitives();
		for (auto Primitive : CurrentDrawPrimitives)
		{
			// 			UpdatePrimitiveBuffer(Primitive.second);
			auto PrimitivePipeline = Primitive.second->GetPipeline(PipelineType::ShadowPipeline);
			if (PrimitivePipeline != PCurrentPipeline)
			{
				PRHI::Get()->SetGraphicsPipeline(PrimitivePipeline);
				PCurrentPipeline = PrimitivePipeline;
			}
			PRHI::Get()->SetMeshBuffer(Primitive.second->GetMeshBuffer());
			PRHI::Get()->SetRenderResourceTable("PerCameraBuffer", MainLightBuffer->GetHandleOffset());
			ShaderResourceBinding(Primitive.second);
			PRHI::Get()->DrawPrimitiveIndexedInstanced(Primitive.second->GetMeshBuffer()->GetIndexCount());
		}
		//   		PRHI::Get()->EndRenderRTBuffer(PShadowMap->GetDepthStencilBuffer());
		PRHI::Get()->EndRenderRenderTarget(PShadowMap);
	}


	void PRenderer::IBLBRDFPass(PRenderScene* RenderScene)
	{
		PIBLBRDFRenderTarget = PRHI::Get()->CreateRenderTarget("IBLBRDF", 1024, 1024);
		PIBLBRDFRenderTarget->AddColorBuffer(1, P_TextureFormat::P_FORMAT_B8G8R8A8_UNORM);
		PIBLBRDFRenderTarget->GetColorBuffer(1)->PRTTexture = PRHI::Get()->CreateTexture("IBLBRDFTexture", PIBLBRDFRenderTarget->GetColorBuffer(1), P_TextureType::P_Texture2D);
		//		PRHI::Get()->BeginRenderRTBuffer(PIBLBRDFRenderTarget->GetColorBuffer(1));
		PRHI::Get()->SetRenderTarget(PIBLBRDFRenderTarget);

		//		PRHI::Get()->EndRenderRTBuffer(PIBLBRDFRenderTarget->GetColorBuffer(1));
	}


	void PRenderer::ExportPass(PRenderScene* RenderScene)
	{
		if (!PExportRenderTarget)
		{
			PExportRenderTarget = PRHI::Get()->CreateRenderTarget("Export", 1024, 1024);
			PExportRenderTarget->AddDepthStencilBuffer(DepthStencil_Format);
			PExportRenderTarget->GetDepthStencilBuffer()->PRTTexture = PRHI::Get()->CreateTexture("ExportDepthTexture", PExportRenderTarget->GetDepthStencilBuffer(), P_TextureType::P_Texture2D);
		}
		//		PRHI::Get()->BeginRenderRTBuffer(PExportRenderTarget->GetDepthStencilBuffer());
		PRHI::Get()->BeginRenderRenderTarget(PExportRenderTarget, L"ExportPass");

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
		//		PRHI::Get()->EndRenderRTBuffer(PExportRenderTarget->GetDepthStencilBuffer());
		PRHI::Get()->EndRenderRenderTarget(PExportRenderTarget);
	}


	void PRenderer::SkyBoxPass(PRenderScene* RenderScene)
	{
		PPrimitive* SkySphere = RenderScene->GetSkySphere();
		PPipeline* SkyPipeline = SkySphere->GetPipeline(PipelineType::SkyPipeline);
		PRHI::Get()->SetGraphicsPipeline(SkyPipeline);
		PCurrentPipeline = SkyPipeline;
		PRHI::Get()->SetMeshBuffer(SkySphere->GetMeshBuffer());
		PRHI::Get()->SetRenderResourceTable("PerCameraBuffer", PerOrthoCameraBuffer->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("Texture", SkySphere->GetMaterial()->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("PerObjectBuffer", SkySphere->GetPerObjBuffer()->GetHandleOffset());
		PRHI::Get()->DrawPrimitiveIndexedInstanced(SkySphere->GetMeshBuffer()->GetIndexCount());
	}


	void PRenderer::HDRPass(PRenderScene* RenderScene)
	{
		// HDR
		if (!PHDRRenderTarget)
		{
			PHDRRenderTarget = PRHI::Get()->CreateRenderTarget("HDR", ScreenWidth, ScreenHeight);
			PHDRRenderTarget->AddColorBuffer(1, HDR_FORMAT);
			PHDRRenderTarget->AddDepthStencilBuffer(DepthStencil_Format);

			for (auto& ColorBuffer : PHDRRenderTarget->GetColorBuffer())
			{
				ColorBuffer->PRTTexture = PRHI::Get()->CreateTexture("HDRColorTexture", PHDRRenderTarget->GetColorBuffer().at(0), P_TextureType::P_Texture2D);
			}
			PHDRRenderTarget->GetDepthStencilBuffer()->PRTTexture = PRHI::Get()->CreateTexture("HDRDepthTexture", PHDRRenderTarget->GetDepthStencilBuffer(), P_TextureType::P_Texture2D);
		}

		PRHI::Get()->BeginRenderRenderTarget(PHDRRenderTarget, L"HDRPass");

		PRHI::Get()->SetRenderTarget(PHDRRenderTarget);

		auto CurrentDrawPrimitives = RenderScene->GetPrimitives();
		for (auto Primitive : CurrentDrawPrimitives)
		{
			auto PrimitivePipeline = Primitive.second->GetPipeline(PipelineType::HDRPipeline);
			if (PrimitivePipeline != PCurrentPipeline)
			{
				PRHI::Get()->SetGraphicsPipeline(PrimitivePipeline);
				PCurrentPipeline = PrimitivePipeline;
			}
			PRHI::Get()->SetMeshBuffer(Primitive.second->GetMeshBuffer());
			PRHI::Get()->SetRenderResourceTable("PerCameraBuffer", PerCameraBuffer->GetHandleOffset());
			ShaderResourceBinding(Primitive.second);
			if (PShadowMap)
			{
				PRHI::Get()->SetRenderResourceTable("ShadowTexture", PShadowMap->GetDepthStencilBuffer()->PRTTexture->GetHandleOffset());
				PRHI::Get()->SetRenderResourceTable("PerLightBuffer", RenderScene->GetMainRenderLight()->GetPerLightBuffer()->GetHandleOffset());
			}
			PRHI::Get()->DrawPrimitiveIndexedInstanced(Primitive.second->GetMeshBuffer()->GetIndexCount());
		}

		PRHI::Get()->EndRenderRenderTarget(PHDRRenderTarget);
	}


	void PRenderer::DOFPass()
	{
		if(!DOF->PDOFCOCRenderTarget)
		{
			DOF->PDOFCOCRenderTarget = PRHI::Get()->CreateRenderTarget("DOFCOC", ScreenWidth, ScreenHeight);
			DOF->PDOFCOCRenderTarget->AddColorBuffer(1, LDR_FORMAT);
			DOF->PDOFCOCRenderTarget->AddDepthStencilBuffer(DepthStencil_Format);

//			DOF->PDOFCOCRenderTarget->GetDepthStencilBuffer()->PRTTexture = PRHI::Get()->CreateTexture("DOFCOC", DOF->PDOFCOCRenderTarget->GetDepthStencilBuffer(), P_TextureType::P_Texture2D);
		}
		PRHI::Get()->UpdatePipeline(DOF->DOFCOCPipeline, LDR_FORMAT, DepthStencil_Format);

		PRHI::Get()->BeginRenderRenderTarget(DOF->PDOFCOCRenderTarget, L"DOFCOCPass");

		PRHI::Get()->SetRenderTarget(DOF->PDOFCOCRenderTarget);

		std::shared_ptr<void> DOFDataSt = std::make_shared<DOFDataStruct>(DOFDataStruct(0.024f, float(ScreenWidth), float(ScreenHeight)));

		PRHI::Get()->SetGraphicsPipeline(DOF->DOFCOCPipeline);
		PRHI::Get()->UpdateCommonBuffer(DOF->DOFCOCBuffer, DOFDataSt);
		PRHI::Get()->SetMeshBuffer(ScreenMeshBuffer);
		PRHI::Get()->SetRenderResourceTable("RTSize", DOF->DOFCOCBuffer->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("Texture", PHDRRenderTarget->GetDepthStencilBuffer()->PRTTexture->GetHandleOffset());
		PRHI::Get()->DrawInstanced(3);
		PRHI::Get()->EndRenderRenderTarget(DOF->PDOFCOCRenderTarget);
	}

	void PRenderer::BloomPass()
	{
		// Extract HighLight
		if (!Bloom->PHighLightRenderTarget)
		{
			Bloom->PHighLightRenderTarget = PRHI::Get()->CreateRenderTarget("HighLight", ScreenWidth / 4, ScreenHeight / 4);
			Bloom->PHighLightRenderTarget->AddColorBuffer(1, RGBOnly_Format);
			Bloom->PHighLightRenderTarget->AddDepthStencilBuffer(DepthStencil_Format);

			for (auto ColorBuffer : Bloom->PHighLightRenderTarget->GetColorBuffer())
			{
				ColorBuffer->PRTTexture = PRHI::Get()->CreateTexture("HighLight", Bloom->PHighLightRenderTarget->GetColorBuffer().at(0), P_TextureType::P_Texture2D);
			}
		}

		PRHI::Get()->UpdatePipeline(Bloom->HighLightPipeline, Bloom->PHighLightRenderTarget);

		PRHI::Get()->BeginRenderRenderTarget(Bloom->PHighLightRenderTarget, L"HighLightPass");

		PRHI::Get()->SetRenderTarget(Bloom->PHighLightRenderTarget);

		std::shared_ptr<void> RenderTargetSize = std::make_shared<PINT2>(PINT2(ScreenWidth, ScreenHeight));
		PRHI::Get()->SetGraphicsPipeline(Bloom->HighLightPipeline);
		PRHI::Get()->UpdateCommonBuffer(Bloom->BloomDownBuffer, RenderTargetSize);
		PRHI::Get()->SetMeshBuffer(ScreenMeshBuffer);
		PRHI::Get()->SetRenderResourceTable("RTSize", Bloom->BloomDownBuffer->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("BloomTexture", PHDRRenderTarget->GetColorBuffer().at(0)->PRTTexture->GetHandleOffset());
		PRHI::Get()->DrawInstanced(3);

		PRHI::Get()->EndRenderRenderTarget(Bloom->PHighLightRenderTarget);

		// Bloom Down
		if (!Bloom->PBloomDownRenderTarget)
		{
			Bloom->PBloomDownRenderTarget = PRHI::Get()->CreateRenderTarget("BloomDown", ScreenWidth / 8, ScreenHeight / 8);
			Bloom->PBloomDownRenderTarget->AddColorBuffer(1, RGBOnly_Format);
			Bloom->PBloomDownRenderTarget->AddDepthStencilBuffer(DepthStencil_Format);

			for (auto ColorBuffer : Bloom->PBloomDownRenderTarget->GetColorBuffer())
			{
				ColorBuffer->PRTTexture = PRHI::Get()->CreateTexture("BloomDown", Bloom->PBloomDownRenderTarget->GetColorBuffer().at(0), P_TextureType::P_Texture2D);
			}
		}

		PRHI::Get()->UpdatePipeline(Bloom->DownSamplePipeline, Bloom->PBloomDownRenderTarget);

		PRHI::Get()->BeginRenderRenderTarget(Bloom->PBloomDownRenderTarget, L"BloomDown0");

		PRHI::Get()->SetRenderTarget(Bloom->PBloomDownRenderTarget);

		std::shared_ptr<void> BloomDownSize1 = std::make_shared<PINT2>(PINT2(ScreenWidth / 4, ScreenHeight / 4));
		PRHI::Get()->SetGraphicsPipeline(Bloom->DownSamplePipeline);
		PRHI::Get()->UpdateCommonBuffer(Bloom->BloomDownBuffer1, BloomDownSize1);
		PRHI::Get()->SetMeshBuffer(ScreenMeshBuffer);
		PRHI::Get()->SetRenderResourceTable("RTSize", Bloom->BloomDownBuffer1->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("BloomTexture", Bloom->PHighLightRenderTarget->GetColorBuffer().at(0)->PRTTexture->GetHandleOffset());
		PRHI::Get()->DrawInstanced(3);

		PRHI::Get()->EndRenderRenderTarget(Bloom->PBloomDownRenderTarget);

		// Bloom Down1
		if (!Bloom->PBloomDownRenderTarget1)
		{
			Bloom->PBloomDownRenderTarget1 = PRHI::Get()->CreateRenderTarget("BloomDown1", ScreenWidth / 16, ScreenHeight / 16);
			Bloom->PBloomDownRenderTarget1->AddColorBuffer(1, RGBOnly_Format);
			Bloom->PBloomDownRenderTarget1->AddDepthStencilBuffer(DepthStencil_Format);

			for (auto ColorBuffer : Bloom->PBloomDownRenderTarget1->GetColorBuffer())
			{
				ColorBuffer->PRTTexture = PRHI::Get()->CreateTexture("BloomDown1", Bloom->PBloomDownRenderTarget1->GetColorBuffer().at(0), P_TextureType::P_Texture2D);
			}
		}

		PRHI::Get()->BeginRenderRenderTarget(Bloom->PBloomDownRenderTarget1, L"BloomDown1");

		PRHI::Get()->SetRenderTarget(Bloom->PBloomDownRenderTarget1);

		std::shared_ptr<void> BloomDownSize2 = std::make_shared<PINT2>(PINT2(ScreenWidth / 8, ScreenHeight / 8));
		PRHI::Get()->SetGraphicsPipeline(Bloom->DownSamplePipeline);
		PRHI::Get()->UpdateCommonBuffer(Bloom->BloomDownBuffer2, BloomDownSize2);
		PRHI::Get()->SetMeshBuffer(ScreenMeshBuffer);
		PRHI::Get()->SetRenderResourceTable("RTSize", Bloom->BloomDownBuffer2->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("BloomTexture", Bloom->PBloomDownRenderTarget->GetColorBuffer().at(0)->PRTTexture->GetHandleOffset());
		PRHI::Get()->DrawInstanced(3);

		PRHI::Get()->EndRenderRenderTarget(Bloom->PBloomDownRenderTarget1);

		// Bloom Down2
		if (!Bloom->PBloomDownRenderTarget2)
		{
			Bloom->PBloomDownRenderTarget2 = PRHI::Get()->CreateRenderTarget("BloomDown2", ScreenWidth / 32, ScreenHeight / 32);
			Bloom->PBloomDownRenderTarget2->AddColorBuffer(1, RGBOnly_Format);
			Bloom->PBloomDownRenderTarget2->AddDepthStencilBuffer(DepthStencil_Format);

			for (auto ColorBuffer : Bloom->PBloomDownRenderTarget2->GetColorBuffer())
			{
				ColorBuffer->PRTTexture = PRHI::Get()->CreateTexture("BloomDown2", Bloom->PBloomDownRenderTarget2->GetColorBuffer().at(0), P_TextureType::P_Texture2D);
			}
		}

		PRHI::Get()->BeginRenderRenderTarget(Bloom->PBloomDownRenderTarget2, L"BloomDown2");

		PRHI::Get()->SetRenderTarget(Bloom->PBloomDownRenderTarget2);

		std::shared_ptr<void> BloomDownSize3 = std::make_shared<PINT2>(PINT2(ScreenWidth / 16, ScreenHeight / 16));
		PRHI::Get()->SetGraphicsPipeline(Bloom->DownSamplePipeline);
		PRHI::Get()->UpdateCommonBuffer(Bloom->BloomDownBuffer3, BloomDownSize3);
		PRHI::Get()->SetMeshBuffer(ScreenMeshBuffer);
		PRHI::Get()->SetRenderResourceTable("RTSize", Bloom->BloomDownBuffer3->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("BloomTexture", Bloom->PBloomDownRenderTarget1->GetColorBuffer().at(0)->PRTTexture->GetHandleOffset());
		PRHI::Get()->DrawInstanced(3);

		PRHI::Get()->EndRenderRenderTarget(Bloom->PBloomDownRenderTarget2);

		// Bloom Down3
		if (!Bloom->PBloomDownRenderTarget3)
		{
			Bloom->PBloomDownRenderTarget3 = PRHI::Get()->CreateRenderTarget("BloomDown3", ScreenWidth / 64, ScreenHeight / 64);
			Bloom->PBloomDownRenderTarget3->AddColorBuffer(1, RGBOnly_Format);
			Bloom->PBloomDownRenderTarget3->AddDepthStencilBuffer(DepthStencil_Format);

			for (auto ColorBuffer : Bloom->PBloomDownRenderTarget3->GetColorBuffer())
			{
				ColorBuffer->PRTTexture = PRHI::Get()->CreateTexture("BloomDown3", Bloom->PBloomDownRenderTarget3->GetColorBuffer().at(0), P_TextureType::P_Texture2D);
			}
		}

		PRHI::Get()->BeginRenderRenderTarget(Bloom->PBloomDownRenderTarget3, L"BloomDown3");

		PRHI::Get()->SetRenderTarget(Bloom->PBloomDownRenderTarget3);

		std::shared_ptr<void> BloomDownSize4 = std::make_shared<PINT2>(PINT2(ScreenWidth / 32, ScreenHeight / 32));
		PRHI::Get()->SetGraphicsPipeline(Bloom->DownSamplePipeline);
		PRHI::Get()->UpdateCommonBuffer(Bloom->BloomDownBuffer4, BloomDownSize4);
		PRHI::Get()->SetMeshBuffer(ScreenMeshBuffer);
		PRHI::Get()->SetRenderResourceTable("RTSize", Bloom->BloomDownBuffer4->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("BloomTexture", Bloom->PBloomDownRenderTarget2->GetColorBuffer().at(0)->PRTTexture->GetHandleOffset());
		PRHI::Get()->DrawInstanced(3);

		PRHI::Get()->EndRenderRenderTarget(Bloom->PBloomDownRenderTarget3);

		//Bloom Up
		if (!Bloom->PBloomUpRenderTarget0)
		{
			Bloom->PBloomUpRenderTarget0 = PRHI::Get()->CreateRenderTarget("BloomUp0", ScreenWidth / 32, ScreenHeight / 32);
			Bloom->PBloomUpRenderTarget0->AddColorBuffer(1, RGBOnly_Format);
			Bloom->PBloomUpRenderTarget0->AddDepthStencilBuffer(DepthStencil_Format);

			for (auto ColorBuffer : Bloom->PBloomUpRenderTarget0->GetColorBuffer())
			{
				ColorBuffer->PRTTexture = PRHI::Get()->CreateTexture("BloomUp0", Bloom->PBloomUpRenderTarget0->GetColorBuffer().at(0), P_TextureType::P_Texture2D);
			}
		}

		PRHI::Get()->UpdatePipeline(Bloom->UpSamplePipeline, Bloom->PBloomUpRenderTarget0);

		PRHI::Get()->BeginRenderRenderTarget(Bloom->PBloomUpRenderTarget0, L"BloomUp0");

		PRHI::Get()->SetRenderTarget(Bloom->PBloomUpRenderTarget0);

		std::shared_ptr<void> BloomUpSize = std::make_shared<PINT2>(PINT2(ScreenWidth / 32, ScreenHeight / 32));
		PRHI::Get()->SetGraphicsPipeline(Bloom->UpSamplePipeline);
		PRHI::Get()->UpdateCommonBuffer(Bloom->BloomUpBuffer, BloomUpSize);
		PRHI::Get()->SetMeshBuffer(ScreenMeshBuffer);
		PRHI::Get()->SetRenderResourceTable("RTSize", Bloom->BloomUpBuffer->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("BloomTexture", Bloom->PBloomDownRenderTarget2->GetColorBuffer().at(0)->PRTTexture->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("Texture", Bloom->PBloomDownRenderTarget3->GetColorBuffer().at(0)->PRTTexture->GetHandleOffset());
		PRHI::Get()->DrawInstanced(3);

		PRHI::Get()->EndRenderRenderTarget(Bloom->PBloomUpRenderTarget0);

		//Bloom Up1
		if (!Bloom->PBloomUpRenderTarget1)
		{
			Bloom->PBloomUpRenderTarget1 = PRHI::Get()->CreateRenderTarget("BloomUp1", ScreenWidth / 16, ScreenHeight / 16);
			Bloom->PBloomUpRenderTarget1->AddColorBuffer(1, RGBOnly_Format);
			Bloom->PBloomUpRenderTarget1->AddDepthStencilBuffer(DepthStencil_Format);

			for (auto ColorBuffer : Bloom->PBloomUpRenderTarget1->GetColorBuffer())
			{
				ColorBuffer->PRTTexture = PRHI::Get()->CreateTexture("BloomUp1", Bloom->PBloomUpRenderTarget1->GetColorBuffer().at(0), P_TextureType::P_Texture2D);
			}
		}

		PRHI::Get()->BeginRenderRenderTarget(Bloom->PBloomUpRenderTarget1, L"BloomUp1");

		PRHI::Get()->SetRenderTarget(Bloom->PBloomUpRenderTarget1);

		std::shared_ptr<void> BloomUpSize1 = std::make_shared<PINT2>(PINT2(ScreenWidth / 16, ScreenHeight / 16));
		PRHI::Get()->SetGraphicsPipeline(Bloom->UpSamplePipeline);
		PRHI::Get()->UpdateCommonBuffer(Bloom->BloomUpBuffer1, BloomUpSize1);
		PRHI::Get()->SetMeshBuffer(ScreenMeshBuffer);
		PRHI::Get()->SetRenderResourceTable("RTSize", Bloom->BloomUpBuffer1->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("BloomTexture", Bloom->PBloomDownRenderTarget1->GetColorBuffer().at(0)->PRTTexture->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("Texture", Bloom->PBloomUpRenderTarget0->GetColorBuffer().at(0)->PRTTexture->GetHandleOffset());
		PRHI::Get()->DrawInstanced(3);

		PRHI::Get()->EndRenderRenderTarget(Bloom->PBloomUpRenderTarget1);

		//Bloom Up2
		if (!Bloom->PBloomUpRenderTarget2)
		{
			Bloom->PBloomUpRenderTarget2 = PRHI::Get()->CreateRenderTarget("BloomUp2", ScreenWidth / 8, ScreenHeight / 8);
			Bloom->PBloomUpRenderTarget2->AddColorBuffer(1, RGBOnly_Format);
			Bloom->PBloomUpRenderTarget2->AddDepthStencilBuffer(DepthStencil_Format);

			for (auto ColorBuffer : Bloom->PBloomUpRenderTarget2->GetColorBuffer())
			{
				ColorBuffer->PRTTexture = PRHI::Get()->CreateTexture("BloomUp2", Bloom->PBloomUpRenderTarget2->GetColorBuffer().at(0), P_TextureType::P_Texture2D);
			}
		}

		PRHI::Get()->BeginRenderRenderTarget(Bloom->PBloomUpRenderTarget2, L"BloomUp2");

		PRHI::Get()->SetRenderTarget(Bloom->PBloomUpRenderTarget2);

		std::shared_ptr<void> BloomUpSize2 = std::make_shared<PINT2>(PINT2(ScreenWidth / 8, ScreenHeight / 8));
		PRHI::Get()->SetGraphicsPipeline(Bloom->UpSamplePipeline);
		PRHI::Get()->UpdateCommonBuffer(Bloom->BloomUpBuffer2, BloomUpSize2);
		PRHI::Get()->SetMeshBuffer(ScreenMeshBuffer);
		PRHI::Get()->SetRenderResourceTable("RTSize", Bloom->BloomUpBuffer2->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("BloomTexture", Bloom->PBloomDownRenderTarget->GetColorBuffer().at(0)->PRTTexture->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("Texture", Bloom->PBloomUpRenderTarget1->GetColorBuffer().at(0)->PRTTexture->GetHandleOffset());
		PRHI::Get()->DrawInstanced(3);

		PRHI::Get()->EndRenderRenderTarget(Bloom->PBloomUpRenderTarget2);

		//Bloom Sun Merge
		if (!Bloom->PBloomMergeRenderTarget)
		{
			Bloom->PBloomMergeRenderTarget = PRHI::Get()->CreateRenderTarget("BloomMerge", ScreenWidth / 4, ScreenHeight / 4);
			Bloom->PBloomMergeRenderTarget->AddColorBuffer(1, HDR_FORMAT);
			Bloom->PBloomMergeRenderTarget->AddDepthStencilBuffer(DepthStencil_Format);

			for (auto ColorBuffer : Bloom->PBloomMergeRenderTarget->GetColorBuffer())
			{
				ColorBuffer->PRTTexture = PRHI::Get()->CreateTexture("BloomMerge", Bloom->PBloomMergeRenderTarget->GetColorBuffer().at(0), P_TextureType::P_Texture2D);
			}
		}

		PRHI::Get()->UpdatePipeline(Bloom->MergeSamplePipeline, Bloom->PBloomMergeRenderTarget);

		PRHI::Get()->BeginRenderRenderTarget(Bloom->PBloomMergeRenderTarget, L"BloomMerge");

		PRHI::Get()->SetRenderTarget(Bloom->PBloomMergeRenderTarget);

		std::shared_ptr<void> BloomUpSize3 = std::make_shared<PINT2>(PINT2(ScreenWidth / 4, ScreenHeight / 4));
		PRHI::Get()->SetGraphicsPipeline(Bloom->MergeSamplePipeline);
		PRHI::Get()->UpdateCommonBuffer(Bloom->BloomUpBuffer3, BloomUpSize3);
		PRHI::Get()->SetMeshBuffer(ScreenMeshBuffer);
		PRHI::Get()->SetRenderResourceTable("RTSize", Bloom->BloomUpBuffer3->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("BloomTexture", Bloom->PHighLightRenderTarget->GetColorBuffer().at(0)->PRTTexture->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("Texture", Bloom->PBloomUpRenderTarget2->GetColorBuffer().at(0)->PRTTexture->GetHandleOffset());
		PRHI::Get()->DrawInstanced(3);

		PRHI::Get()->EndRenderRenderTarget(Bloom->PBloomMergeRenderTarget);
	}


	void PRenderer::ToneMappingPass()
	{
		PRHI::Get()->UpdatePipeline(Bloom->ToneMapPipeline, LDR_FORMAT, DepthStencil_Format);
		std::shared_ptr<void> ToneMapSize = std::make_shared<PINT2>(PINT2(ScreenWidth, ScreenHeight));
		PRHI::Get()->SetGraphicsPipeline(Bloom->ToneMapPipeline);
		PRHI::Get()->UpdateCommonBuffer(Bloom->ToneMapBuffer, ToneMapSize);
		PRHI::Get()->SetMeshBuffer(ScreenMeshBuffer);
		PRHI::Get()->SetRenderResourceTable("RTSize", Bloom->ToneMapBuffer->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("BloomTexture", Bloom->PBloomMergeRenderTarget->GetColorBuffer().at(0)->PRTTexture->GetHandleOffset());
		PRHI::Get()->SetRenderResourceTable("Texture", PHDRRenderTarget->GetColorBuffer().at(0)->PRTTexture->GetHandleOffset());
		PRHI::Get()->DrawInstanced(3);
	}

	void PRenderer::RenderCurrentScene(PRenderScene* RenderScene)
	{
		auto CurrentDrawPrimitives = RenderScene->GetPrimitives();
		for (auto Primitive : CurrentDrawPrimitives)
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
			if (PShadowMap)
			{
				PRHI::Get()->SetRenderResourceTable("ShadowTexture", PShadowMap->GetDepthStencilBuffer()->PRTTexture->GetHandleOffset());
				PRHI::Get()->SetRenderResourceTable("PerLightBuffer", RenderScene->GetMainRenderLight()->GetPerLightBuffer()->GetHandleOffset());
			}
			PRHI::Get()->DrawPrimitiveIndexedInstanced(Primitive.second->GetMeshBuffer()->GetIndexCount());
		}
		//		SkyBoxPass(RenderScene);
	}
}