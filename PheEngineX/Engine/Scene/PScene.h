#pragma once
#include "pch.h"
#include "PNode.h"
#include "Engine/Editor/PStaticMesh.h"
#include "Engine/Core/Transform.h"
#include "Render/PRenderThread.h"
#include "PCamera.h"
#include "PCameraController.h"
#include "Engine/Editor/PLight.h"


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
		void AddLight(std::string LightName, Transform LightTransform);
		void AddLightFromFile(const std::string FilePath);

		void SetLightDynamic();

		void ClearScene();
		void Update();

		void UpdateMainPassBuffer();
		void UpdateShadowPassBuffer();

		glm::vec3 GetSceneCenter() { return PSceneCenter; }
		float GetSceneRadius() { return PSceneRadius; }
	private:
		glm::vec3 PSceneCenter;
		float PSceneRadius;

		PNodeScene* PSceneNode;
		PNodeLight* PMainLight;
		std::unordered_map<std::string, std::vector<Transform>> SceneMeshList;
		std::unordered_map<std::string, PLightDataStruct> SceneLightList;
		PRenderThread* PRender;
		std::shared_ptr<PPerspectiveCamera> PMainCamera;
//		std::shared_ptr<POrthographicCamera> PMainCamera;
		std::unique_ptr<PCameraController> PMainCameraController;

		
	};
}