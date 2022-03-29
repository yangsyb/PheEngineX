#pragma once
#include "pch.h"
#include "PShader.h"

namespace Phe
{
	class PShaderManager
	{
	public:
		static PShaderManager* Get();
		static void CreateShaderManager();
		static void DestroyShaderManager();
		static PShaderManager* ShaderManager;

		PShaderManager();
		virtual ~PShaderManager();

		virtual void AddShader(std::string ShaderName, std::wstring FilePath) = 0;
		virtual void AddShader(PShader* Shader) = 0;
		virtual PShader* GetShaderByName(std::string ShaderName) = 0;

	protected:
	};
}