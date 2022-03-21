#include "pch.h"
#if defined (RenderDeviceDX12)
#include "PDX12GPUTexture.h"
namespace Phe
{

	PDX12GPUTexture::PDX12GPUTexture(std::string TextureName, std::wstring FileName) : PGPUTexture(TextureName, FileName)
	{
		PResource = new PDX12GPUResource();
		PUploadHeap = new PDX12GPUResource();
	}

	PDX12GPUTexture::~PDX12GPUTexture()
	{
		ReleasePtr(PResource);
		ReleasePtr(PUploadHeap);
	}

}

#endif

