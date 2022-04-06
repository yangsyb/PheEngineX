#include "pch.h"
#include "PGPUMeshBuffer.h"
#include "Engine/Editor/PStaticMesh.h"

namespace Phe
{

	PGPUMeshBuffer::PGPUMeshBuffer() : PVertexByteStride(0), PVertexBufferByteSize(0), PIndexBufferByteSize(0), PBufferSize(0), PVertexCount(0), PIndexCount(0),
		PVertexVector(), PIndexVector()
	{

	}

	PGPUMeshBuffer::~PGPUMeshBuffer()
	{

	}

	void PGPUMeshBuffer::SetMeshBuffer(std::string Name, PStaticMesh* StaticMeshData)
	{
		auto Vertices = StaticMeshData->GetVertices();
		auto Tangents = StaticMeshData->GetTangents();
		auto Normals = StaticMeshData->GetNormals();
		auto UVs = StaticMeshData->GetUVs();
		for (size_t index = 0; index < Vertices.size() / 3; index++)
		{
			PVertex Vertex;
			Vertex.Pos = { Vertices[index * 3], Vertices[index * 3 + 1] ,Vertices[index * 3 + 2] };
			Vertex.Tangent = { Tangents[index * 3], Tangents[index * 3 + 1], Tangents[index * 3 + 2], 0 };
			Vertex.Normal = { Normals[index * 3], Normals[index * 3 + 1] ,Normals[index * 3 + 2],1 };
			Vertex.TextCoord = { UVs[index * 2], UVs[index * 2 + 1] };
			PVertexVector.push_back(Vertex);
		}
		PIndexVector = StaticMeshData->GetIndices();
		PVertexCount = UINT32(PVertexVector.size());
		PIndexCount = UINT32(PIndexVector.size());

		PVertexByteStride = sizeof(PVertex);
		PVertexBufferByteSize = PVertexByteStride * PVertexCount;
		PIndexBufferByteSize = sizeof(UINT16) * PIndexCount;
		PBufferSize = PVertexBufferByteSize + PIndexBufferByteSize;
	}

}