#pragma once
#include "pch.h"
#include "PShader.h"

namespace Phe
{
	class PMaterial
	{
	public:
		PMaterial(std::string MName, std::shared_ptr<PShader> shader);
		~PMaterial();
		bool operator==(const std::shared_ptr<PMaterial> m) { return this->GetName() == m->GetName(); }
		void SetDescriptorTable(UINT propertyID, CD3DX12_GPU_DESCRIPTOR_HANDLE descriptorTable);
		void SetDescriptorTable(const std::string& property, CD3DX12_GPU_DESCRIPTOR_HANDLE descriptorTable);
		void SetConstantBuffer(UINT propertyID, D3D12_GPU_VIRTUAL_ADDRESS address);
		void SetConstantBuffer(const std::string& property, D3D12_GPU_VIRTUAL_ADDRESS address);
		void BindParameters(ID3D12GraphicsCommandList* commandList);
		std::string GetName() { return MaterialName; }
		std::shared_ptr<PShader> GetShader() { return Shader; }
	private:
		std::string MaterialName;
		std::shared_ptr<PShader> Shader;
		std::unordered_map<UINT, CD3DX12_GPU_DESCRIPTOR_HANDLE> PDescriptorTables;
		std::unordered_map<UINT, D3D12_GPU_VIRTUAL_ADDRESS> PConstantBuffers;
	};
}
