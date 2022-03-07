#include "pch.h"
#include "PScene.h"
#include "PUtility.h"
#include "PTask.h"
#include "PStaticMesh.h"
namespace Phe
{
	PScene::PScene()
	{
		PMainCamera = std::make_shared<PCamera>(45.0f, 1920.0f,1080.0f);
		PMainCameraController = std::make_unique<PCameraController>(PMainCamera);
	}

	PScene::~PScene()
	{
		PRender = nullptr;
	}

	// Add Mesh Data To MainThread Scene
	// Then Create PTask to RenderThread queue
	void PScene::AddStaticMesh(std::shared_ptr<PStaticMesh> StaticMesh, Transform MeshTransform)
	{
		if (!PRender) PRender = PRenderThread::Get();
		assert(PRender);
		if (MeshData.find(StaticMesh->GetName()) == MeshData.end())
		{
			PMeshDataStruct data;
			data.Vertices = StaticMesh->GetVertices();
			data.Indices = StaticMesh->GetIndices();
			data.Normal = StaticMesh->GetTangents();
			MeshData.insert({ StaticMesh->GetName(), data });
			SceneMeshList.insert({ StaticMesh->GetName(), std::vector<Transform>{MeshTransform} });
			PTask* task = new PTask([=]() {PRender->GetRenderScene()->BuildMeshData(StaticMesh, MeshTransform);});
			PRender->AddTask(task);
		}
		else
		{
			auto KVpair = SceneMeshList.find(StaticMesh->GetName());
			std::vector<Transform>& TransformVec = KVpair->second;
			TransformVec.push_back(MeshTransform);
//			SceneMeshList.insert({ StaticMesh->GetName(), TransformVec });
			PTask* task = new PTask([=]() {PRender->GetRenderScene()->AddExistedMesh(StaticMesh->GetName(), MeshTransform);});
			PRender->AddTask(task);
		}
	}

	// Parse Json File Data
	void PScene::AddStaticMeshFromFile(const std::string FilePath)
	{
 		std::vector<std::shared_ptr<PStaticMesh>> StaticMeshList;
		std::vector<Transform> StaticMeshTransformList;
 		DeserilizeJsonFile(FilePath, StaticMeshList, StaticMeshTransformList);
		for(size_t index = 0; index < StaticMeshList.size(); index++)
		{
			AddStaticMesh(StaticMeshList[index], StaticMeshTransformList[index]);
		}

	}

	void PScene::AddStaticMeshWPO(std::shared_ptr<PStaticMesh> StaticMesh, Transform MeshTransform)
	{
		if (!PRender) PRender = PRenderThread::Get();
		assert(PRender);
		if (WPOMeshData.find(StaticMesh->GetName()) == WPOMeshData.end())
		{
			PMeshDataStruct data;
			data.Vertices = StaticMesh->GetVertices();
			data.Indices = StaticMesh->GetIndices();
			data.Normal = StaticMesh->GetTangents();
			WPOMeshData.insert({ StaticMesh->GetName(), data });
			WPOSceneMeshList.insert({ StaticMesh->GetName(), std::vector<Transform>{MeshTransform} });
			PTask* task = new PTask([=]() {PRender->GetRenderScene()->BuildWPOMeshData(StaticMesh, MeshTransform); });
			PRender->AddTask(task);
		}
		else
		{
			auto KVpair = WPOSceneMeshList.find(StaticMesh->GetName());
			std::vector<Transform>& TransformVec = KVpair->second;
			TransformVec.push_back(MeshTransform);
			PTask* task = new PTask([=]() {PRender->GetRenderScene()->AddWPOExistedMesh(StaticMesh->GetName(), MeshTransform); });
			PRender->AddTask(task);
		}
	}

	void PScene::AddStaticMeshWPOFromFile(const std::string FilePath)
	{
		std::vector<std::shared_ptr<PStaticMesh>> StaticMeshList;
		std::vector<Transform> StaticMeshTransformList;
		DeserilizeJsonFile(FilePath, StaticMeshList, StaticMeshTransformList);
		for (size_t index = 0; index < StaticMeshList.size(); index++)
		{
			AddStaticMeshWPO(StaticMeshList[index], StaticMeshTransformList[index]);
		}
	}

	void PScene::ClearScene()
	{
		MeshData.clear();
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


