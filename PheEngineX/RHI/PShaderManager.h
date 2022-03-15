#pragma once
#include "pch.h"
#include "PShader.h"
namespace Phe
{
	class PShaderManager : public Singleton<PShaderManager>
	{
	public:
		PShaderManager();
		~PShaderManager();

		void AddShader(std::shared_ptr<PShader> Shader);

		UINT PropertyToID(std::string property);
		UINT GetPerObjectBufferID() { return PerObjectBufferID; }
		UINT GetPerCameraBufferID() { return PerCameraBufferID; }
		UINT GetPerFrameBufferID() { return PerFrameBufferID; }
		UINT GetPerMaterialBufferID() { return PerMaterialBufferID; }

		void CompileAllShader();
		void CreateUploadBuffer();
		std::shared_ptr<PShader> GetShader(std::string ShaderName);
		void AddConstantSize(std::string ShaderName, std::vector<UINT32> ConstantSize);
	private:
		UINT CurID;
		std::unordered_map<std::string, UINT> PropertyID;
		std::vector<std::shared_ptr<PShader>> ShaderList;
		std::unordered_map<std::string, std::vector<UINT32>> ShaderConstantSizeMap;

		UINT TextureID;
		UINT PerObjectBufferID;
		UINT PerCameraBufferID;
		UINT PerFrameBufferID;
		UINT PerMaterialBufferID;

	};
}
