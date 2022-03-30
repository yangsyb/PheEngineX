#pragma once
#include "pch.h"

namespace Phe
{
	class PTexture
	{
	public:
		PTexture(std::string TextureName, std::wstring TexturePathName);
		~PTexture();

		std::string GetTextureName() { return PTextureName; }
		std::wstring GetTextureFileName() { return PTexturePathName; }
	private:
		std::string PTextureName;
		std::wstring PTexturePathName;
	};


}