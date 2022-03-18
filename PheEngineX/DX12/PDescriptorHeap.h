#if defined (RenderDeviceDX12)
#pragma once
#include "pch.h"

namespace Phe
{

	class PDescriptorHeap
	{
	public:
		PDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE HeapType, ComPtr<ID3D12Device> Device);
		~PDescriptorHeap();
		std::pair<CD3DX12_CPU_DESCRIPTOR_HANDLE, UINT32> Allocate(UINT32 Count);
		ID3D12DescriptorHeap* GetCurrentHeap() { return PCurrentHeap; }
		void ClearHeap();
	private:
		static const UINT32 NumDescriptorPerHeap = 2048;
		std::vector<ComPtr<ID3D12DescriptorHeap>> PDescriptorHeapPool;
		ID3D12DescriptorHeap* RequestNewHeap(D3D12_DESCRIPTOR_HEAP_TYPE Type);

		ComPtr<ID3D12Device> PDevice;
		D3D12_DESCRIPTOR_HEAP_TYPE PHeapType;
		ID3D12DescriptorHeap* PCurrentHeap;
		CD3DX12_CPU_DESCRIPTOR_HANDLE PCurrentHandle;
		UINT32 PDescriptorSize;
		UINT32 PRemainingFreeHandles;
		UINT32 CurrentOffset;
	};

	class PDescriptorHandle
	{
	public:
		PDescriptorHandle();
		PDescriptorHandle(CD3DX12_CPU_DESCRIPTOR_HANDLE CpuHandle);
		PDescriptorHandle(CD3DX12_CPU_DESCRIPTOR_HANDLE CpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE GpuHandle);
		PDescriptorHandle operator+ (INT OffsetScaledByDescriptorSize) const
		{
			PDescriptorHandle ret = *this;
			ret += OffsetScaledByDescriptorSize;
			return ret;
		}
		void operator += (INT OffsetScaledByDescriptorSize)
		{
			if (PCpuHandle.ptr != D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
				PCpuHandle.ptr += OffsetScaledByDescriptorSize;
			if (PGpuHandle.ptr != D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
				PGpuHandle.ptr += OffsetScaledByDescriptorSize;
		}
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() const { return PCpuHandle; }
		CD3DX12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() const { return PGpuHandle; }
		bool IsShaderVisible() const { return PGpuHandle.ptr != D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN; }

	private:
		CD3DX12_CPU_DESCRIPTOR_HANDLE PCpuHandle;
		CD3DX12_GPU_DESCRIPTOR_HANDLE PGpuHandle;
	};
}
#endif