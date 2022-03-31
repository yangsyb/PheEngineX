#include "pch.h"
#include "PTexture.h"
#include "PMaterial.h"
#include "GPUResource/PGPUTexture.h"

namespace Phe
{

	PTexture::PTexture(std::string TextureName, std::wstring TexturePathName) : PTextureName(TextureName), PTexturePathName(TexturePathName), PLinkedGPUTexture(nullptr)
	{

	}

	PTexture::~PTexture()
	{

	}

	void PTexture::BindMaterial(PMaterial* Material)
	{
		PLinkedMaterial.insert({ Material->GetName(), Material });
	}

	void PTexture::BindGPUTexture(PGPUTexture* LinkedGPUTexture)
	{
		PLinkedGPUTexture = LinkedGPUTexture;
	}

}