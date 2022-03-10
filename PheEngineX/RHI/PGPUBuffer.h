#pragma once
#include "PGPUResource.h"

namespace Phe
{
	class PGpuUploadBuffer;
	class PGPUBuffer : public PGPUResource
	{
	public:
		PGPUBuffer(UINT32 NumElements, UINT32 ElementSize, D3D12_RESOURCE_STATES InitialState, D3D12_HEAP_TYPE HeapType);


		virtual D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress() const { return PGpuVirtualAddress; }
		UINT64 GetBufferSize() const { return PBufferSize; }
		UINT32 GetElementCount() const { return PElementCount; }
		UINT32 GetElementSize() const { return PElementSize; }
	protected:
		void CreateBufferResource(const void* initialData);
		void CreateBufferResource(const PGpuUploadBuffer& srcData, UINT32 srcOffset);

		D3D12_RESOURCE_DESC DescribeBuffer();

		D3D12_GPU_VIRTUAL_ADDRESS PGpuVirtualAddress = D3D12_GPU_VIRTUAL_ADDRESS_NULL;
		D3D12_HEAP_TYPE PHeapType;

		UINT64 PBufferSize;
		UINT32 PElementCount;
		UINT32 PElementSize;
	};

	class PGPUDefaultBuffer : public PGPUBuffer
	{
	public:

		PGPUDefaultBuffer(UINT32 NumElements, UINT32 ElementSize, const void* InitialData) :
			PGPUBuffer(NumElements, ElementSize, D3D12_RESOURCE_STATE_COMMON, D3D12_HEAP_TYPE_DEFAULT)
		{
			CreateBufferResource(InitialData);
		}

		PGPUDefaultBuffer(UINT32 NumElements, UINT32 ElementSize, const PGpuUploadBuffer& srcData, UINT32 srcOffset) :
			PGPUBuffer(NumElements, ElementSize, D3D12_RESOURCE_STATE_COMMON, D3D12_HEAP_TYPE_DEFAULT)
		{
			CreateBufferResource(srcData, srcOffset);
		}
	};

	class PGpuUploadBuffer : public PGPUBuffer
	{
	public:
		PGpuUploadBuffer(UINT32 NumElements, UINT32 ElementSize) :
			PGPUBuffer(NumElements, ElementSize, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_HEAP_TYPE_UPLOAD)
		{
			CreateBufferResource(nullptr);
		}

		void* Map(void)
		{
			void* Memory = nullptr;
			auto range = CD3DX12_RANGE(0, PBufferSize);
			PResource->Map(0, &range, &Memory);
			return Memory;
		}

		void Unmap(size_t begin = 0, size_t end = -1)
		{
			auto range = CD3DX12_RANGE(begin, std::min(end, PBufferSize));
			PResource->Unmap(0, &range);
		}
	};

}