#include "pch.h"

#if defined (RenderDeviceDX12)
#include "PDX12GPUCommonBuffer.h"

namespace Phe
{

	PDX12GPUCommonBuffer::PDX12GPUCommonBuffer(UINT32 StructByteSize, UINT32 ElementsNum, UINT32 Flag) : PGPUCommonBuffer(StructByteSize, ElementsNum, Flag), SizeofStructByteSize(StructByteSize), PBufferResource(nullptr)
	{
		PStructureSizeInBytes = StructByteSize;
		if (Flag == 0)
		{
			PStructureSizeInBytes = (StructByteSize + 255) & ~255;
		}

	}

	PDX12GPUCommonBuffer::~PDX12GPUCommonBuffer()
	{
		if(PBufferResource)
		{
			if (PBufferResource->GetResource())
			{
				PBufferResource->GetResource()->Unmap(0, nullptr);
			}
		}
		PMappedData = nullptr;
		ReleasePtr(PBufferResource);
	}

	void PDX12GPUCommonBuffer::AllocateData(int DataIndex, void* data)
	{
		CurrentData = data;
		memcpy(&PMappedData[DataIndex * PStructureSizeInBytes], data, SizeofStructByteSize);
	}

}

#endif