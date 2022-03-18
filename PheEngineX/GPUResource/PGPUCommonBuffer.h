#pragma once
#include "pch.h"

namespace Phe
{
	class PGPUCommonBuffer
	{
	public:
		PGPUCommonBuffer(UINT32 StructByteSize, UINT32 ElementsNum, UINT32 Flag);
		virtual ~PGPUCommonBuffer();

		virtual void AllocateData(int DataIndex, void* data) = 0;

		virtual UINT32 GetTotalBufferSize() const
		{
			return PStructureSizeInBytes * PElements;
		}
		virtual UINT32 GetStructureSizeInBytes() const
		{
			return PStructureSizeInBytes;
		}
		virtual UINT32 GetElements() const
		{
			return PElements;
		}
		virtual UINT32 GetFlag() const
		{
			return PFlag;
		}
		virtual void SetHandleOffset(UINT32 Offset) { PHandleOffset = Offset; }
		virtual UINT32 GetHandleOffset() { return PHandleOffset; }

	protected:
		UINT32 PStructureSizeInBytes;
		UINT32 PElements;
		UINT32 PFlag;

		UINT32 PHandleOffset;

	};
}