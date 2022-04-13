#include "pch.h"
#include "PShaderManager.h"
#include "DX12/PDX12ShaderManager.h"
#include "RHI/PRHI.h"
namespace Phe
{
	PShaderManager* PShaderManager::ShaderManager = nullptr;

	PShaderManager* PShaderManager::PShaderManager::Get()
	{
		return ShaderManager;
	}

	void PShaderManager::CreateShaderManager()
	{
#if defined (RenderDeviceDX12)
		ShaderManager = new PDX12Shadermanager();

#elif defined (RenderDeviceVulkan)


#elif defined (RenderDeviceOpenGL)


#endif
	}

	void PShaderManager::DestroyShaderManager()
	{
		ReleasePtr(ShaderManager);
	}

	PShaderManager::PShaderManager() : CurID(0)
	{

	}

	PShaderManager::~PShaderManager()
	{
		for (auto it : ShaderPool)
		{
			ReleasePtr(it.second);
		}
		ShaderPool.clear();
	}

	void PShaderManager::AddShader(std::string ShaderName, std::wstring FilePath)
	{
		PShader* NewShader = PRHI::Get()->CreateShader(ShaderName, FilePath);
		ShaderPool.insert({ NewShader->GetShaderName(), NewShader });
	}

	void PShaderManager::AddShader(PShader* Shader)
	{
		if (ShaderPool.count(Shader->GetShaderName()) == 0)
		{
			ShaderPool.insert({ Shader->GetShaderName(), Shader });
		}
	}

	PShader* PShaderManager::GetShaderByName(std::string ShaderName)
	{
		if (ShaderPool.count(ShaderName) > 0)
		{
			return ShaderPool.at(ShaderName);
		}
		return nullptr;
	}

	UINT PShaderManager::PropertyToID(std::string property)
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