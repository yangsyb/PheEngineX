#include "pch.h"
#include "PMaterial.h"
#include "PShaderManager.h"
namespace Phe
{

	PMaterial::PMaterial(std::string MName, std::shared_ptr<PShader> shader, std::string TName, glm::vec4 DiffuseAlbedo, glm::vec3 FresnelR0, float Roughness) : Shader(shader), MaterialName(MName), TextureName(TName),
		PDiffuseAlbedo(DiffuseAlbedo), PFresnelR0(FresnelR0), PRoughness(Roughness)
	{

	}

	PMaterial::~PMaterial()
	{

	}

	PRenderMaterial::PRenderMaterial(std::string MName, std::shared_ptr<PShader> shader, std::string TName, glm::vec4 DiffuseAlbedo /*= glm::vec4(1.f, 1.f, 1.f, 1.f)*/, glm::vec3 FresnelR0 /*= glm::vec3(0.05f, 0.05f, 0.05f)*/, float Roughness /*= 0.2f*/) :
		PMaterial(MName, shader, TName, DiffuseAlbedo, FresnelR0, Roughness), HeapOffsetIndex(0)
	{

	}

	PRenderMaterial::PRenderMaterial(std::shared_ptr<PMaterial> Mat) : PMaterial(Mat->GetName(), Mat->GetShader(), Mat->GetTextureName(), Mat->GetDiffuseAlbedo(), Mat->GetPFresnelR0(), Mat->GetRoughness())
	{

	}

	PRenderMaterial::~PRenderMaterial()
	{

	}

	void PRenderMaterial::SetDescriptorTable(UINT propertyID, CD3DX12_GPU_DESCRIPTOR_HANDLE descriptorTable)
	{
		PDescriptorTables[propertyID] = descriptorTable;
	}

	void PRenderMaterial::SetDescriptorTable(const std::string& property, CD3DX12_GPU_DESCRIPTOR_HANDLE descriptorTable)
	{
		PDescriptorTables[PShaderManager::GetSingleton().PropertyToID(property)] = descriptorTable;
	}

	void PRenderMaterial::SetConstantBuffer(UINT propertyID, D3D12_GPU_VIRTUAL_ADDRESS address)
	{
		PConstantBuffers[propertyID] = address;
	}

	void PRenderMaterial::SetConstantBuffer(const std::string& property, D3D12_GPU_VIRTUAL_ADDRESS address)
	{
		PConstantBuffers[PShaderManager::GetSingleton().PropertyToID(property)] = address;
	}

	void PRenderMaterial::BindParameters(ID3D12GraphicsCommandList* commandList)
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

	void PRenderMaterial::SetHeapOffsetIndex(UINT Index)
	{
		HeapOffsetIndex = Index;
	}

}