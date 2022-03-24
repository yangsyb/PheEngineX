#include "pch.h"
#include "PRHI.h"
#include "PDX12RHI.h"
namespace Phe
{

	PRHI* PRHI::RHI = nullptr;
	PGPURenderTarget* PRHI::ScreenRT = nullptr;
	PRHI::PRHI()
	{

	}

	PRHI::~PRHI()
	{
 		for(auto it : TextureRefPool)
 		{
 			delete it.first;
 		}
		TextureRefPool.clear();
	}

	PRHI* PRHI::Get()
	{
		return RHI;
	}

	void PRHI::CreateRHI()
	{
#if defined (RenderDeviceDX12)
		RHI = new PDX12RHI();

#elif defined (RenderDeviceVulkan)

#elif defined (RenderDeviceOpenGL)

#endif
	}

	void PRHI::DestroyRHI()
	{
		ReleasePtr(ScreenRT);
		ReleasePtr(RHI);
	}

}