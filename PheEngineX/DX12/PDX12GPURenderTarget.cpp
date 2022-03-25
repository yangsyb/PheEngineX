#include "pch.h"
#if defined (RenderDeviceDX12)
#include "PDX12GPURenderTarget.h"
#include "RHI/PRHI.h"
namespace Phe
{

	PDX12GPURenderTarget::PDX12GPURenderTarget(std::string RenderTargetName, UINT32 Width, UINT32 Height) : PGPURenderTarget(RenderTargetName, Width, Height), PViewport(D3D12_VIEWPORT()), PScissorRect(D3D12_RECT())
	{

	}

	PDX12GPURenderTarget::~PDX12GPURenderTarget()
	{

	}

	void PDX12GPURenderTarget::AddColorBuffer()
	{
		for(size_t index = 0; index < 3; index++)
		{
			auto NewColorBuffer = PRHI::Get()->CreateRTBuffer(RTBufferType::RTColorBuffer, PWidth, PHeight);
			PRenderTargetColorBuffer.push_back(NewColorBuffer);
		}
	}

	void PDX12GPURenderTarget::AddDepthStencilBuffer()
	{
		PRenderTargetDepthStencilBuffer = PRHI::Get()->CreateRTBuffer(RTBufferType::RTDepthStencilBuffer, PWidth, PHeight);
	}

}


#endif