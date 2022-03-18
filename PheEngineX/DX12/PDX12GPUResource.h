#if defined (RenderDeviceDX12)
#pragma once
#include "pch.h"

namespace Phe
{
	class PDX12GPUResource
	{
	public:
		PDX12GPUResource();
		PDX12GPUResource(D3D12_RESOURCE_STATES State);
		~PDX12GPUResource();

	public:
		ComPtr<ID3D12Resource> GetResource()
		{
			return PResource;
		}

		D3D12_RESOURCE_STATES GetCurrentState()
		{
			return PUsageState;
		}

		void SetCurrentState(D3D12_RESOURCE_STATES State)
		{
			PUsageState = State;
		}

		void CreateResource(
			ID3D12Device* Device,
			const D3D12_HEAP_PROPERTIES* pHeapProperties,
			D3D12_HEAP_FLAGS HeapFlags,
			const D3D12_RESOURCE_DESC* pDesc,
			D3D12_RESOURCE_STATES InitialResourceState,
			const D3D12_CLEAR_VALUE* pOptimizedClearValue = nullptr);

		void SetResource(ComPtr<ID3D12Resource> InResource) { PResource = InResource; }
		ComPtr<ID3D12Resource> GetPResouce() {return PResource;}

	protected:
		D3D12_RESOURCE_STATES PUsageState;
		ComPtr<ID3D12Resource> PResource;
	};
}
#endif