#include "pch.h"
#include "PGPURenderTarget.h"
#include "RHI/PRHI.h"

namespace Phe
{

	PGPURenderTarget::PGPURenderTarget(std::string RenderTargetName, UINT32 Width, UINT32 Height) : PGPUResource(), PRenderTargetName(RenderTargetName),
		PWidth(Width), PHeight(Height), PRenderTargetDepthStencilBuffer(nullptr)
	{

	}

	PGPURenderTarget::~PGPURenderTarget()
	{
		for(int index = 0; index< PRenderTargetColorBuffer.size();index++)
		{
			PRHI::Get()->DestroyRTBuffer(PRenderTargetColorBuffer[index]);
			ReleasePtr(PRenderTargetColorBuffer[index]);
		}
		PRHI::Get()->DestroyRTBuffer(PRenderTargetDepthStencilBuffer);
		ReleasePtr(PRenderTargetDepthStencilBuffer);
	}

	RTBuffer::~RTBuffer()
	{
		if(PRTTexture)
		{
			PRHI::Get()->DestroyTexture(PRTTexture, true);
		}
	}

}