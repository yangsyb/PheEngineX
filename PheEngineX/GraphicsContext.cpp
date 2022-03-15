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


UINT GraphicContext::GetInputLayoutIndex(bool hasColor, bool hasNormal)
{
	UINT mask = 0;
	if (hasColor) { mask = mask | (1 << 0); }
	if (hasNormal) { mask = mask | (1 << 1); }
	auto&& InputLayoutIterator = InputLayoutMap.find(mask);
	if (InputLayoutIterator == InputLayoutMap.end())
	{
		std::unique_ptr<std::vector<D3D12_INPUT_ELEMENT_DESC>> desc = std::make_unique<std::vector<D3D12_INPUT_ELEMENT_DESC>>();
		GenerateInputElementDesc(*desc.get(), hasColor, hasNormal);
		UINT index = UINT(InputLayoutsVec.size());
		InputLayoutMap[mask] = index;
		InputLayoutsVec.push_back(move(desc));
		return index;
	}
	return InputLayoutIterator->second;
}

void GraphicContext::GenerateInputElementDesc(std::vector<D3D12_INPUT_ELEMENT_DESC>& desc, bool hasColor, bool hasNormal)
{
	desc.reserve(3);
	desc.push_back(
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0 }
	);
	UINT offset = 12;
	if (hasColor)
	{
		desc.push_back(
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offset, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		);
		offset += 16;
	}
	else
	{
		desc.push_back(
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		);
	}

	if (hasNormal)
	{
		desc.push_back(
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offset, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		);
		offset += 16;
	}
	else
	{
		desc.push_back(
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		);
	}
}

std::vector<D3D12_INPUT_ELEMENT_DESC>* GraphicContext::GetInputElementDesc(UINT index)
{
	return InputLayoutsVec[index].get();
}

ID3D12PipelineState* GraphicContext::GetPSO(std::shared_ptr<Phe::PShader> shader)
{
	auto&& PsoMapIterator = PSOMap.find(shader);
	if (PsoMapIterator == PSOMap.end())
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PsoDesc;
		ZeroMemory(&PsoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
		//To Refactor
		std::vector<D3D12_INPUT_ELEMENT_DESC> InputLayout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXTCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
		};

		PsoDesc.InputLayout = { InputLayout.data(), (UINT)InputLayout.size() };
		shader->SetPSODesc(&PsoDesc);
		PsoDesc.NumRenderTargets = 1;
		PsoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		PsoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		PsoDesc.SampleMask = UINT_MAX;
		PsoDesc.SampleDesc.Count = 1;
		PsoDesc.SampleDesc.Quality = 0;
		PsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		ComPtr<ID3D12PipelineState> Pso;
		ThrowIfFailed(PDevice->CreateGraphicsPipelineState(&PsoDesc, IID_PPV_ARGS(&Pso)));
		PSOMap.insert_or_assign(shader, Pso);
		return Pso.Get();
	}
	return PsoMapIterator->second.Get();
}