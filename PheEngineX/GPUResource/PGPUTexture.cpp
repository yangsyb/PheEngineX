#include "pch.h"
#include "PGPUTexture.h"

namespace Phe
{
	PGPUTexture::PGPUTexture(std::string TextureName, P_TextureType TextureType /*= P_TextureType::P_Texture2D*/) : PTextureName(TextureName), PFileName(), PHandleOffset(0), PTextureType(TextureType)
	{

	}

	PGPUTexture::PGPUTexture(std::string TextureName, std::wstring FileName, P_TextureType TextureType /*= P_TextureType::P_Texture2D*/) : PTextureName(TextureName), PFileName(FileName), PHandleOffset(0), PTextureType(TextureType)
	{

	}

	PGPUTexture::~PGPUTexture()
	{

	}

}