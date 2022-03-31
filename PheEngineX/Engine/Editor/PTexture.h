#pragma once
#include "pch.h"

namespace Phe
{
	class PGPUTexture;
	class PMaterial;
	class PTexture
	{
	public:
		PTexture(std::string TextureName, std::wstring TexturePathName);
		~PTexture();

		std::string GetTextureName() { return PTextureName; }
		std::wstring GetTextureFileName() { return PTexturePathName; }

		void BindMaterial(PMaterial* Material);
		void BindGPUTexture(PGPUTexture* LinkedGPUTexture);
		PGPUTexture* GetGPUTexture() { return PLinkedGPUTexture; }
	private:
		std::string PTextureName;
		std::wstring PTexturePathName;

		std::unordered_multimap<std::string, PMaterial*> PLinkedMaterial;
		PGPUTexture* PLinkedGPUTexture;
	};


}