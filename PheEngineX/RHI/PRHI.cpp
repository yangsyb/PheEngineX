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
#if defined (PlatformWin32)
		RHI = new PDX12RHI();

#elif defined (PlatformIOS)

#elif defined (PlatformAndroid)

#endif
	}

	void PRHI::DestroyRHI()
	{
		delete RHI;
		RHI = nullptr;
	}

}