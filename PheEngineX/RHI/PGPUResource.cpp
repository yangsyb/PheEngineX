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

	void PGPUResource::CreateResource(ID3D12Device* Device, const D3D12_HEAP_PROPERTIES* pHeapProperties, D3D12_HEAP_FLAGS HeapFlags, const D3D12_RESOURCE_DESC* pDesc, D3D12_RESOURCE_STATES InitialResourceState, const D3D12_CLEAR_VALUE* pOptimizedClearValue /*= nullptr*/)
	{
		PResource = nullptr;
		PUsageState = InitialResourceState;
		Device->CreateCommittedResource(
			pHeapProperties,
			HeapFlags,
			pDesc,
			InitialResourceState,
			pOptimizedClearValue,
			IID_PPV_ARGS(&PResource));
	}

}