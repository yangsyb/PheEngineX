#if defined (RenderDeviceDX12)
#pragma once
#include "Engine/Core/PShader.h"
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


	class PDX12Shader : public PShader
	{
	public:
		PDX12Shader(const std::string ShaderName, const std::wstring FilePath, std::string VS = "VS", std::string PS = "PS");

		void SetRootSignature(ComPtr<ID3D12RootSignature>& RootSignature) { PRootSignature = RootSignature; }
		void SetVS(ComPtr<ID3DBlob>& VS) { PVS = VS; }
		void SetPS(ComPtr<ID3DBlob>& PS) { PPS = PS; }
		void SetRasterizerState(D3D12_RASTERIZER_DESC RasterizerState) { PRasterizerState = RasterizerState; }
		void SetDepthStencilState(D3D12_DEPTH_STENCIL_DESC DepthStencilState) { PDepthStencilState = DepthStencilState; }
		void SetBlendState(D3D12_BLEND_DESC BlendState) { PBlendState = BlendState; }

		void SetPSODesc(D3D12_GRAPHICS_PIPELINE_STATE_DESC* psoDesc);
		std::unordered_map<UINT, UINT> GetParamMap() { return ParamMap; }
		std::vector<ShaderParameter> GetParams() { return Params; }

		ComPtr<ID3D12RootSignature> GetRootSignature() { return PRootSignature; }
	protected:
		ComPtr<ID3D12RootSignature> PRootSignature;
		ComPtr<ID3DBlob> PVS;
		ComPtr<ID3DBlob> PPS;
		std::unordered_map<UINT, UINT> ParamMap;
		std::vector<ShaderParameter> Params;
		D3D12_RASTERIZER_DESC PRasterizerState;
		D3D12_DEPTH_STENCIL_DESC PDepthStencilState;
		D3D12_BLEND_DESC PBlendState;
	};
}


#endif