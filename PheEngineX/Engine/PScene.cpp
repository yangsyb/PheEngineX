#include "pch.h"
#include "PScene.h"
#include "PUtility.h"
#include "PTask.h"
#include "PStaticMesh.h"
#include "PAssetManager.h"

namespace Phe
{
	PScene::PScene()
	{
		PMainCamera = std::make_shared<PPerspectiveCamera>(45.0f, 1920.0f, 1080.0f);
//		PMainCamera = std::make_shared<POrthographicCamera>(1000.0f, 1000.0f);
		PMainCameraController = std::make_unique<PCameraController>(PMainCamera);
	}

	PScene::~PScene()
	{
		PRender = nullptr;
	}

	// Add Mesh Data To MainThread Scene
	// Then Create PTask to RenderThread queue
	void PScene::AddStaticMesh(std::string StaticMeshName, Transform MeshTransform, std::string MaterialName)
	{
		if (!PRender) PRender = PRenderThread::Get();
		assert(PRender);
		PMeshDataStruct data = PAssetManager::GetSingleton().GetMeshData(StaticMeshName);
		if (SceneMeshList.count(StaticMeshName) == 0)
		{
			SceneMeshList.insert({ StaticMeshName, std::vector<Transform>{MeshTransform} });
			PTask* task = CreateTask(PTask, PRender->GetRenderScene()->AddMeshBufferAndPrimitive(StaticMeshName, data, MeshTransform, MaterialName));
			PRender->AddTask(task);
		}
		else
		{
			auto KVpair = SceneMeshList.find(StaticMeshName);
			std::vector<Transform>& TransformVec = KVpair->second;
			TransformVec.push_back(MeshTransform);
			PTask* task = CreateTask(PTask, PRender->GetRenderScene()->AddPrimitive(StaticMeshName, MeshTransform, MaterialName));
			PRender->AddTask(task);
		}
	}

	// Parse Json File Data
	void PScene::AddStaticMeshFromFile(const std::string FilePath, std::string MaterialName)
	{
		std::vector<std::pair<std::string, Transform>> StaticMeshTransformList;
		DeserilizeActorJsonFile(FilePath, StaticMeshTransformList);
		for (size_t index = 0; index < StaticMeshTransformList.size(); index++)
		{
			AddStaticMesh(StaticMeshTransformList[index].first, StaticMeshTransformList[index].second, MaterialName);
		}
	}

	void PScene::AddLight(std::string LightName, PLightDataStruct LightData)
	{
		SceneLightList.insert({LightName, LightData});
		PTask* task = CreateTask(PTask, PRender->GetRenderScene()->AddLight(LightName, LightData));
		PRender->AddTask(task);
	}

	void PScene::AddLightFromFile(const std::string FilePath)
	{

	}

	void PScene::SetLightDynamic(std::string LightName)
	{
		PTask* task = CreateTask(PTask, PRender->GetRenderScene()->SetLightDynamic(LightName));
		PRender->AddTask(task);
	}

	void PScene::ClearScene()
	{
		SceneMeshList.clear();
		PTask* task = CreateTask(PTask, PRender->GetRenderScene()->ClearScene());
		PRender->AddTask(task);
	}

	void PScene::Update()
	{
		PMainCameraController->OnUpdate();
		UpdateMainPassBuffer();
		std::cout<< "PX:"<<PMainCamera->GetPosition().x << "Y:"<<PMainCamera->GetPosition().y << "Z:"<<PMainCamera->GetPosition().z << std::endl;
		std::cout<< "RX:"<<PMainCamera->GetRotation().x << "Y:"<<PMainCamera->GetRotation().y << "Z:"<<PMainCamera->GetRotation().z << std::endl;
	}

	void PScene::UpdateMainPassBuffer()
	{
		if (!PRender) PRender = PRenderThread::Get();
		assert(PRender);
		PTask* task = CreateTask(PTask, PRender->GetRenderer()->UpdateCamera(PMainCamera->GetPassConstant()));
		PRender->AddTask(task);
	}

}


