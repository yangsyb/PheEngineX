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

		virtual void AddShader(std::string ShaderName, std::wstring FilePath);
		virtual void AddShader(PShader* Shader);
		virtual void AddCompiledShader(PShader* Shader);
		virtual PShader* GetShaderByName(std::string ShaderName);
		virtual std::unordered_map<std::string, PShader*> GetAllShaders() { return ShaderPool; }

		virtual PShader* GetCompiledShaderByName(std::string ShaderName);

		virtual UINT PropertyToID(std::string property);

	protected:
		UINT CurID;
		std::unordered_map<std::string, UINT> PropertyID;
		std::unordered_map<std::string, PShader*> ShaderPool;
		std::unordered_map<std::string, PShader*> CompiledShaderPool;
	};
}