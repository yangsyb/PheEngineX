#pragma once
#include "pch.h"
#include "PGPUResource.h"

namespace Phe
{
	class PTexture
	{
	public:
		PTexture(const std::string TName, const std::wstring TFileName);
		virtual ~PTexture();

		virtual std::string GetTextureName() { return TextureName; }
		virtual std::wstring GetFileName() { return FileName; }
	protected:
		std::string TextureName;
		std::wstring FileName;

	};

	class PRenderTexture : public PGPUResource, PTexture
	{
	public:
		PRenderTexture(const std::string TName, const std::wstring TFileName);
		PRenderTexture(std::shared_ptr<PTexture> Tex);
		~PRenderTexture();
		bool operator==(const std::shared_ptr<PRenderTexture> t) { return this->GetTextureName() == t->GetTextureName(); }

		void CreateSRV(CD3DX12_CPU_DESCRIPTOR_HANDLE& Handle);

	protected:
		ComPtr<ID3D12Resource> UploadHeap = nullptr;
	};
}