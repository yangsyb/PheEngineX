#pragma once
#include "pch.h"
#include "PShader.h"

namespace Phe
{
	class PMaterial
	{
	public:
		PMaterial(std::shared_ptr<PShader> shader);
		~PMaterial();

		void SetDescriptorTable(UINT propertyID, CD3DX12_GPU_DESCRIPTOR_HANDLE descriptorTable);
		void SetDescriptorTable(const std::string& property, CD3DX12_GPU_DESCRIPTOR_HANDLE descriptorTable);
		void SetConstantBuffer(UINT propertyID, D3D12_GPU_VIRTUAL_ADDRESS address);
		void SetConstantBuffer(const std::string& property, D3D12_GPU_VIRTUAL_ADDRESS address);
		void BindParameters(ID3D12GraphicsCommandList* commandList);
		std::shared_ptr<PShader> GetShader() { return Shader; }
	private:
		std::shared_ptr<PShader> Shader;
		std::unordered_map<UINT, CD3DX12_GPU_DESCRIPTOR_HANDLE> PDescriptorTables;
		std::unordered_map<UINT, D3D12_GPU_VIRTUAL_ADDRESS> PConstantBuffers;
	};
}
