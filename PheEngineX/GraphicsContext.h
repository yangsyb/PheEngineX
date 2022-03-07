#pragma once
#include "pch.h"

class GraphicContext : public Singleton<GraphicContext>
{
public:
	GraphicContext(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList,
		ID3D12CommandAllocator* commandAllocator,
		ID3D12CommandQueue* commandQueue,
		ID3D12Fence* fence,
		UINT CbvSrvUavDescriptorSize);
	~GraphicContext();

	ComPtr<ID3D12Device> Device() { return PDevice; }
	ID3D12GraphicsCommandList* CommandList() { return PCommandList; }
	UINT GetCbvSrvUavDescriptorSize() { return PCbvSrvUavDescriptorSize; }
	void ResetCommandList();
	void ExecuteCommandList();

private:
//	ID3D12Device* PDevice;
	ComPtr<ID3D12Device> PDevice;
	ID3D12GraphicsCommandList* PCommandList;
	ID3D12CommandAllocator* PCommandAllocator;
	ID3D12CommandQueue* PCommandQueue;
	ID3D12Fence* PFence;
	UINT PCbvSrvUavDescriptorSize = 0;
};
