#include "pch.h"
#include "PScene.h"
#include "PNodeStaticMesh.h"
#include "PNodeLight.h"
#include "Engine/Core/PUtility.h"
#include "Engine/Core/PTask.h"
#include "Engine/Editor/PAssetManager.h"
#include "Engine/Core/PEngine.h"
namespace Phe
{
	PNodeScene::PNodeScene(PNode* Parent) : PNode(nullptr)
	{

	}

	PNodeScene::~PNodeScene()
	{
	
	}

	PScene::PScene() : PSceneCenter(glm::vec3(0, 0, 0)), PSceneRadius(35), PMainLight(nullptr)
	{
		PSceneNode = PNodeFactory::CreateNode<PNodeScene>(nullptr, "MainScene");
		PMainCamera = std::make_shared<PPerspectiveCamera>(45.0f, 1920.0f, 1080.0f);
//		PMainCamera = std::make_shared<POrthographicCamera>(1000.0f, 1000.0f);
		PMainCameraController = std::make_unique<PCameraController>(PMainCamera);
	}

	PScene::~PScene()
	{
		PSceneNode->RemoveAllChild();
		PRender = nullptr;
		ReleasePtr(PSceneNode);
	}

	// Add Mesh Data To MainThread Scene
	// Then Create PTask to RenderThread queue
	void PScene::AddStaticMesh(std::string StaticMeshName, Transform MeshTransform, std::string MaterialName)
	{
		if (!PRender) PRender = PRenderThread::Get();
		assert(PRender);
		PStaticMesh* StaticMesh = PAssetManager::GetSingleton().GetMeshData(StaticMeshName);
		PMaterial* Material = PAssetManager::GetSingleton().GetMaterialData(MaterialName);

		PNodeStaticMesh* NodeStaticMesh = PNodeFactory::CreateNode<PNodeStaticMesh>(PSceneNode, GetIncrementID(PSceneNode->GetLastChildID()));
		PSceneNode->AddChild(NodeStaticMesh);
		StaticMesh->BindNodeStaticMesh(NodeStaticMesh);
		NodeStaticMesh->BindLinkedStaticMesh(StaticMesh);
		NodeStaticMesh->SetStaticMeshName(StaticMeshName);
		NodeStaticMesh->SetTransform(MeshTransform);
		PTask* task = CreateTask(PTask, PRender->GetRenderScene()->AddMeshBufferAndPrimitive(NodeStaticMesh, Material, NodeStaticMesh->GetTransform()));
		PRender->AddTask(task);
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

	void PScene::AddLight(std::string LightName, Transform LightTransform)
	{
		PLight* Light = PAssetManager::GetSingleton().GetLightData(LightName);
		if (!PRender) PRender = PRenderThread::Get();
		assert(PRender);

		PNodeLight* NodeLight = PNodeFactory::CreateNode<PNodeLight>(PSceneNode, GetIncrementID(PSceneNode->GetLastChildID()));
		PSceneNode->AddChild(NodeLight);
		if(!PMainLight) PMainLight = NodeLight;
		NodeLight->SetAsLight(Light);
		Light->BindNodeLight(NodeLight);
		NodeLight->BindLinkedLight(Light);
		NodeLight->SetPosition(LightTransform.GetPosition());
		NodeLight->SetRotation(LightTransform.GetRotation());
 		PTask* task = CreateTask(PTask, PRender->GetRenderScene()->AddLight(NodeLight));
 		PRender->AddTask(task);
	}


	void PScene::AddLightFromFile(const std::string FilePath)
	{

	}

	void PScene::SetLightDynamic()
	{
		if(PMainLight)
		{
			PMainLight->SetIsDynamic(!PMainLight->GetIsDynamic());
		}
	}

	void PScene::ClearScene()
	{
		PSceneNode->RemoveAllChild();
		PMainLight = nullptr;
	}

	void PScene::Update()
	{
		PMainCameraController->OnUpdate();
		UpdateMainPassBuffer();
		if(PMainLight)
		{
			if(PMainLight->GetIsDynamic())
			{
				static bool IsUp = true;
				glm::vec3 CurrentRotation = PMainLight->GetLightView()->GetRotation();
				if (IsUp)
				{
					PMainLight->GetLightView()->SetRotation(glm::vec3(CurrentRotation.x, CurrentRotation.y + 0.05, CurrentRotation.z));
					if (CurrentRotation.y + 0.02 > -20)
					{
						IsUp = false;
					}
				}
				else
				{
					PMainLight->GetLightView()->SetRotation(glm::vec3(CurrentRotation.x, CurrentRotation.y - 0.05, CurrentRotation.z));
					if (CurrentRotation.y - 0.02 < -70)
					{
						IsUp = true;
					}
				}
			}
			UpdateShadowPassBuffer();
		}
	}

	void PScene::UpdateMainPassBuffer()
	{
		if (!PRender) PRender = PRenderThread::Get();
		assert(PRender);
		PerCameraCBuffer PerCameraBuffer;
		auto CameraPassConstant = PMainCamera->GetPassConstant();
		PerCameraBuffer.Time = PEngine::GetSingleton().GetTimer().TotalTime();
		PerCameraBuffer.View = CameraPassConstant.View;
		PerCameraBuffer.Proj = CameraPassConstant.Proj;
		PerCameraBuffer.CameraLocation = CameraPassConstant.CameraLocation;
//		PerCameraBuffer.CameraLocationMat = CameraPassConstant.CameraLocationMat;
		if(PMainLight)
		{
			PerCameraBuffer.ShadowTransform = PMainLight->GetPassCBuffer().ShadowTransform;
		}
		PTask* task = CreateTask(PTask, PRender->GetRenderer()->UpdateCamera(PerCameraBuffer));
		PRender->AddTask(task);
	}

	void PScene::UpdateShadowPassBuffer()
	{
		if (!PRender) PRender = PRenderThread::Get();
		assert(PRender);
		PMainLight->GetLightView()->RecalculateOrtho(PSceneCenter, PSceneRadius);
		PerCameraCBuffer PerCameraBuffer;
		auto LightData = PMainLight->GetPassCBuffer();
		PerCameraBuffer.Time = PEngine::GetSingleton().GetTimer().TotalTime();
		PerCameraBuffer.View = LightData.View;
		PerCameraBuffer.Proj = LightData.Proj;
		PerCameraBuffer.CameraLocation = LightData.CameraLocation;
//		PerCameraBuffer.CameraLocationMat = LightData.CameraLocationMat;
		PerCameraBuffer.ShadowTransform = PMainLight->GetVP();
		PTask* task = CreateTask(PTask, PRender->GetRenderScene()->GetMainRenderLight()->UpdateCameraBuffer(PerCameraBuffer));
		PRender->AddTask(task);
	}
}