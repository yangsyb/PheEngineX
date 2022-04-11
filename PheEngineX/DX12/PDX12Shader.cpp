#include "pch.h"
#if defined (RenderDeviceDX12)
#include "PDX12Shader.h"
#include "PDX12Shadermanager.h"
#include "Engine/Editor/PShaderManager.h"
#include "PDX12Converter.h"
namespace Phe
{
	PDX12Shader::PDX12Shader(const std::string ShaderName, const std::wstring FilePath, std::string VS, std::string PS) : 
		PShader(ShaderName, FilePath, VS, PS), PVS(nullptr), PPS(nullptr)
	{
		PDX12Shadermanager* DX12ShaderManager = dynamic_cast<PDX12Shadermanager*>(PShaderManager::Get());

		ShaderParameter perText("Texture", ShaderParamType::SRVDescriptorHeap, 4, 0, 0);
		ParamMap[DX12ShaderManager->PropertyToID(perText.name)] = UINT(Params.size());
		Params.push_back(perText);

		ShaderParameter shadowTexture("ShadowTexture", ShaderParamType::SRVDescriptorHeap, 1, 4, 0);
		ParamMap[DX12ShaderManager->PropertyToID(shadowTexture.name)] = UINT(Params.size());
		Params.push_back(shadowTexture);

		ShaderParameter TextureCube("SkyTexture", ShaderParamType::SRVDescriptorHeap, 1, 5, 0);
		ParamMap[DX12ShaderManager->PropertyToID(TextureCube.name)] = UINT(Params.size());
		Params.push_back(TextureCube);

		ShaderParameter perObject("PerObjectBuffer", ShaderParamType::CBVDescriptorHeap, 1, 0, 0);
		ParamMap[DX12ShaderManager->PropertyToID(perObject.name)] = UINT(Params.size());
		Params.push_back(perObject);

		ShaderParameter perPass("PerCameraBuffer", ShaderParamType::CBVDescriptorHeap, 1, 1, 0);
		ParamMap[DX12ShaderManager->PropertyToID(perPass.name)] = UINT(Params.size());
		Params.push_back(perPass);

		ShaderParameter perMaterial("PerMaterialBuffer", ShaderParamType::CBVDescriptorHeap, 1, 2, 0);
		ParamMap[DX12ShaderManager->PropertyToID(perMaterial.name)] = UINT(Params.size());
		Params.push_back(perMaterial);

		ShaderParameter perLight("PerLightBuffer", ShaderParamType::CBVDescriptorHeap, 1, 3, 0);
		ParamMap[DX12ShaderManager->PropertyToID(perLight.name)] = UINT(Params.size());
		Params.push_back(perLight);

		MakeDX12RasterizerState(RasterizerDesc, PRasterizerState);
		MakeDX12BlendState(BlendState, PBlendState);
		MakeDX12DepthStencilState(DepthStencilState, PDepthStencilState);
	}



	void PDX12Shader::SetRasterizerDesc(P_RasterizerDesc Raster)
	{
		RasterizerDesc = Raster;
		MakeDX12RasterizerState(RasterizerDesc, PRasterizerState);
	}

	void PDX12Shader::SetBlendState(P_BlendState Blend)
	{
		BlendState = Blend;
		MakeDX12BlendState(BlendState, PBlendState);
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
		psoDesc->RasterizerState.FrontCounterClockwise = true;
		psoDesc->DepthStencilState = PDepthStencilState;
		psoDesc->BlendState = PBlendState;
	}

}


#endif

