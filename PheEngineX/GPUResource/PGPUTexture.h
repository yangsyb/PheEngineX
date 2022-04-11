#pragma once
#include "pch.h"
#include "PGPUResource.h"

namespace Phe
{
	enum class P_TextureType
	{
		P_Texture2D,
		P_TextureCube
	};

	class PGPUTexture : public PGPUResource
	{
	public:
		PGPUTexture(std::string TextureName, P_TextureType TextureType = P_TextureType::P_Texture2D);
		PGPUTexture(std::string TextureName, std::wstring FileName, P_TextureType TextureType = P_TextureType::P_Texture2D);
		virtual ~PGPUTexture();
		virtual bool operator==(const PGPUTexture* t) { return this->GetTextureName() == t->PTextureName; }

		virtual void SetHandleOffset(UINT32 Offset) { PHandleOffset = Offset; }
		virtual std::string GetTextureName() { return PTextureName; }
		virtual std::wstring GetFileName() { return PFileName; }
		virtual UINT32 GetHandleOffset() { return PHandleOffset; }
		virtual P_TextureType GetTextureType() { return PTextureType; }
	private:
		std::string PTextureName;
		std::wstring PFileName;
		UINT32 PHandleOffset;
		P_TextureType PTextureType;
	};
}