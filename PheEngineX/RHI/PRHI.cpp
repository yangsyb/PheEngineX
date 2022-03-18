#include "pch.h"
#include "PRHI.h"
#include "PDX12RHI.h"

namespace Phe
{

	PRHI* PRHI::RHI = nullptr;

	PRHI::PRHI()
	{

	}

	PRHI::~PRHI()
	{

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
		delete RHI;
		RHI = nullptr;
	}

}