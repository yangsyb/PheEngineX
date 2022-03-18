#pragma once
#include "pch.h"

namespace Phe
{
	class PShader
	{
	public:
		PShader(const std::string ShaderName, const std::wstring FilePath, std::string VS = "VS", std::string PS = "PS");
		virtual ~PShader();

		std::string GetShaderName() { return PShaderName; }
		std::wstring GetShaderFilePath() { return PFilePath; }
	private:
		std::string PShaderName;
		std::wstring PFilePath;
		std::string PVSEntry;
		std::string PPSEntry;
	};
}