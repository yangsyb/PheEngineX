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
		AddShader(DefaultShader);
		AddShader(WPOShader);
		AddShader(WPOTrunkShader);
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
		PRHI::Get()->CreateShader(ShaderName, FilePath);
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