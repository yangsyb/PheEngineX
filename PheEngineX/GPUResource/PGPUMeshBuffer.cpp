#include "pch.h"
#include "PGPUMeshBuffer.h"

namespace Phe
{

	PGPUMeshBuffer::PGPUMeshBuffer() : PVertexByteStride(0), PVertexBufferByteSize(0), PIndexBufferByteSize(0), PBufferSize(0), PVertexCount(0), PIndexCount(0),
		PVertexVector(), PIndexVector()
	{

	}

	PGPUMeshBuffer::~PGPUMeshBuffer()
	{

	}

	void PGPUMeshBuffer::SetMeshBuffer(std::string Name, PMeshDataStruct RMeshData)
	{
		for (size_t index = 0; index < RMeshData.Vertices.size() / 3; index++)
		{
			PVertex Vertex;
			Vertex.Pos = { RMeshData.Vertices[index * 3], RMeshData.Vertices[index * 3 + 1] ,RMeshData.Vertices[index * 3 + 2] };
			Vertex.Normal = { RMeshData.Normal[index * 3], RMeshData.Normal[index * 3 + 1] ,RMeshData.Normal[index * 3 + 2],1 };
			Vertex.TextCoord = { RMeshData.UVs[index * 2], RMeshData.UVs[index * 2 + 1] };
			PVertexVector.push_back(Vertex);
		}
		PIndexVector = RMeshData.Indices;
		PVertexCount = UINT32(PVertexVector.size());
		PIndexCount = UINT32(PIndexVector.size());

		PVertexByteStride = sizeof(PVertex);
		PVertexBufferByteSize = PVertexByteStride * PVertexCount;
		PIndexBufferByteSize = sizeof(UINT16) * PIndexCount;
		PBufferSize = PVertexBufferByteSize + PIndexBufferByteSize;
	}

}