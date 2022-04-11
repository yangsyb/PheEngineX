#include "pch.h"
#if defined (RenderDeviceDX12)
#include "PDX12GPUTexture.h"
namespace Phe
{

	PDX12GPUTexture::PDX12GPUTexture(std::string TextureName, std::wstring FileName, P_TextureType TextureType) : PGPUTexture(TextureName, FileName, TextureType)
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

