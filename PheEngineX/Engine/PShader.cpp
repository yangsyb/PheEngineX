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

	void PShader::RegisterResourceBinding(UINT32 PrimitiveId, std::string ParameterName, UINT32 BindingOffset)
	{
		if(ResourceBinding.count(PrimitiveId)>0)
		{
			ResourceBinding.at(PrimitiveId).insert({ParameterName, BindingOffset});
		}
		else
		{
			std::unordered_map<std::string, UINT32> BindingMap;
			BindingMap.insert({ParameterName, BindingOffset});
			ResourceBinding.insert({ PrimitiveId, BindingMap });
		}
	}

	void PShader::UnRegisterResourceBinding(UINT32 PrimitiveId)
	{
		ResourceBinding.erase(PrimitiveId);
	}

}