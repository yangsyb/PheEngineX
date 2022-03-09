#pragma once
#include "pch.h"
#include "RHI/PShader.h"

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


	UINT GetInputLayoutIndex(
		bool hasColor,
		bool hasNormal);
	void GenerateInputElementDesc(std::vector<D3D12_INPUT_ELEMENT_DESC>& desc,
		bool hasColor,
		bool hasNormal
		);
	std::vector<D3D12_INPUT_ELEMENT_DESC>* GetInputElementDesc(UINT index);
	ID3D12PipelineState* GetPSO(std::shared_ptr<Phe::PShader> shader);
	std::unordered_map<UINT, UINT> InputLayoutMap;
	std::vector<std::unique_ptr<std::vector<D3D12_INPUT_ELEMENT_DESC>>> InputLayoutsVec;
	std::unordered_map<std::shared_ptr<Phe::PShader>, ComPtr<ID3D12PipelineState>> PSOMap;
private:
//	ID3D12Device* PDevice;
	ComPtr<ID3D12Device> PDevice;
	ID3D12GraphicsCommandList* PCommandList;
	ID3D12CommandAllocator* PCommandAllocator;
	ID3D12CommandQueue* PCommandQueue;
	ID3D12Fence* PFence;
	UINT PCbvSrvUavDescriptorSize = 0;
};
