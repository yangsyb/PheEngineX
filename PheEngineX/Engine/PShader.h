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


	class PShader
	{
	public:
		PShader(const std::string ShaderName, const std::wstring FilePath, std::string VS = "VS", std::string PS = "PS");
		virtual ~PShader();

		void RegisterResourceBinding(UINT32 PrimitiveId, std::string ParameterName, UINT32 BindingOffset);
		void UnRegisterResourceBinding(UINT32 PrimitiveId);

		std::string GetShaderName() { return PShaderName; }
		std::wstring GetShaderFilePath() { return PFilePath; }
		std::vector<ShaderParameter> GetShaderParameters() { return Params; }
		std::unordered_map<std::string, UINT32> GetResourceBindingByPrimitiveName(UINT32 PrimitiveId) { return ResourceBinding.at(PrimitiveId); }
	protected:
		std::string PShaderName;
		std::wstring PFilePath;
		std::string PVSEntry;
		std::string PPSEntry;

		std::unordered_map<UINT, UINT> ParamMap;
		std::vector<ShaderParameter> Params;
		std::unordered_map<UINT32, std::unordered_map<std::string, UINT32>> ResourceBinding;
	};
}