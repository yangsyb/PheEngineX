#include "pch.h"
#if defined (RenderDeviceDX12)
#include "PDX12Shader.h"
#include "PDX12Shadermanager.h"
#include "Engine/PShaderManager.h"
namespace Phe
{
	PDX12Shader::PDX12Shader(const std::string ShaderName, const std::wstring FilePath, std::string VS, std::string PS) : 
		PShader(ShaderName, FilePath, VS, PS), PVS(nullptr), PPS(nullptr)
	{
		PDX12Shadermanager* DX12ShaderManager = dynamic_cast<PDX12Shadermanager*>(PShaderManager::Get());

		ShaderParameter perText("Texture", ShaderParamType::SRVDescriptorHeap, 2, 0, 0);
		ParamMap[DX12ShaderManager->PropertyToID(perText.name)] = UINT(Params.size());
		Params.push_back(perText);

		ShaderParameter perObject("PerObjectBuffer", ShaderParamType::CBVDescriptorHeap, 1, 0, 0);
		ParamMap[DX12ShaderManager->PropertyToID(perObject.name)] = UINT(Params.size());
		Params.push_back(perObject);

		ShaderParameter perPass("PerCameraBuffer", ShaderParamType::CBVDescriptorHeap, 1, 1, 0);
		ParamMap[DX12ShaderManager->PropertyToID(perPass.name)] = UINT(Params.size());
		Params.push_back(perPass);

		ShaderParameter perMaterial("PerMaterialBuffer", ShaderParamType::CBVDescriptorHeap, 1, 2, 0);
		ParamMap[DX12ShaderManager->PropertyToID(perMaterial.name)] = UINT(Params.size());
		Params.push_back(perMaterial);

  		ShaderParameter shadowTexture("ShadowTexture", ShaderParamType::SRVDescriptorHeap, 1, 2, 0);
  		ParamMap[DX12ShaderManager->PropertyToID(shadowTexture.name)] = UINT(Params.size());
  		Params.push_back(shadowTexture);

		PRasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		PBlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		PDepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	}



	void PDX12Shader::SetPSODesc(D3D12_GRAPHICS_PIPELINE_STATE_DESC* psoDesc)
	{
		if(PVS)
		{
			psoDesc->VS =
			{
				reinterpret_cast<BYTE*>(PVS->GetBufferPointer()),
				PVS->GetBufferSize()
			};
		}
		if(PPS)
		{
			psoDesc->PS =
			{
				reinterpret_cast<BYTE*>(PPS->GetBufferPointer()),
				PPS->GetBufferSize()
			};
		}

		psoDesc->pRootSignature = PRootSignature.Get();
		psoDesc->RasterizerState = PRasterizerState;
		psoDesc->RasterizerState.FrontCounterClockwise = TRUE;
		psoDesc->DepthStencilState = PDepthStencilState;
		psoDesc->BlendState = PBlendState;

		psoDesc->RasterizerState.DepthBias = 100000;
		psoDesc->RasterizerState.DepthBiasClamp = 0.0f;
		psoDesc->RasterizerState.SlopeScaledDepthBias = 1.0f;
	}

}


#endif

