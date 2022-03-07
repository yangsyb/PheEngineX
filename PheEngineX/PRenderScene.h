#pragma once
#include "pch.h"
#include "Transform.h"
#include "PStaticMesh.h"

namespace Phe
{
	class PRenderScene
	{
	public:
		PRenderScene();
		~PRenderScene();

		void BuildMeshData(std::shared_ptr<PStaticMesh> StaticMesh, Transform MeshTransform);
		void AddExistedMesh(std::string MeshName, Transform MeshTransform);
		void ClearScene();
		void UpdateCamera(PassConstants passcb);

		void BuildConstantBuffer();
		void BuildPSO();
		void Render();

		ComPtr<ID3DBlob> CompileShader(const std::wstring& Filename, const D3D_SHADER_MACRO* Defines, const std::string& EntryPoint, const std::string& Target);
	private:
		UINT RenderMeshNum;
		std::unordered_map<std::string, std::shared_ptr<PRenderStaticMesh>> RenderMeshData;
		std::unordered_map<std::string, std::vector<Transform>> RenderSceneMeshList;


	private:
		UINT PCbvSrvUavDescriptorSize;
		ComPtr<ID3D12DescriptorHeap> CbvHeap;
		std::unique_ptr <UploadBuffer<ObjectConstants>> mObjectCB = nullptr;
		std::unique_ptr <UploadBuffer<PassConstants>> mPassCB = nullptr;

		ComPtr<ID3D12RootSignature> RootSignature = nullptr;
		ComPtr<ID3DBlob> MvsByteCode = nullptr;
		ComPtr<ID3DBlob> MpsByteCode = nullptr;
		std::vector<D3D12_INPUT_ELEMENT_DESC> InputLayout;
		ComPtr<ID3D12PipelineState> PSO = nullptr;
	};
}