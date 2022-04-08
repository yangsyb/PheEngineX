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
		PMainPerspectCamera = std::make_shared<PPerspectiveCamera>(45.0f, 1920.0f, 1080.0f);
		PMainCameraController = std::make_unique<PCameraController>(PMainPerspectCamera);
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
		NodeLight->GetLightView()->SetDirection(glm::vec3(0,0,0) - NodeLight->GetLightView()->GetPosition());
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
 				static bool IsLeft = true;
				glm::vec3 CurrentPosition = PMainLight->GetLightView()->GetPosition();
 				if (IsLeft)
 				{
					if(CurrentPosition.y - 0.03 < -55)
					{
						IsLeft = !IsLeft;
					}
					PMainLight->SetPosition((glm::vec3(CurrentPosition.x, CurrentPosition.y - 0.03, CurrentPosition.z)));
					PMainLight->GetLightView()->SetDirection(glm::vec3(0, 0, 0) - PMainLight->GetLightView()->GetPosition());
					PTask* task = CreateTask(PTask, PRender->GetRenderer()->UpdateLight(PMainLight->GetLinkedRenderLight(), PMainLight->GetPerLightCBuffer()));
					PRender->AddTask(task);
 				}
 				else
 				{
					if (CurrentPosition.y + 0.03 > 55)
					{
						IsLeft = !IsLeft;
					}
					PMainLight->SetPosition((glm::vec3(CurrentPosition.x, CurrentPosition.y + 0.03, CurrentPosition.z)));
					PMainLight->GetLightView()->SetDirection(glm::vec3(0, 0, 0) - PMainLight->GetLightView()->GetPosition());
					PTask* task = CreateTask(PTask, PRender->GetRenderer()->UpdateLight(PMainLight->GetLinkedRenderLight(), PMainLight->GetPerLightCBuffer()));
					PRender->AddTask(task);
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
		PerCameraCBuffer PerOrthoCameraBuffer;
		auto CameraPassConstant = PMainPerspectCamera->GetPassConstant();
		PerCameraBuffer.Time = PEngine::GetSingleton().GetTimer().TotalTime();
		PerCameraBuffer.View = CameraPassConstant.View;
		PerCameraBuffer.Proj = CameraPassConstant.Proj;
		PerCameraBuffer.CameraLocation = CameraPassConstant.CameraLocation;

		PerOrthoCameraBuffer = PerCameraBuffer;
		PerOrthoCameraBuffer.Proj = PMainPerspectCamera->AsOrthoProjection(PSceneCenter, PSceneRadius);

//		PerCameraBuffer.CameraLocationMat = CameraPassConstant.CameraLocationMat;
		if(PMainLight)
		{
			PerCameraBuffer.ShadowTransform = PMainLight->GetPassCBuffer().ShadowTransform;
		}
		PTask* task = CreateTask(PTask, PRender->GetRenderer()->UpdateCamera(PerCameraBuffer));
		PRender->AddTask(task);
		PTask* task1 = CreateTask(PTask, PRender->GetRenderer()->UpdateOrthoCamera(PerOrthoCameraBuffer));
		PRender->AddTask(task1);
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