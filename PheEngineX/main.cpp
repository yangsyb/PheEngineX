#include "pch.h"
#include "Engine/Core/PEngine.h"
using namespace Phe;
#ifdef PlatformUndef
int main()
{
	//	_CrtSetBreakAlloc(108599);
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	PHashMap<KeyCode, PFunc<void()>> RegisterList;
	std::unique_ptr<PEngine> PheEngine = std::make_unique<PEngine>();
	//Load Test Data To AssetManager
	PAssetManager::GetSingleton().LoadJsonFile("JsonFile\\InSceneData.json");
	PAssetManager::GetSingleton().LoadJsonFile("JsonFile\\Tree.json");
	PAssetManager::GetSingleton().AddMeshData("box", StandardBoxVertices, StandardBoxIndices, StandardBoxNormal, StandardBoxUVs);
	//Texture Data
	PAssetManager::GetSingleton().AddTextureData("Texture1", L"Textures\\jacket_diff.dds");
	PAssetManager::GetSingleton().AddTextureData("Texture1Normal", L"Textures\\jacket_norm.dds");
	PAssetManager::GetSingleton().AddTextureData("Texture2", L"Textures\\grass.dds");
	PAssetManager::GetSingleton().AddTextureData("Texture3", L"Textures\\TreeTrunk.dds");
	PAssetManager::GetSingleton().AddTextureData("Texture3Normal", L"Textures\\TreeTrunkNormal.dds");
	//Material
	PAssetManager::GetSingleton().AddMaterialData("DefaultMat", "DefaultShader", std::vector<std::string>{"Texture1", "Texture1Normal"});
	PAssetManager::GetSingleton().AddMaterialData("WPOTreeLeafMat", "WPOShader", std::vector<std::string>{"Texture2"});
	PAssetManager::GetSingleton().AddMaterialData("WPOTreeTruckMat", "WPOTrunkShader", std::vector<std::string>{"Texture3", "Texture3Normal"});
	//Light
	PAssetManager::GetSingleton().AddLightData("DefaultLight");

	PFunc<void()> f1 = [&]() {
		PEngine::GetSingleton().GetScene()->ClearScene();
		PEngine::GetSingleton().GetScene()->AddStaticMesh("box", Transform{ glm::vec3(0,0, 0), glm::vec3(0,0,0), glm::vec3(1,1,1) }, "DefaultMat");
	};
	PFunc<void()> f2 = [&]() {
		PEngine::GetSingleton().GetScene()->ClearScene();
		PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("JsonFile\\InSceneData.json", "DefaultMat");
	};
	PFunc<void()> f3 = [&]() {
		PEngine::GetSingleton().GetScene()->ClearScene();
		PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("JsonFile\\InSceneData.json", "DefaultMat");
		PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("JsonFile\\Tree.json", "WPOTreeTruckMat");
		PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("JsonFile\\Tree.json", "WPOTreeLeafMat");
	};
	PFunc<void()> f4 = [&]() {
		Transform LightTransform(glm::vec3(-2.f, 55.f, 36.f), glm::vec3(0.f, -45.f, -90.f), glm::vec3(1.f, 1.f, 1.f));
		PEngine::GetSingleton().GetScene()->AddLight("DefaultLight", LightTransform);
	};
	PFunc<void()> f5 = [&]() {
		PEngine::GetSingleton().GetScene()->SetLightDynamic();
	};
	//Register Key
	RegisterList.insert({ KeyCode::Keypad1, f1 });
	RegisterList.insert({ KeyCode::Keypad2, f2 });
	RegisterList.insert({ KeyCode::Keypad3, f3 });
	RegisterList.insert({ KeyCode::Keypad4, f4 });
	RegisterList.insert({ KeyCode::Keypad5, f5 });

	PEditor::GetSingleton().Register(RegisterList);
	PheEngine->Start();
	PheEngine->Shutdown();

	return 0;
}
#elif defined(PlatformWin32)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//	_CrtSetBreakAlloc(108599);
	//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	PHashMap<KeyCode, PFunc<void()>> RegisterList;
	std::unique_ptr<PEngine> PheEngine = std::make_unique<PEngine>();
	//Load Test Data To AssetManager
	PAssetManager::GetSingleton().LoadJsonFile("JsonFile\\InSceneData.json");
	PAssetManager::GetSingleton().LoadJsonFile("JsonFile\\Tree.json");
	PAssetManager::GetSingleton().AddMeshData("box", StandardBoxVertices, StandardBoxIndices, StandardBoxNormal, StandardBoxTangent, StandardBoxUVs);
	//Texture Data
	PAssetManager::GetSingleton().AddTextureData("Texture1", L"Textures\\jacket_diff.dds");
	PAssetManager::GetSingleton().AddTextureData("Texture1Normal", L"Textures\\jacket_norm.dds");
	PAssetManager::GetSingleton().AddTextureData("Texture2", L"Textures\\grass.dds");
	PAssetManager::GetSingleton().AddTextureData("Texture3", L"Textures\\TreeTrunk.dds");
	PAssetManager::GetSingleton().AddTextureData("Texture3Normal", L"Textures\\TreeTrunkNormal.dds");
	//Material
	PAssetManager::GetSingleton().AddMaterialData("DefaultMat", "DefaultShader", std::vector<std::string>{"Texture1", "Texture1Normal"});
	PAssetManager::GetSingleton().AddMaterialData("WPOTreeLeafMat", "WPOShader", std::vector<std::string>{"Texture2"});
	PAssetManager::GetSingleton().AddMaterialData("WPOTreeTruckMat", "WPOTrunkShader", std::vector<std::string>{"Texture3", "Texture3Normal"});
	//Light
	PAssetManager::GetSingleton().AddLightData("DefaultLight");

	PFunc<void()> f1 = [&]() {
		PEngine::GetSingleton().GetScene()->ClearScene();
		PEngine::GetSingleton().GetScene()->AddStaticMesh("box", Transform{ glm::vec3(0,0, 0), glm::vec3(0,0,0), glm::vec3(1,1,1) }, "DefaultMat");
	};
	PFunc<void()> f2 = [&]() {
		PEngine::GetSingleton().GetScene()->ClearScene();
		PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("JsonFile\\InSceneData.json", "DefaultMat");
	};
	PFunc<void()> f3 = [&]() {
		PEngine::GetSingleton().GetScene()->ClearScene();
		PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("JsonFile\\InSceneData.json", "DefaultMat");
		PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("JsonFile\\Tree.json", "WPOTreeTruckMat");
		PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("JsonFile\\Tree.json", "WPOTreeLeafMat");
	};
	PFunc<void()> f4 = [&]() {
		Transform LightTransform(glm::vec3(-2.f, 55.f, 36.f), glm::vec3(0.f, -45.f, -90.f), glm::vec3(1.f, 1.f, 1.f));
		PEngine::GetSingleton().GetScene()->AddLight("DefaultLight", LightTransform);
	};
	PFunc<void()> f5 = [&]() {
		PEngine::GetSingleton().GetScene()->SetLightDynamic();
	};
	//Register Key
	RegisterList.insert({ KeyCode::Keypad1, f1 });
	RegisterList.insert({ KeyCode::Keypad2, f2 });
	RegisterList.insert({ KeyCode::Keypad3, f3 });
	RegisterList.insert({ KeyCode::Keypad4, f4 });
	RegisterList.insert({ KeyCode::Keypad5, f5 });

	PEditor::GetSingleton().Register(RegisterList);
	PheEngine->Start();
	PheEngine->Shutdown();

	return 0;
}

#elif defined(PlatformIOS)


#elif defined(PlatformAndroid)


#endif