#include "pch.h"
#include "PMaterial.h"
#include "PShaderManager.h"
namespace Phe
{

	PMaterial::PMaterial(std::string MName, std::shared_ptr<PShader> shader) : Shader(shader), MaterialName(MName)
	{

	}

	PMaterial::~PMaterial()
	{

	}

	void PMaterial::SetDescriptorTable(UINT propertyID, CD3DX12_GPU_DESCRIPTOR_HANDLE descriptorTable)
	{
		PDescriptorTables[propertyID] = descriptorTable;
	}

	void PMaterial::SetDescriptorTable(const std::string& property, CD3DX12_GPU_DESCRIPTOR_HANDLE descriptorTable)
	{
		PDescriptorTables[PShaderManager::GetSingleton().PropertyToID(property)] = descriptorTable;
	}

	void PMaterial::SetConstantBuffer(UINT propertyID, D3D12_GPU_VIRTUAL_ADDRESS address)
	{
		PConstantBuffers[propertyID] = address;
	}

	void PMaterial::SetConstantBuffer(const std::string& property, D3D12_GPU_VIRTUAL_ADDRESS address)
	{
		PConstantBuffers[PShaderManager::GetSingleton().PropertyToID(property)] = address;
	}

	void PMaterial::BindParameters(ID3D12GraphicsCommandList* commandList)
	{
		for (auto& DescriptorData : PDescriptorTables)
		{
			Shader->SetDescriptorTable(commandList, DescriptorData.first, DescriptorData.second);
		}
		for (auto& ConstantData : PConstantBuffers)
		{
			Shader->SetRootConstantBufferView(commandList, ConstantData.first, ConstantData.second);
		}
	}

}