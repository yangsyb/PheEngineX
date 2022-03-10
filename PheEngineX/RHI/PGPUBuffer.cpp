#include "pch.h"
#include "PGPUBuffer.h"
#include "GraphicsContext.h"
#include "DxException.h"

namespace Phe
{

	PGPUBuffer::PGPUBuffer(UINT32 NumElements, UINT32 ElementSize, D3D12_RESOURCE_STATES InitialState, D3D12_HEAP_TYPE HeapType)
	{
		PElementCount = NumElements;
		PElementSize = ElementSize;
		PBufferSize = NumElements * ElementSize;
		PUsageState = InitialState;
		PHeapType = HeapType;
	}



	void PGPUBuffer::CreateBufferResource(const void* initialData)
	{
		D3D12_RESOURCE_DESC ResourceDesc = DescribeBuffer();
		D3D12_HEAP_PROPERTIES HeapProps;
		HeapProps.Type = PHeapType;
		HeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		HeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		HeapProps.CreationNodeMask = 1;
		HeapProps.VisibleNodeMask = 1;

		ID3D12Device* d3dDevice = GraphicContext::GetSingleton().Device().Get();
		ThrowIfFailed(d3dDevice->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE,
			&ResourceDesc, PUsageState, nullptr, IID_PPV_ARGS(&PResource)));

		PGpuVirtualAddress = PResource->GetGPUVirtualAddress();
	}

	void PGPUBuffer::CreateBufferResource(const PGpuUploadBuffer& srcData, UINT32 srcOffset)
	{

	}

	D3D12_RESOURCE_DESC PGPUBuffer::DescribeBuffer()
	{
		D3D12_RESOURCE_DESC Desc = {};
		Desc.Alignment = 0;
		Desc.DepthOrArraySize = 1;
		Desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		Desc.Flags = D3D12_RESOURCE_FLAG_NONE;
		Desc.Format = DXGI_FORMAT_UNKNOWN;
		Desc.Height = 1;
		Desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		Desc.MipLevels = 1;
		Desc.SampleDesc.Count = 1;
		Desc.SampleDesc.Quality = 0;
		Desc.Width = (UINT64)PBufferSize;
		return Desc;
	}

}