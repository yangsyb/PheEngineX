#pragma once
#include "pch.h"

namespace Phe
{
	class PGPUResource
	{
	public:
		PGPUResource();
		PGPUResource(D3D12_RESOURCE_STATES State);
		~PGPUResource();

	public:
		ComPtr<ID3D12Resource> GetResource()
		{
			return PResource;
		}

		D3D12_RESOURCE_STATES GetCurrentState()
		{
			return PUsageState;
		}

	protected:
		D3D12_RESOURCE_STATES PUsageState;
		ComPtr<ID3D12Resource> PResource;
	};
}