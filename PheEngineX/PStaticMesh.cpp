#include "pch.h"
#include "PStaticMesh.h"
#include "GraphicsContext.h"
#include "DxException.h"
namespace Phe
{
	// Raw StaticMesh Data2
	// Managed By AssetManager
	PStaticMesh::PStaticMesh(std::string Name, std::vector<float> Vertices, std::vector<UINT32> Indices, std::vector<float> Tangents, std::vector<float> UVs) : PMeshName(Name), PVertices(Vertices), PIndices(Indices), PTangents(Tangents), PUVs(UVs)
	{

	}

	PStaticMesh::~PStaticMesh()
	{

	}

	// Render StaticMesh Data
	// Managed By RenderThread Scene
	// Raw StaticMesh Data passed by MainThread, and converted to Default Buffer in Constructor 
	PRenderStaticMesh::PRenderStaticMesh(std::string Name, PMeshDataStruct RMeshData) : PStaticMesh(Name, RMeshData.Vertices, RMeshData.Indices, RMeshData.Normal, RMeshData.UVs)
	{
		//		GraphicContext::GetSingleton().ResetCommandList();
		std::vector<PVertex> VerticesVec;
		for (size_t index = 0; index < RMeshData.Vertices.size() / 3; index++)
		{
			PVertex Vertex;
			Vertex.Pos = { RMeshData.Vertices[index * 3], RMeshData.Vertices[index * 3 + 1] ,RMeshData.Vertices[index * 3 + 2] };
			Vertex.Normal = { RMeshData.Normal[index * 3], RMeshData.Normal[index * 3 + 1] ,RMeshData.Normal[index * 3 + 2],1 };
			Vertex.TextCoord = { RMeshData.UVs[index * 2], RMeshData.UVs[index * 2 + 1] };
			VerticesVec.push_back(Vertex);
		}

		PVertexCount = UINT(VerticesVec.size());
		PIndexCount = UINT(PIndices.size());
		PVertexByteStride = sizeof(PVertex);
		PVertexBufferByteSize = PVertexCount * sizeof(PVertex);
		PIndexBufferByteSize = PIndexCount * sizeof(UINT32);
		ULONGLONG bufferSize = ULONGLONG(PVertexBufferByteSize) + PIndexBufferByteSize;
		char* buffer = new char[bufferSize];
		std::unique_ptr<char> dataPtrGuard(buffer);
		UINT offset = 0;
		auto vertBufferCopy = [&](char* buffer, const char* ptr, UINT size, UINT& offset) -> void
		{
			if (ptr)
			{
				for (UINT i = 0; i < PVertexCount; ++i)
				{
					memcpy(buffer + size_t(i) * PVertexByteStride + offset, ptr + size_t(size) * i, size);
				}
				offset += size;
			}
		};
		vertBufferCopy(buffer, reinterpret_cast<const char*>(VerticesVec.data()), sizeof(PVertex), offset);

		char* indexBufferStart = buffer + PVertexBufferByteSize;
		memcpy(indexBufferStart, PIndices.data(), PIndexCount * sizeof(UINT32));

		PVertexBufferGPU = CreateDefaultBuffer(
			GraphicContext::GetSingleton().CommandList(), buffer, bufferSize, PUploadBuffer);

		PVertexBufferView.BufferLocation = PVertexBufferGPU->GetGPUVirtualAddress();
		PVertexBufferView.StrideInBytes = PVertexByteStride;
		PVertexBufferView.SizeInBytes = PVertexBufferByteSize;
		PIndexBufferView.BufferLocation = PVertexBufferGPU->GetGPUVirtualAddress() + PVertexBufferByteSize;
		PIndexBufferView.Format = PIndexFormat;
		PIndexBufferView.SizeInBytes = PIndexBufferByteSize;
		//		GraphicContext::GetSingleton().ExecuteCommandList();
	}

	PRenderStaticMesh::~PRenderStaticMesh()
	{
		PVertexBufferGPU = nullptr;
		PIndexBufferGPU = nullptr;
		PUploadBuffer = nullptr;
	}

	Microsoft::WRL::ComPtr<ID3D12Resource> PRenderStaticMesh::CreateDefaultBuffer(ID3D12GraphicsCommandList* cmdList, const void* initData, UINT64 byteSize, Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer)
	{
		ID3D12Device* device = GraphicContext::GetSingleton().Device().Get();
		ComPtr<ID3D12Resource> defaultBuffer;

		// Create the actual default buffer resource.
		auto HeapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		auto ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
		ThrowIfFailed(device->CreateCommittedResource(
			&HeapProperty,
			D3D12_HEAP_FLAG_NONE,
			&ResourceDesc,
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(defaultBuffer.GetAddressOf())));

		HeapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
		ThrowIfFailed(device->CreateCommittedResource(
			&HeapProperty,
			D3D12_HEAP_FLAG_NONE,
			&ResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(uploadBuffer.GetAddressOf())));

		D3D12_SUBRESOURCE_DATA subResourceData = {};
		subResourceData.pData = initData;
		subResourceData.RowPitch = byteSize;
		subResourceData.SlicePitch = subResourceData.RowPitch;

		auto Resourcebarrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
		cmdList->ResourceBarrier(1, &Resourcebarrier);
		UpdateSubresources<1>(cmdList, defaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, &subResourceData);
		Resourcebarrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
		cmdList->ResourceBarrier(1, &Resourcebarrier);

		return defaultBuffer;
	}

}