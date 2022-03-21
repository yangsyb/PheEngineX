#include "pch.h"
#if defined (RenderDeviceDX12)
#include "PDX12GPUMeshBuffer.h"

namespace Phe
{

	PDX12GPUMeshBuffer::PDX12GPUMeshBuffer() : Buffer(nullptr), BufferUpload(nullptr), VertexBufferView(D3D12_VERTEX_BUFFER_VIEW()), IndexBufferView(D3D12_INDEX_BUFFER_VIEW())
	{

	}

	PDX12GPUMeshBuffer::~PDX12GPUMeshBuffer()
	{

	}

}
#endif