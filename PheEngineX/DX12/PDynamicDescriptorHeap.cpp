#include "pch.h"
#include "PDynamicDescriptorHeap.h"

//TODO
namespace Phe
{

	PDynamicDescriptorHeap::PDynamicDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE HeapType) : PDescriptorType(HeapType), PCurrentHeapPtr(nullptr)
	{

	}

	PDynamicDescriptorHeap::~PDynamicDescriptorHeap()
	{

	}

	void PDynamicDescriptorHeap::DescriptorHandleCache::StageDescriptorHandles(UINT RootIndex, UINT Offset, UINT NumHandles, const D3D12_CPU_DESCRIPTOR_HANDLE Handles[])
	{
		DescriptorTableCache& TableCache = PRootDescriptorTable[RootIndex];
		D3D12_CPU_DESCRIPTOR_HANDLE* CopyDest = TableCache.TableStart + Offset;
		for (UINT i = 0; i < NumHandles; ++i)
			CopyDest[i] = Handles[i];
		TableCache.AssignedHandlesBitMap |= ((1 << NumHandles) - 1) << Offset;
		PStaleRootParamsBitMap |= (1 << RootIndex);
	}

}