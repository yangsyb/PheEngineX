#pragma once
#include "pch.h"
#include "PStaticMesh.h"
#include "Transform.h"
#include "PRenderThread.h"
#include "PTask.h"
#include "PCamera.h"
#include "PCameraController.h"

namespace Phe
{
	class PScene
	{
	public:
		PScene();
		~PScene();

		void AddStaticMesh(std::shared_ptr<PStaticMesh> StaticMesh, Transform MeshTransform);
		void AddStaticMeshFromFile(const std::string FilePath);

		//Those Two function below are static only for test WPO!!
		void AddStaticMeshWPO(std::shared_ptr<PStaticMesh> StaticMesh, Transform MeshTransform);
		void AddStaticMeshWPOFromFile(const std::string FilePath);

		void ClearScene();
		void Update();

		void UpdateMainPassBuffer();

	private:
		std::unordered_map<std::string, PMeshDataStruct> MeshData;
		std::unordered_map<std::string, std::vector<Transform>> SceneMeshList;
		PRenderThread* PRender;
		std::shared_ptr<PCamera> PMainCamera;
		std::unique_ptr<PCameraController> PMainCameraController;

		std::unordered_map<std::string, PMeshDataStruct> WPOMeshData;
		std::unordered_map<std::string, std::vector<Transform>> WPOSceneMeshList;
	};
}