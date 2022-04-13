#if defined (RenderDeviceDX12)
#pragma once
#include "Engine/Editor/PShader.h"
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
		virtual ~PDX12Shader();
		void SetRootSignature(ComPtr<ID3D12RootSignature>& RootSignature) { PRootSignature = RootSignature; }
		void SetVS(ComPtr<ID3DBlob>& VS) { PVS = VS; }
		void SetPS(ComPtr<ID3DBlob>& PS) { PPS = PS; }

		std::unordered_map<UINT, UINT> GetParamMap() { return ParamMap; }
		std::vector<ShaderParameter> GetParams() { return Params; }

		ComPtr<ID3D12RootSignature> GetRootSignature() { return PRootSignature; }
		ComPtr<ID3DBlob> GetVS() { return PVS; }
		ComPtr<ID3DBlob> GetPS() { return PPS; }
	protected:
		ComPtr<ID3D12RootSignature> PRootSignature;
		ComPtr<ID3DBlob> PVS;
		ComPtr<ID3DBlob> PPS;
		std::unordered_map<UINT, UINT> ParamMap;
		std::vector<ShaderParameter> Params;
	};
}


#endif