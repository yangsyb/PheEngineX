#pragma once
#include "pch.h"

namespace Phe
{
	enum class ShaderParamType
	{
		ConstantBuffer,
		CBVDescriptorHeap,
		SRVDescriptorHeap,
		UAVDescriptorHeap,
	};

	struct ShaderParameter
	{
		std::string name;
		ShaderParamType type;
		UINT descriptorNums;
		UINT baseRegister;
		UINT registerSpace;

		ShaderParameter(
			const std::string& name,
			ShaderParamType type,
			UINT descriptorNums,
			UINT baseRegister,
			UINT registerSpace) :
			name(name),
			type(type),
			descriptorNums(descriptorNums),
			baseRegister(baseRegister),
			registerSpace(registerSpace) {}
	};

	class PShader : std::enable_shared_from_this<PShader>
	{
	public:
		PShader(const std::wstring FilePath, std::string VS = "VS", std::string PS = "PS");
		virtual ~PShader();
		bool operator==(const std::shared_ptr<PShader> p) { return this->GetName() == p->GetName(); }

		virtual void Register();
		virtual void Initialize();
		virtual void ReflectShader();
		void BindRootSignature(ID3D12GraphicsCommandList* commandList);
		void SetDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT paramID, CD3DX12_GPU_DESCRIPTOR_HANDLE handle);
		void SetRootConstantBufferView(ID3D12GraphicsCommandList* commandList, UINT paramID, D3D12_GPU_VIRTUAL_ADDRESS address);

		void SetPSODesc(D3D12_GRAPHICS_PIPELINE_STATE_DESC* psoDesc);

		std::string GetName() { return PName; }
	protected:
		std::string PName;
		std::unordered_map<UINT, UINT> ParamMap;
		std::vector<ShaderParameter> Params;

		std::wstring PFilePath;
		std::string PVSEntry;
		std::string PPSEntry;

	private:
		ComPtr<ID3D12RootSignature> PRootSignature;
		ComPtr<ID3DBlob> PVS;
		ComPtr<ID3DBlob> PPS;
		D3D12_RASTERIZER_DESC PRasterizerState;
		D3D12_DEPTH_STENCIL_DESC PDepthStencilState;
		D3D12_BLEND_DESC PBlendState;
	};
}




