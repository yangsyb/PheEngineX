#include "pch.h"
#if defined (RenderDeviceDX12)
#include "PDX12GPUResource.h"

namespace Phe
{

	PDX12GPUResource::PDX12GPUResource() : PUsageState(D3D12_RESOURCE_STATE_COMMON)
	{
		PResource = nullptr;
	}

	PDX12GPUResource::PDX12GPUResource(D3D12_RESOURCE_STATES State) : PUsageState(State)
	{

	}

	PDX12GPUResource::~PDX12GPUResource()
	{

	}

	void PDX12GPUResource::CreateResource(ID3D12Device* Device, const D3D12_HEAP_PROPERTIES* pHeapProperties, D3D12_HEAP_FLAGS HeapFlags, const D3D12_RESOURCE_DESC* pDesc, D3D12_RESOURCE_STATES InitialResourceState, const D3D12_CLEAR_VALUE* pOptimizedClearValue /*= nullptr*/)
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
#endif