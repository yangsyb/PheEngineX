#pragma once
#include "pch.h"
#include "PNode.h"
#include "PStaticMesh.h"
#include "Engine/Core/Transform.h"
#include "Render/PRenderThread.h"
#include "PCamera.h"
#include "Engine/Core/PCameraController.h"
#include "PLight.h"

namespace Phe
{

	class PNodeScene : public PNode
	{
		DECLARE_NODE_WITH_CONSTRUCTOR(Scene);
		public:
		virtual ~PNodeScene();
	};

	class PScene
	{
	public:
		PScene();
		~PScene();

		void AddStaticMesh(std::string StaticMeshName, Transform MeshTransform, std::string MaterialName);
		void AddStaticMeshFromFile(const std::string FilePath, std::string MaterialName);
		void AddLight(std::string LightName, PLightDataStruct LightData);
		void AddLightFromFile(const std::string FilePath);

		void SetLightDynamic(std::string LightName);

		void ClearScene();
		void Update();

		void UpdateMainPassBuffer();
		void UpdateShadowPassBuffer();

	private:
		PNodeScene* PSceneNode;

		std::unordered_map<std::string, std::vector<Transform>> SceneMeshList;
		std::unordered_map<std::string, PLightDataStruct> SceneLightList;
		PRenderThread* PRender;
		std::shared_ptr<PPerspectiveCamera> PMainCamera;
//		std::shared_ptr<POrthographicCamera> PMainCamera;
		std::unique_ptr<PCameraController> PMainCameraController;
	};
}