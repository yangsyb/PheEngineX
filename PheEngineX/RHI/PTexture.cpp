#include "pch.h"
#include "PTexture.h"
#include "GraphicsContext.h"
#include "DDSTextureLoader.h"

namespace Phe
{
	PTexture::PTexture(const std::string TName, const std::wstring TFileName) : TextureName(TName), FileName(TFileName)
	{

	}

	PTexture::~PTexture()
	{

	}

	PRenderTexture::PRenderTexture(const std::string TName, const std::wstring TFileName) : PGPUResource(), PTexture(TName, TFileName)
	{
		if (PResource) { PResource = nullptr; }
		if (UploadHeap) { UploadHeap = nullptr; }
		DirectX::CreateDDSTextureFromFile12(GraphicContext::GetSingleton().Device().Get(), GraphicContext::GetSingleton().CommandList(), FileName.c_str(), PResource, UploadHeap);
	}

	PRenderTexture::PRenderTexture(std::shared_ptr<PTexture> Tex) : PTexture(Tex->GetTextureName(), Tex->GetFileName())
	{
		if (PResource) { PResource = nullptr; }
		if (UploadHeap) { UploadHeap = nullptr; }
		DirectX::CreateDDSTextureFromFile12(GraphicContext::GetSingleton().Device().Get(), GraphicContext::GetSingleton().CommandList(), FileName.c_str(), PResource, UploadHeap);
	}

	PRenderTexture::~PRenderTexture()
	{

	}

	void PRenderTexture::CreateSRV(CD3DX12_CPU_DESCRIPTOR_HANDLE& Handle)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = PResource->GetDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = PResource->GetDesc().MipLevels;
		srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
		
		GraphicContext::GetSingleton().Device()->CreateShaderResourceView(PResource.Get(), &srvDesc, Handle);
	}


}