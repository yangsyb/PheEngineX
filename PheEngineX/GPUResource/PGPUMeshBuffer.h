#pragma once
#include "pch.h"
#include "PGPUResource.h"


namespace Phe
{
	class PStaticMesh;
	class PGPUMeshBuffer
	{
	public:
		PGPUMeshBuffer();
		virtual ~PGPUMeshBuffer();

		void SetMeshBuffer(std::string Name, PStaticMesh* StaticMeshData);
		UINT32 GetVertexByteStride() const { return PVertexByteStride; }
		UINT32 GetVertexCount() const { return PVertexCount;}
		UINT32 GetIndexCount() const { return PIndexCount; }
		UINT32 GetVertexBufferByteSize() const { return PVertexBufferByteSize; }
		UINT32 GetIndexBufferByteSize() const { return PIndexBufferByteSize; }
		UINT32 GetBufferSize() const { return PBufferSize; }
		
		std::vector<PVertex> GetVertexVector() { return PVertexVector; }
		std::vector<UINT16> GetIndexVector() { return PIndexVector; }
	protected:
		UINT32 PVertexByteStride;
		UINT32 PVertexBufferByteSize;
		UINT32 PIndexBufferByteSize;
		UINT32 PBufferSize;

		UINT32 PVertexCount;
		UINT32 PIndexCount;

		std::vector<PVertex> PVertexVector;
		std::vector<UINT16> PIndexVector;
	};
}