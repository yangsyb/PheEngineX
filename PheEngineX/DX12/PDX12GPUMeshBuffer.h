#if defined (RenderDeviceDX12)
#pragma once
#include "pch.h"
#include "GPUResource/PGPUMeshBuffer.h"
#include "DX12/PDX12GPUResource.h"

namespace Phe
{
	class PDX12GPUMeshBuffer : public PGPUMeshBuffer
	{
	public:
		PDX12GPUMeshBuffer();
		~PDX12GPUMeshBuffer();

//		PDX12GPUResource VertexBuffer;
//		PDX12GPUResource IndexBuffer;
		ComPtr<ID3D12Resource> Buffer;
		ComPtr<ID3D12Resource> BufferUpload;
		D3D12_VERTEX_BUFFER_VIEW VertexBufferView;
		D3D12_INDEX_BUFFER_VIEW IndexBufferView;
	};
}
#endif