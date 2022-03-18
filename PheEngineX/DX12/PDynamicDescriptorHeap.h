#if defined (RenderDeviceDX12)
#pragma once
#include "pch.h"
#include "PDescriptorHeap.h"

namespace Phe
{
	class PDynamicDescriptorHeap
	{
	public:
		PDynamicDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE HeapType);
		~PDynamicDescriptorHeap();

	private:
		ID3D12DescriptorHeap* RequestDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE HeapType);

		const UINT32 PNumDescriptorPerHeap = 1024;
		std::vector<ComPtr<ID3D12DescriptorHeap>> PDynamicDescriptorHeapPool[2];
		std::queue<ID3D12DescriptorHeap*> PAvaliableDynamicDescriptorHeap[2];

		ID3D12DescriptorHeap* PCurrentHeapPtr;
		const D3D12_DESCRIPTOR_HEAP_TYPE PDescriptorType;
		UINT32 PDescriptorSize;
		UINT32 PCurrentOffset;
		PDescriptorHandle PFirstDescriptor;

		struct DescriptorTableCache
		{
			DescriptorTableCache() : AssignedHandlesBitMap(0) {}
			UINT32 AssignedHandlesBitMap;
			D3D12_CPU_DESCRIPTOR_HANDLE* TableStart;
			UINT32 TableSize;
		};

		struct DescriptorHandleCache
		{
			DescriptorHandleCache()
			{
				ClearCache();
			}

			void ClearCache()
			{
				PRootDescriptorTablesBitMap = 0;
				PMaxCachedDescriptors = 0;
			}

			UINT32 PRootDescriptorTablesBitMap;
			UINT32 PStaleRootParamsBitMap;
			UINT32 PMaxCachedDescriptors;

			static const UINT32 PMaxNumDescriptors = 256;
			static const UINT32 PMaxNumDescriptorTables = 16;

			uint32_t ComputeStagedSize();
			void CopyAndBindStaleTables(D3D12_DESCRIPTOR_HEAP_TYPE Type, uint32_t DescriptorSize, PDescriptorHandle DestHandleStart, ID3D12GraphicsCommandList* CmdList,
				void (STDMETHODCALLTYPE ID3D12GraphicsCommandList::* SetFunc)(UINT, D3D12_GPU_DESCRIPTOR_HANDLE));

			DescriptorTableCache PRootDescriptorTable[PMaxNumDescriptorTables];
			D3D12_CPU_DESCRIPTOR_HANDLE PHandleCache[PMaxNumDescriptors];

			void UnbindAllValid();
			void StageDescriptorHandles(UINT RootIndex, UINT Offset, UINT NumHandles, const D3D12_CPU_DESCRIPTOR_HANDLE Handles[]);
		};


	};
}

#endif;