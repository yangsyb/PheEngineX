#pragma once
#include "pch.h"

namespace Phe
{
	class PShader
	{
	public:
		PShader(const std::string ShaderName, const std::wstring FilePath, std::string VS = "VS", std::string PS = "PS");
		virtual ~PShader();

		void RegisterResourceBinding(UINT32 PrimitiveId, std::string ParameterName, UINT32 BindingOffset);
		void UnRegisterResourceBinding(UINT32 PrimitiveId);

		virtual std::string GetShaderName() { return PShaderName; }
		virtual std::wstring GetShaderFilePath() { return PFilePath; }
		virtual std::string GetVSEntry() { return PVSEntry; }
		virtual std::string GetPSEntry() { return PPSEntry; }
//		virtual std::vector<ShaderParameter> GetShaderParameters() { return Params; }
//		virtual std::unordered_map<std::string, UINT32> GetResourceBindingByPrimitiveName(UINT32 PrimitiveId) { return ResourceBinding.at(PrimitiveId); }
	protected:
		std::string PShaderName;
		std::wstring PFilePath;
		std::string PVSEntry;
		std::string PPSEntry;
//		std::unordered_map<UINT32, std::unordered_map<std::string, UINT32>> ResourceBinding;
	};
}