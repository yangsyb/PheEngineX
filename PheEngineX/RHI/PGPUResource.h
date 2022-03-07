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

		void CreateResource(
			ID3D12Device* Device,
			const D3D12_HEAP_PROPERTIES* pHeapProperties,
			D3D12_HEAP_FLAGS HeapFlags,
			const D3D12_RESOURCE_DESC* pDesc,
			D3D12_RESOURCE_STATES InitialResourceState,
			const D3D12_CLEAR_VALUE* pOptimizedClearValue = nullptr);

	public:
		ComPtr<ID3D12Resource> GetResource()
		{
			return PResource;
		}

		D3D12_RESOURCE_STATES GetCurrentState()
		{
			return PUsageState;
		}

	private:
		D3D12_RESOURCE_STATES PUsageState;
		ComPtr<ID3D12Resource> PResource;
	};
}