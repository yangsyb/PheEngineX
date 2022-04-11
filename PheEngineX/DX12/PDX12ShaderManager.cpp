#include "pch.h"
#if defined (RenderDeviceDX12)
#include "PDX12ShaderManager.h"
#include "RHI/PRHI.h"
namespace Phe
{
	PDX12Shadermanager::PDX12Shadermanager() : PShaderManager(), CurID(0)
	{
	//Default Shaders, You can Add shader whenever you want in runtime
		PShader* DefaultShader = new PShader("DefaultShader", L"Shaders\\color.hlsl");
		PShader* WPOShader = new PShader("WPOShader", L"Shaders\\tree.hlsl");
		PShader* WPOTrunkShader = new PShader("WPOTrunkShader", L"Shaders\\treetrunk.hlsl");
		PShader* TowerShader = new PShader("PBRShader", L"Shaders\\BRDFCommon.hlsl");
		PShader* SkyBoxShader = new PShader("SkyShader", L"Shaders\\SkyBox.hlsl", "VS", "PS", 
		P_RasterizerDesc(P_CULL_MODE::P_CULL_MODE_NONE,0,0,0), 
		P_BlendState(), 
		P_DepthStencilState(
			true,
			P_DEPTH_WRITE_MASK::P_DEPTH_WRITE_MASK_ALL,
			P_COMPARISON_FUNC::P_COMPARISON_FUNC_LESS_EQUAL,
			false,
			0xff,
			0xff,
			P_STENCIL_OP::P_STENCIL_OP_KEEP,
			P_STENCIL_OP::P_STENCIL_OP_KEEP,
			P_STENCIL_OP::P_STENCIL_OP_KEEP,
			P_COMPARISON_FUNC::P_COMPARISON_FUNC_ALWAYS,
			P_STENCIL_OP::P_STENCIL_OP_KEEP,
			P_STENCIL_OP::P_STENCIL_OP_KEEP,
			P_STENCIL_OP::P_STENCIL_OP_KEEP,
			P_COMPARISON_FUNC::P_COMPARISON_FUNC_ALWAYS
		)
		
		);
		AddShader(DefaultShader);
		AddShader(WPOShader);
		AddShader(WPOTrunkShader);
		AddShader(TowerShader);
		AddShader(SkyBoxShader);
		PropertyID.reserve(64);
	}

	PDX12Shadermanager::~PDX12Shadermanager()
	{
		for(auto it : ShaderPool)
		{
			ReleasePtr(it.second);
		}
		ShaderPool.clear();
	}

	void PDX12Shadermanager::AddShader(std::string ShaderName, std::wstring FilePath)
	{
		PShader* NewShader = PRHI::Get()->CreateShader(ShaderName, FilePath);
		ShaderPool.insert({ NewShader->GetShaderName(), NewShader});
	}

	void PDX12Shadermanager::AddShader(PShader* Shader)
	{
		if (ShaderPool.count(Shader->GetShaderName()) == 0)
		{
			ShaderPool.insert({ Shader->GetShaderName(), Shader });
		}
	}

	PShader* PDX12Shadermanager::GetShaderByName(std::string ShaderName)
	{
		if(ShaderPool.count(ShaderName) > 0)
		{
			return ShaderPool.at(ShaderName);
		}
		return nullptr;
	}

	UINT PDX12Shadermanager::PropertyToID(std::string property)
	{
		auto&& IDIterator = PropertyID.find(property);
		if (IDIterator == PropertyID.end())
		{
			unsigned int value = CurID;
			PropertyID[property] = CurID;
			++CurID;
			return value;
		}

		return IDIterator->second;
	}


}


#endif