#include "pch.h"
#include "PShaderManager.h"
#include "DX12/PDX12ShaderManager.h"
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

	PShaderManager::PShaderManager()
	{

	}

	PShaderManager::~PShaderManager()
	{

	}

}