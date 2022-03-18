#if defined (RenderDeviceDX12)
#pragma once
#include "pch.h"
#include "GPUResource/PGPUCommonBuffer.h"
#include "PDX12GPUResource.h"

namespace Phe
{
	class PDX12GPUCommonBuffer : public PGPUCommonBuffer
	{
	public:
		PDX12GPUCommonBuffer(UINT32 StructByteSize, UINT32 ElementsNum, UINT32 Flag);
		virtual ~PDX12GPUCommonBuffer();

		virtual void AllocateData(int DataIndex, void* data);

		PDX12GPUResource* GetResource()
		{
			return PBufferResource;
		}

		ComPtr<ID3D12Resource> GetBufferResource(){ return PBuffer; }

	public:
		PDX12GPUResource* PBufferResource;
		ComPtr<ID3D12Resource> PBuffer;
		BYTE* PMappedData = nullptr;
		UINT32 SizeofStructByteSize;
		CD3DX12_CPU_DESCRIPTOR_HANDLE BufferHandle;
	};
}
#endif