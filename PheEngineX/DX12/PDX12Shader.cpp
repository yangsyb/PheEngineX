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

		ShaderParameter HDRTexture("BloomTexture", ShaderParamType::SRVDescriptorHeap, 1, 6, 0);
		ParamMap[DX12ShaderManager->PropertyToID(HDRTexture.name)] = UINT(Params.size());
		Params.push_back(HDRTexture);

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

		ShaderParameter RenderTargetSize("RTSize", ShaderParamType::CBVDescriptorHeap, 1, 4, 0);
		ParamMap[DX12ShaderManager->PropertyToID(RenderTargetSize.name)] = UINT(Params.size());
		Params.push_back(RenderTargetSize);

		
	}


	PDX12Shader::~PDX12Shader()
	{

	}

}


#endif

