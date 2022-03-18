#pragma once
#include "pch.h"
#include "Engine/PShaderManager.h"
#include "PDX12Shader.h"
namespace Phe
{
	class PDX12Shadermanager : public PShaderManager
	{
	public:
		PDX12Shadermanager();
		virtual ~PDX12Shadermanager();
		virtual void AddShader(PShader* Shader) override;
		virtual void AddShader(std::string ShaderName, std::wstring FilePath) override;
		virtual PShader* GetShaderByName(std::string ShaderName) override;

//		void RegisterShader(PDX12Shader* Shader);
		UINT PropertyToID(std::string property);


	private:
		UINT CurID;
		std::unordered_map<std::string, UINT> PropertyID;
		std::unordered_map<std::string, PShader*> ShaderPool;
	};
}
