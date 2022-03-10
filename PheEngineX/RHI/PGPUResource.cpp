#include "pch.h"
#include "PGPUResource.h"

namespace Phe
{

	PGPUResource::PGPUResource() : PUsageState(D3D12_RESOURCE_STATE_COMMON)
	{

	}

	PGPUResource::PGPUResource(D3D12_RESOURCE_STATES State) : PUsageState(State)
	{

	}

	PGPUResource::~PGPUResource()
	{
		PResource = nullptr;
	}

}