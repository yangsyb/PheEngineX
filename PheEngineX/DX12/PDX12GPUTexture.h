#if defined (RenderDeviceDX12)
#pragma once
#include "pch.h"
#include "GPUResource/PGPUTexture.h"
#include "PDX12GPUResource.h"

namespace Phe
{
	class PDX12GPUTexture : public PGPUTexture
	{
	public:
		PDX12GPUTexture(std::string TextureName, std::wstring FileName = L"");
		virtual ~PDX12GPUTexture();

 		void SetPResource(ComPtr<ID3D12Resource> Resource) { PResource->SetResource(Resource); }
 		void SetPUploadHeap(ComPtr<ID3D12Resource> UploadHeap) { PUploadHeap->SetResource(UploadHeap); }
		PDX12GPUResource* GetPResource() { return PResource; }
		PDX12GPUResource* GetUploadHeap() { return PUploadHeap; }
	private:
		PDX12GPUResource* PResource;
		PDX12GPUResource* PUploadHeap;
	};
}


#endif