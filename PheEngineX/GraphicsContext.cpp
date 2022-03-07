#include "pch.h"
#include "GraphicsContext.h"
#include "DxException.h"

GraphicContext::GraphicContext(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12CommandAllocator* commandAllocator, ID3D12CommandQueue* commandQueue, ID3D12Fence* fence, UINT CbvSrvUavDescriptorSize) :
	PDevice(device), PCommandList(commandList), PCommandAllocator(commandAllocator), PCommandQueue(commandQueue), PFence(fence), PCbvSrvUavDescriptorSize(CbvSrvUavDescriptorSize)
{

}

GraphicContext::~GraphicContext()
{
	PCommandList = nullptr;
	PCommandAllocator = nullptr;
	PCommandQueue = nullptr;
	PFence = nullptr;
}

void GraphicContext::ResetCommandList()
{
	PCommandList->Reset(PCommandAllocator, nullptr);
}

void GraphicContext::ExecuteCommandList()
{
	ThrowIfFailed(PCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { PCommandList };
	PCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
}