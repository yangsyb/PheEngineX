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
		void ClearScene();
		void Update();

		void UpdateMainPassBuffer();

	private:
		std::unordered_map<std::string, PMeshDataStruct> MeshData;
		std::unordered_map<std::string, std::vector<Transform>> SceneMeshList;
		PRenderThread* PRender;
		std::shared_ptr<PCamera> PMainCamera;
		std::unique_ptr<PCameraController> PMainCameraController;
	};
}