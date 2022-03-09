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
		UINT GetPerMaterialBufferID() { return PerMaterialBufferID; }

		void CompileAllShader();
		void CreateUploadBuffer();
	private:
		UINT CurID;
		std::unordered_map<std::string, UINT> PropertyID;
		std::vector<std::shared_ptr<PShader>> ShaderList;
		std::unordered_map<std::string, ComPtr<ID3D12Resource>> UploadBufferMap;
		std::unordered_map<std::string, BYTE*> MappedDataMap;

		UINT PerObjectBufferID;
		UINT PerCameraBufferID;
		UINT PerMaterialBufferID;

	};
}
