#include "pch.h"
#include "PGPUCommonBuffer.h"


namespace Phe
{

	PGPUCommonBuffer::PGPUCommonBuffer(UINT32 StructByteSize, UINT32 ElementsNum, UINT32 Flag) : PStructureSizeInBytes(StructByteSize), PElements(ElementsNum), PFlag(Flag), PHandleOffset(-1), CurrentData(nullptr)
	{

	}

	PGPUCommonBuffer::~PGPUCommonBuffer()
	{
//		ReleasePtr(CurrentData);
		ReleaseCurrentData();
	}

}