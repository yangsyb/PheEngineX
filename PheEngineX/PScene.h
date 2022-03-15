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

		void AddStaticMesh(std::string StaticMeshName, Transform MeshTransform, std::string MaterialName);
		void AddStaticMeshFromFile(const std::string FilePath, std::string MaterialName);

		void ClearScene();
		void Update();

		void UpdateMainPassBuffer();

	private:
		std::unordered_map<std::string, std::vector<Transform>> SceneMeshList;
		PRenderThread* PRender;
		std::shared_ptr<PCamera> PMainCamera;
		std::unique_ptr<PCameraController> PMainCameraController;

		std::unordered_map<std::string, PMeshDataStruct> WPOMeshData;
		std::unordered_map<std::string, std::vector<Transform>> WPOSceneMeshList;
	};
}