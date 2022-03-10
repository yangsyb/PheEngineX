#pragma once
#include "pch.h"
#include "Transform.h"
#include "PStaticMesh.h"
#include "PActor.h"
#include "RHI/PGPUBuffer.h"

namespace Phe
{
	class DrawPool
	{
	public:
		DrawPool(std::string ShaderName);
		~DrawPool();
		void Push(std::shared_ptr<PActor> actor);

		std::string GetShaderName() { return PShaderName; }
		std::unordered_map<std::string, std::vector<std::shared_ptr<PActor>>> Request();
	private:
		std::string PShaderName;
		std::unordered_map<std::string, std::vector<std::shared_ptr<PActor>>> PActorMap;
	};

	class PRenderScene
	{
	public:
		PRenderScene();
		~PRenderScene();

		void AddActor(std::string StaticMeshName, Transform MeshTransform, std::shared_ptr<PMaterial> MeshMaterial);
		void AddActors(std::vector<std::string> StaticMeshName, std::vector<Transform> MeshTransform, std::vector<std::shared_ptr<PMaterial>> MeshMaterial);
		void ClearScene();
		void UpdateCamera(PerCameraCBuffer CameraCBuffer);

		void BuildConstantBuffer();
		void Render();

	private:


		UINT ActorNum;
		std::unordered_map<std::string, std::shared_ptr<PRenderStaticMesh>> RenderActorData;

		std::unordered_map<std::string, std::shared_ptr<DrawPool>> RenderActorPools;

	private:

		UINT PCbvSrvUavDescriptorSize;
		ComPtr<ID3D12DescriptorHeap> CbvHeap;
		// 		std::unique_ptr <PGpuUploadBuffer> PerObjCB;
		// 		std::unique_ptr <PGpuUploadBuffer> PerCameraCB;
		// 		std::unique_ptr <PGpuUploadBuffer> PerFrameCB;
		// 		std::unique_ptr <PGpuUploadBuffer> PerMaterialCB;

		std::unique_ptr <UploadBuffer<PerObjectCBuffer>> PerObjCB;
		std::unique_ptr <UploadBuffer<PerCameraCBuffer>> PerCameraCB;
		std::unique_ptr <UploadBuffer<PerFrameCBuffer>> PerFrameCB;


	};
}