#include "pch.h"
#if defined (RenderDeviceDX12)
#include "PDescriptorHeap.h"

namespace Phe
{

	PDescriptorHeap::PDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE HeapType, ComPtr<ID3D12Device> Device) : PHeapType(HeapType), PCurrentHeap(nullptr), PCurrentHandle(D3D12_CPU_DESCRIPTOR_HANDLE()),
		PDescriptorHeapPool(), PDescriptorSize(0), PRemainingFreeHandles(0), PDevice(Device)
	{
		Allocate(0);
	}

	PDescriptorHeap::~PDescriptorHeap()
	{
		PDescriptorHeapPool.clear();
	}

	ID3D12DescriptorHeap* PDescriptorHeap::RequestNewHeap(D3D12_DESCRIPTOR_HEAP_TYPE Type)
	{
		D3D12_DESCRIPTOR_HEAP_DESC Desc;
		Desc.Type = Type;
		Desc.NumDescriptors = NumDescriptorPerHeap;
		Desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		Desc.NodeMask = 1;

		ComPtr<ID3D12DescriptorHeap> PHeap;
		PDevice->CreateDescriptorHeap(&Desc, IID_PPV_ARGS(&PHeap));
		PDescriptorHeapPool.emplace_back(PHeap);
		return PHeap.Get();
	}

	std::pair<CD3DX12_CPU_DESCRIPTOR_HANDLE, UINT32> PDescriptorHeap::Allocate(UINT32 HandleCount)
	{
		if (PFreeSizeMap.size() > 0)
		{
			for (auto MapIt : PFreeSizeMap)
			{
				if (MapIt.second == HandleCount)
				{
					auto RetHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(PCurrentHeap->GetCPUDescriptorHandleForHeapStart());
					RetHandle.ptr += MapIt.first * PDescriptorSize;
					PFreeSizeMap.erase(MapIt.first);
					return { RetHandle, MapIt.first };
				}
			}
		}

		if (PCurrentHeap == nullptr || PRemainingFreeHandles < HandleCount)
		{
			PCurrentHeap = RequestNewHeap(PHeapType);
			PCurrentHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(PCurrentHeap->GetCPUDescriptorHandleForHeapStart());
			PRemainingFreeHandles = NumDescriptorPerHeap;

			if (PDescriptorSize == 0)
			{
				PDescriptorSize = PDevice->GetDescriptorHandleIncrementSize(PHeapType);
			}
		}

		CD3DX12_CPU_DESCRIPTOR_HANDLE ret = PCurrentHandle;
		UINT32 retNum = CurrentOffset;
		PCurrentHandle.ptr += HandleCount * PDescriptorSize;
		PRemainingFreeHandles -= HandleCount;
		CurrentOffset += HandleCount;
		return { ret, retNum };
	}

	void PDescriptorHeap::Deallocate(UINT32 Offset, UINT32 Count)
	{
		PFreeSizeMap.insert({ Offset, Count });
	}

	void PDescriptorHeap::ClearHeap()
	{
		PDescriptorHeapPool.clear();
		PCurrentHeap = nullptr;
		Allocate(0);
	}

	PDescriptorHandle::PDescriptorHandle()
	{
		PCpuHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		PGpuHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
	}

	PDescriptorHandle::PDescriptorHandle(CD3DX12_CPU_DESCRIPTOR_HANDLE CpuHandle) : PCpuHandle(CpuHandle)
	{
		PGpuHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
	}

	PDescriptorHandle::PDescriptorHandle(CD3DX12_CPU_DESCRIPTOR_HANDLE CpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE GpuHandle) : PCpuHandle(CpuHandle), PGpuHandle(GpuHandle)
	{

	}

}
#endif