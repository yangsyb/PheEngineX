#pragma once
#include "pch.h"
#include "PShader.h"

namespace Phe
{

	PShader::PShader(const std::string ShaderName, const std::wstring FilePath, std::string VS, std::string PS) : PFilePath(FilePath), PVSEntry(VS), PPSEntry(PS), PShaderName(ShaderName)
	{

	}

	PShader::~PShader()
	{

	}

}