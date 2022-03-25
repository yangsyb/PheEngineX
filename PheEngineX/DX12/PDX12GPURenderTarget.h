#if defined (RenderDeviceDX12)
#pragma once
#include "pch.h"
#include "GPUResource/PGPURenderTarget.h"
#include "DX12/PDX12GPUResource.h"

namespace Phe
{
	struct DX12RTBuffer : public RTBuffer
	{
		DX12RTBuffer(RTBufferType Type, UINT32 Width, UINT32 Height) : RTBuffer(Type, Width, Height), PResourceDescriptor(), POptClear(), InitialState(D3D12_RESOURCE_STATE_COMMON)
		{
			PResource = new PDX12GPUResource();
			switch (Type)
			{
			case RTBufferType::RTColorBuffer:
				D3D12_RESOURCE_DESC ColorDesc;
				memset(&ColorDesc, 0, sizeof(D3D12_RESOURCE_DESC));
				ColorDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
				ColorDesc.Alignment = 0;
				ColorDesc.Width = PWidth;
				ColorDesc.Height = PHeight;
				ColorDesc.DepthOrArraySize = 1;
				ColorDesc.MipLevels = 1;
				ColorDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				ColorDesc.SampleDesc.Count = 1;
				ColorDesc.SampleDesc.Quality = 0;
				ColorDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
				ColorDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

				POptClear.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				PResourceDescriptor = ColorDesc;
				InitialState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON;
				break;

			case RTBufferType::RTDepthStencilBuffer:
				D3D12_RESOURCE_DESC DepthStencilDesc;
				ZeroMemory(&DepthStencilDesc, sizeof(D3D12_RESOURCE_DESC));
				DepthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
				DepthStencilDesc.Alignment = 0;
				DepthStencilDesc.Width = PWidth;
				DepthStencilDesc.Height = PHeight;
				DepthStencilDesc.DepthOrArraySize = 1;
				DepthStencilDesc.MipLevels = 1;
				DepthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
				//DXGI_FORMAT_D24_UNORM_S8_UINT
				//DXGI_FORMAT_R24G8_TYPELESS
				//DXGI_FORMAT_D24_UNORM_S8_UINT
				DepthStencilDesc.SampleDesc.Count = 1;
				DepthStencilDesc.SampleDesc.Quality = 0;
				DepthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
				DepthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

				POptClear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				POptClear.DepthStencil.Depth = 1.0f;
				POptClear.DepthStencil.Stencil = 0;

				PResourceDescriptor = DepthStencilDesc;
				InitialState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ;
				break;
			}
		}
		~DX12RTBuffer()
		{
			ReleasePtr(PResource);
		}
		PDX12GPUResource* PResource;
		D3D12_RESOURCE_DESC PResourceDescriptor;
		D3D12_RESOURCE_STATES InitialState;
		D3D12_CLEAR_VALUE POptClear;
	};

	class PDX12GPURenderTarget : public PGPURenderTarget
	{
	public:
		PDX12GPURenderTarget(std::string RenderTargetName, UINT32 Width, UINT32 Height);
		virtual ~PDX12GPURenderTarget();

		virtual void AddColorBuffer() override;
		virtual void AddDepthStencilBuffer() override;

	protected:
		D3D12_VIEWPORT PViewport;
		D3D12_RECT PScissorRect;

		friend class PDX12RHI;
	};
}

#endif