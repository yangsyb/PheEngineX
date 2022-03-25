#include "pch.h"
#include "Engine/PEngine.h"
#include "Engine/PShader.h"
#include "Engine/PMaterial.h"


#ifdef PlatformUndef
int main()
{
	std::vector<std::function<void()>> RegisterList;
	std::unique_ptr<Phe::PEngine> PheEngine = std::make_unique<Phe::PEngine>();
	//Load Test Data To AssetManager
	Phe::PAssetManager::GetSingleton().LoadJsonFile("JsonFile\\InSceneData.json");
	Phe::PAssetManager::GetSingleton().LoadJsonFile("JsonFile\\Tree.json");
	PMeshDataStruct Data;
	Data.Vertices = { 0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5 };
	Data.Indices = { 0, 1, 2, 1, 3, 2, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23 };
	Data.Normal = { 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0 };
	Data.UVs = { 0.67529296875, 0.984375, 0.98974609375, 0.984375, 0.67529296875, 0.669921875, 0.98974609375, 0.669921875, 0.329833984375, 0.669921875, 0.01537322998046875, 0.669921875, 0.01537322998046875, 0.984375, 0.329833984375, 0.984375, 0.329833984375, 0.33984375, 0.01537322998046875, 0.33984375, 0.01537322998046875, 0.654296875, 0.329833984375, 0.654296875, 0.65966796875, 0.669921875, 0.34521484375, 0.669921875, 0.34521484375, 0.984375, 0.65966796875, 0.984375, 0.329833984375, 0.01001739501953125, 0.01537322998046875, 0.01001739501953125, 0.01537322998046875, 0.324462890625, 0.329833984375, 0.324462890625, 0.65966796875, 0.33984375, 0.34521484375, 0.33984375, 0.34521484375, 0.654296875, 0.65966796875, 0.654296875 };
	Phe::PAssetManager::GetSingleton().AddMeshData("box", Data);

	//Texture Data
	Phe::PAssetManager::GetSingleton().AddTextureData("Texture1", L"Textures\\jacket_diff.dds");
	Phe::PAssetManager::GetSingleton().AddTextureData("Texture1Normal", L"Textures\\jacket_norm.dds");
	Phe::PAssetManager::GetSingleton().AddTextureData("Texture2", L"Textures\\grass.dds");
	Phe::PAssetManager::GetSingleton().AddTextureData("Texture3", L"Textures\\TreeTrunk.dds");
	Phe::PAssetManager::GetSingleton().AddTextureData("Texture3Normal", L"Textures\\TreeTrunkNormal.dds");

	//Material
	Phe::PAssetManager::GetSingleton().AddMaterialData("DefaultMat", "DefaultShader", std::vector<std::string>{"Texture1", "Texture1Normal"});
	Phe::PAssetManager::GetSingleton().AddMaterialData("WPOTreeLeafMat", "WPOShader", std::vector<std::string>{"Texture2"});
	Phe::PAssetManager::GetSingleton().AddMaterialData("WPOTreeTruckMat", "WPOTrunkShader", std::vector<std::string>{"Texture3", "Texture3Normal"});


	std::function<void()> f1 = [&]() {
		Phe::PEngine::GetSingleton().GetScene()->ClearScene();
		Phe::PEngine::GetSingleton().GetScene()->AddStaticMesh("box", Transform{ glm::vec3(0,0, 0), glm::vec3(0,0,0), glm::vec3(1,1,1) }, "DefaultMat");
	};
	std::function<void()> f2 = [&]() {
		Phe::PEngine::GetSingleton().GetScene()->ClearScene();
		Phe::PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("JsonFile\\InSceneData.json", "DefaultMat");
	};
	std::function<void()> f3 = [&]() {
		Phe::PEngine::GetSingleton().GetScene()->ClearScene();
		Phe::PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("JsonFile\\InSceneData.json", "DefaultMat");
		Phe::PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("JsonFile\\Tree.json", "WPOTreeTruckMat");
		Phe::PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("JsonFile\\Tree.json", "WPOTreeLeafMat");
	};
	//Register Key
	RegisterList.push_back(f1);

	RegisterList.push_back(f2);

	RegisterList.push_back(f3);

	Phe::PEditor::GetSingleton().Register(RegisterList);
	PheEngine->Start();
	PheEngine->Shutdown();

	return 0;
}
#elif defined(PlatformWin32)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	std::vector<std::function<void()>> RegisterList;
	std::unique_ptr<Phe::PEngine> PheEngine = std::make_unique<Phe::PEngine>();
	//Load Test Data To AssetManager
	Phe::PAssetManager::GetSingleton().LoadJsonFile("JsonFile\\InSceneData.json");
	Phe::PAssetManager::GetSingleton().LoadJsonFile("JsonFile\\Tree.json");
	PMeshDataStruct Data;
	Data.Vertices = { 0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5 };
	Data.Indices = { 0, 1, 2, 1, 3, 2, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23 };
	Data.Normal = { 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0 };
	Data.UVs = { 0.67529296875, 0.984375, 0.98974609375, 0.984375, 0.67529296875, 0.669921875, 0.98974609375, 0.669921875, 0.329833984375, 0.669921875, 0.01537322998046875, 0.669921875, 0.01537322998046875, 0.984375, 0.329833984375, 0.984375, 0.329833984375, 0.33984375, 0.01537322998046875, 0.33984375, 0.01537322998046875, 0.654296875, 0.329833984375, 0.654296875, 0.65966796875, 0.669921875, 0.34521484375, 0.669921875, 0.34521484375, 0.984375, 0.65966796875, 0.984375, 0.329833984375, 0.01001739501953125, 0.01537322998046875, 0.01001739501953125, 0.01537322998046875, 0.324462890625, 0.329833984375, 0.324462890625, 0.65966796875, 0.33984375, 0.34521484375, 0.33984375, 0.34521484375, 0.654296875, 0.65966796875, 0.654296875 };
	Phe::PAssetManager::GetSingleton().AddMeshData("box", Data);

	//Texture Data
	Phe::PAssetManager::GetSingleton().AddTextureData("Texture1", L"Textures\\jacket_diff.dds");
	Phe::PAssetManager::GetSingleton().AddTextureData("Texture1Normal", L"Textures\\jacket_norm.dds");
	Phe::PAssetManager::GetSingleton().AddTextureData("Texture2", L"Textures\\grass.dds");
	Phe::PAssetManager::GetSingleton().AddTextureData("Texture3", L"Textures\\TreeTrunk.dds");
	Phe::PAssetManager::GetSingleton().AddTextureData("Texture3Normal", L"Textures\\TreeTrunkNormal.dds");

	//Material
	Phe::PAssetManager::GetSingleton().AddMaterialData("DefaultMat", "DefaultShader", std::vector<std::string>{"Texture1", "Texture1Normal"});
	Phe::PAssetManager::GetSingleton().AddMaterialData("WPOTreeLeafMat", "WPOShader", std::vector<std::string>{"Texture2"});
	Phe::PAssetManager::GetSingleton().AddMaterialData("WPOTreeTruckMat", "WPOTrunkShader", std::vector<std::string>{"Texture3", "Texture3Normal"});


	std::function<void()> f1 = [&]() {
		Phe::PEngine::GetSingleton().GetScene()->ClearScene();
		Phe::PEngine::GetSingleton().GetScene()->AddStaticMesh("box", Transform{ glm::vec3(0,0, 0), glm::vec3(0,0,0), glm::vec3(1,1,1) }, "DefaultMat");
	};
	std::function<void()> f2 = [&]() {
		Phe::PEngine::GetSingleton().GetScene()->ClearScene();
		Phe::PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("JsonFile\\InSceneData.json", "DefaultMat");
	};
	std::function<void()> f3 = [&]() {
		Phe::PEngine::GetSingleton().GetScene()->ClearScene();
		Phe::PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("JsonFile\\InSceneData.json", "DefaultMat");
		Phe::PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("JsonFile\\Tree.json", "WPOTreeTruckMat");
		Phe::PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("JsonFile\\Tree.json", "WPOTreeLeafMat");
	};
	std::function<void()> f4 = [&]() {
		PLightDataStruct LightData;
		LightData.Position = glm::vec3(-1.4f, 52.3f, 35.7f);
		LightData.Rotation = glm::vec3(0.f, -45.f, -90.f);
		LightData.Scale = glm::vec3(1.f, 1.f, 1.f);
		LightData.LightRadius = 1.f;
		LightData.LightStrength = 1.f;
		Phe::PEngine::GetSingleton().GetScene()->AddLight("MainLight", LightData);
	};
	std::function<void()> f5 = [&]() {
		Phe::PEngine::GetSingleton().GetScene()->SetLightDynamic("MainLight");
	};
	//Register Key
	RegisterList.push_back(f1);

	RegisterList.push_back(f2);

	RegisterList.push_back(f3);

	RegisterList.push_back(f4);

	RegisterList.push_back(f5);

	Phe::PEditor::GetSingleton().Register(RegisterList);
	PheEngine->Start();
	PheEngine->Shutdown();

	return 0;
}

#elif defined(PlatformIOS)


#elif defined(PlatformAndroid)


#endif