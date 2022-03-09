#pragma once
#include "pch.h"
#include "Transform.h"
namespace Phe
{
	class PStaticMesh
	{
	public:
		PStaticMesh(std::string Name, std::vector<float> Vertices, std::vector<UINT32> Indices, std::vector<float> Tangents);
		~PStaticMesh();

		std::string GetName() { return PMeshName; }
		std::vector<float> GetVertices() { return PVertices; }
		std::vector<UINT32> GetIndices() { return PIndices; }
		std::vector<float> GetTangents() { return PTangents; }

	protected:
		std::string PMeshName;
		std::vector<float> PVertices;
		std::vector<UINT32> PIndices;
		std::vector<float> PTangents;

	};

	class PRenderStaticMesh : public PStaticMesh
	{
	public:
		PRenderStaticMesh(std::string Name, PMeshDataStruct RMeshData);
		~PRenderStaticMesh();

		D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const { return PVertexBufferView; }
		D3D12_INDEX_BUFFER_VIEW IndexBufferView()const { return PIndexBufferView; }
		UINT VertexCount() const { return PVertexCount; }
		UINT IndexCount() const { return PIndexCount; }
		ComPtr<ID3D12Resource> CreateDefaultBuffer(ID3D12GraphicsCommandList* cmdList, const void* initData, UINT64 byteSize, Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer);
		// 		void BuildMesh();
		// 		Transform GetWorldCb() { return PTransform; }
	protected:
		ComPtr<ID3D12Resource> PVertexBufferGPU = nullptr;
		ComPtr<ID3D12Resource> PIndexBufferGPU = nullptr;
		ComPtr<ID3D12Resource> PUploadBuffer = nullptr;
		UINT PVertexByteStride = 0;
		ULONG PVertexBufferByteSize = 0;
		DXGI_FORMAT PIndexFormat = DXGI_FORMAT_R32_UINT;
		UINT PIndexBufferByteSize = 0;
		D3D12_VERTEX_BUFFER_VIEW PVertexBufferView;
		D3D12_INDEX_BUFFER_VIEW PIndexBufferView;

		UINT PVertexCount = 0;
		UINT PIndexCount;
	};
}