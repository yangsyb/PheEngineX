#pragma once
#include "pch.h"
#include "PShader.h"
#include "PTexture.h"

namespace Phe
{
	class PMaterial
	{
	public:
		PMaterial(std::string MName, std::shared_ptr<PShader> shader, std::string TName, glm::vec4 DiffuseAlbedo = glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3 FresnelR0 = glm::vec3(0.05f, 0.05f, 0.05f), float Roughness = 0.2f);
		virtual ~PMaterial();
		bool operator==(const std::shared_ptr<PMaterial> m) { return this->GetName() == m->GetName(); }

		virtual std::string GetName() { return MaterialName; }
		virtual std::shared_ptr<PShader> GetShader() { return Shader; }
		virtual std::string GetTextureName() { return TextureName; }
		virtual glm::vec4 GetDiffuseAlbedo() { return PDiffuseAlbedo; }
		virtual glm::vec3 GetPFresnelR0() { return PFresnelR0; }
		virtual float GetMetallic() { return PMetallic; }
		virtual float GetSpecular() { return PSpecular; }
		virtual float GetRoughness() { return PRoughness; }
	protected:
		std::string MaterialName;
		std::shared_ptr<PShader> Shader;
		std::string TextureName;

		glm::vec4 PDiffuseAlbedo;
		glm::vec3 PFresnelR0;
		float PMetallic;
		float PSpecular;
		float PRoughness;
		
	};

	class PRenderMaterial : public PMaterial
	{
	public:
		PRenderMaterial(std::string MName, std::shared_ptr<PShader> shader, std::string TName, glm::vec4 DiffuseAlbedo = glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3 FresnelR0 = glm::vec3(0.05f, 0.05f, 0.05f), float Roughness = 0.2f);
		PRenderMaterial(std::shared_ptr<PMaterial> Mat);
		virtual ~PRenderMaterial();
		void SetDescriptorTable(UINT propertyID, CD3DX12_GPU_DESCRIPTOR_HANDLE descriptorTable);
		void SetDescriptorTable(const std::string& property, CD3DX12_GPU_DESCRIPTOR_HANDLE descriptorTable);
		void SetConstantBuffer(UINT propertyID, D3D12_GPU_VIRTUAL_ADDRESS address);
		void SetConstantBuffer(const std::string& property, D3D12_GPU_VIRTUAL_ADDRESS address);
		void BindParameters(ID3D12GraphicsCommandList* commandList);

		void SetHeapOffsetIndex(UINT Index);
		UINT GetHeapOffsetIndex() { return HeapOffsetIndex; }
	protected:
		std::unordered_map<UINT, CD3DX12_GPU_DESCRIPTOR_HANDLE> PDescriptorTables;
		std::unordered_map<UINT, D3D12_GPU_VIRTUAL_ADDRESS> PConstantBuffers;

		UINT HeapOffsetIndex;
	};
}
