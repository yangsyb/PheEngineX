#include "pch.h"
#include "PGPUTexture.h"

namespace Phe
{

	PGPUTexture::PGPUTexture(std::string TextureName, std::wstring FileName) : PTextureName(TextureName), PFileName(FileName), PHandleOffset(0)
	{

	}

	PGPUTexture::~PGPUTexture()
	{

	}

}