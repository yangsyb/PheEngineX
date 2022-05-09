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

		AddSkySphere("JsonFile\\SkySphere.json", PAssetManager::GetSingleton().GetTextureData("SkyBoxTexture"));
	}

	PScene::~PScene()
	{
		PSceneNode->RemoveAllChild();
		PRender = nullptr;
		ReleasePtr(PSceneNode);
		ReleasePtr(PSkySphere);
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

	void PScene::AddStaticMeshFromFbxFile(std::string MaterialName)
	{
		AddFromFbxNode(PAssetManager::GetSingleton().GetFbxScene()->GetRootNode(), MaterialName);
	}


	void PScene::AddFromFbxNode(FbxNode* Node, std::string MaterialName)
	{
		if (Node->GetNodeAttribute())
		{
			switch (Node->GetNodeAttribute()->GetAttributeType())
			{
			case FbxNodeAttribute::eMesh:
			{
				for(int index = 0; index < Node->GetSrcObjectCount(); index++)
				{
					const char* nodeName = Node->GetName();
					const char* srcName = Node->GetSrcObject(index)->GetName();
					auto x = std::string(Node->GetSrcObject(index)->GetTypeName());
					if(std::string(Node->GetSrcObject(index)->GetTypeName()) == "Mesh")
					{
						FbxDouble3 translation = Node->LclTranslation.Get();
						glm::vec3 Translation(translation[0] / 100, translation[1] / 100, translation[2] / 100);
						FbxDouble3 rotation = Node->LclRotation.Get();
						glm::vec3 Rotation(rotation[0], rotation[1], rotation[2]);
						FbxDouble3 scaling = Node->LclScaling.Get();
						glm::vec3 Scale(scaling[0], scaling[1], scaling[2]);
						Transform Trans(Translation, Rotation, Scale);
						AddStaticMesh(std::string(srcName), Trans, MaterialName);
						break;
					}
				}
			}
			break;
			}

		}

		for (int i = 0; i < Node->GetChildCount(); ++i)
		{
			AddFromFbxNode(Node->GetChild(i), MaterialName);
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


	void PScene::AddSkySphere(const std::string FilePath, PTexture* SkyTexture)
	{
		if (!PRender) PRender = PRenderThread::Get();
		assert(PRender);
		std::vector<std::pair<std::string, Transform>> StaticMeshTransformList;
		DeserilizeActorJsonFile(FilePath, StaticMeshTransformList);

		PStaticMesh* StaticMesh = PAssetManager::GetSingleton().GetMeshData(StaticMeshTransformList[0].first);
		PSkySphere = PNodeFactory::CreateNode<PNodeStaticMesh>(nullptr, "Sky");

		StaticMesh->BindNodeStaticMesh(PSkySphere);
		PSkySphere->BindLinkedStaticMesh(StaticMesh);
		PSkySphere->SetStaticMeshName(StaticMeshTransformList[0].first);
		PSkySphere->SetTransform(StaticMeshTransformList[0].second);
		PTask* task = CreateTask(PTask, PRender->GetRenderScene()->AddSkySphere(PSkySphere, PSkySphere->GetTransform(), SkyTexture));
		PRender->AddTask(task);
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