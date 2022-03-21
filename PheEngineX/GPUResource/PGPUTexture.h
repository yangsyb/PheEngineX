#pragma once
#include "pch.h"
#include "PGPUResource.h"

namespace Phe
{
	class PGPUTexture : public PGPUResource
	{
	public:
		PGPUTexture(std::string TextureName, std::wstring FileName);
		virtual ~PGPUTexture();
		virtual bool operator==(const PGPUTexture* t) { return this->GetTextureName() == t->PTextureName; }

		virtual void SetHandleOffset(UINT32 Offset) { PHandleOffset = Offset; }
		virtual std::string GetTextureName() { return PTextureName; }
		virtual std::wstring GetFileName() { return PFileName; }
		virtual UINT32 GetHandleOffset() { return PHandleOffset; }
	private:
		std::string PTextureName;
		std::wstring PFileName;
		UINT32 PHandleOffset;
	};
}