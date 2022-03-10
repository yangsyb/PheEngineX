#include "pch.h"
#include "PScene.h"
#include "PUtility.h"
#include "PTask.h"
#include "PStaticMesh.h"
#include "PAssetManager.h"
#include "RHI/PMaterial.h"

namespace Phe
{
	PScene::PScene()
	{
		PMainCamera = std::make_shared<PCamera>(45.0f, 1920.0f, 1080.0f);
		PMainCameraController = std::make_unique<PCameraController>(PMainCamera);
	}

	PScene::~PScene()
	{
		PRender = nullptr;
	}

	// Add Mesh Data To MainThread Scene
	// Then Create PTask to RenderThread queue
	void PScene::AddStaticMesh(std::string StaticMeshName, Transform MeshTransform, std::shared_ptr<PMaterial> MeshMaterial)
	{
		if (!PRender) PRender = PRenderThread::Get();
		assert(PRender);
		PMeshDataStruct data = PAssetManager::GetSingleton().GetMeshData(StaticMeshName);
		if (SceneMeshList.count(StaticMeshName) == 0)
		{
			SceneMeshList.insert({ StaticMeshName, std::vector<Transform>{MeshTransform} });
			PTask* task = new PTask([=]() {PRender->GetRenderScene()->AddActor(StaticMeshName, MeshTransform, MeshMaterial); });
			PRender->AddTask(task);
		}
		else
		{
			auto KVpair = SceneMeshList.find(StaticMeshName);
			std::vector<Transform>& TransformVec = KVpair->second;
			TransformVec.push_back(MeshTransform);
			PTask* task = new PTask([=]() {PRender->GetRenderScene()->AddActor(StaticMeshName, MeshTransform, MeshMaterial); });
			PRender->AddTask(task);
		}
	}

	// Parse Json File Data
	void PScene::AddStaticMeshFromFile(const std::string FilePath, std::shared_ptr<PMaterial> MeshMaterial)
	{
		std::vector<std::pair<std::string, Transform>> StaticMeshTransformList;
		DeserilizeActorJsonFile(FilePath, StaticMeshTransformList);
		for (size_t index = 0; index < StaticMeshTransformList.size(); index++)
		{
			AddStaticMesh(StaticMeshTransformList[index].first, StaticMeshTransformList[index].second, MeshMaterial);
		}

		//		PTask* task = new PTask([=]() {PRender->GetRenderScene()->AddActor(, MeshTransform, MeshMaterial); });
		//		PRender->AddTask(task);
	}


	void PScene::ClearScene()
	{
		SceneMeshList.clear();
		WPOMeshData.clear();
		WPOSceneMeshList.clear();
		PTask* task = new PTask([=]() {PRender->GetRenderScene()->ClearScene(); });
		PRender->AddTask(task);
	}

	void PScene::Update()
	{
		PMainCameraController->OnUpdate();
		UpdateMainPassBuffer();
	}

	void PScene::UpdateMainPassBuffer()
	{
		if (!PRender) PRender = PRenderThread::Get();
		assert(PRender);
		PTask* task = new PTask([=]() {PRender->GetRenderScene()->UpdateCamera(PMainCamera->GetPassConstant()); });
		PRender->AddTask(task);
	}

}


