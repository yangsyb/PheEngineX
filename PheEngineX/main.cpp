#include "pch.h"
#include "PEngine.h"
#include "RHI/PShader.h"
#include "RHI/PMaterial.h"

//#define PlatformUndef

#ifdef PlatformUndef
int main()
{
	std::vector<std::function<void()>> RegisterList;
	std::unique_ptr<Phe::PEngine> PheEngine = std::make_unique<Phe::PEngine>();
	//Load Test Data To AssetManager
	Phe::PAssetManager::GetSingleton().LoadJsonFile("InSceneData.json");
	Phe::PAssetManager::GetSingleton().LoadJsonFile("Tree.json");
	PMeshDataStruct Data;
	Data.Vertices = { 0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5 };
	Data.Indices = { 0, 1, 2, 1, 3, 2, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23 };
	Data.Normal = { 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0 };
	Phe::PAssetManager::GetSingleton().AddMeshData("box", Data);

	std::shared_ptr<Phe::PShader> DefaultShader = std::make_shared<Phe::PShader>("DefaultShader", L"Shaders\\color.hlsl");
	std::shared_ptr<Phe::PShader> WPOShader = std::make_shared<Phe::PShader>("WPOShader", L"Shaders\\tree.hlsl");
	Phe::PShaderManager::GetSingleton().AddShader(DefaultShader);
	Phe::PShaderManager::GetSingleton().AddShader(WPOShader);
	std::shared_ptr<Phe::PMaterial> DefaultMaterial = std::make_shared<Phe::PMaterial>("DefaultMat", DefaultShader);
	std::shared_ptr<Phe::PMaterial> WPOMaterial = std::make_shared<Phe::PMaterial>("WPOMat", WPOShader);

	std::function<void()> f1 = [&]() {
		Phe::PEngine::GetSingleton().GetScene()->ClearScene();
		Phe::PEngine::GetSingleton().GetScene()->AddStaticMesh("box", Transform{ glm::vec3(0,0, 0), glm::vec3(0,0,0), glm::vec3(1,1,1) }, DefaultMaterial);
	};
	std::function<void()> f2 = [&]() {
		Phe::PEngine::GetSingleton().GetScene()->ClearScene();
		Phe::PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("InSceneData.json", DefaultMaterial);
	};
	std::function<void()> f3 = [&]() {
		Phe::PEngine::GetSingleton().GetScene()->ClearScene();
		Phe::PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("InSceneData.json", DefaultMaterial);
		Phe::PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("Tree.json", WPOMaterial);
	};
	Phe::PShaderManager::GetSingleton().CompileAllShader();
	//Register Key
	RegisterList.push_back(f1);

	RegisterList.push_back(f2);

	RegisterList.push_back(f3);

	Phe::PEditor::GetSingleton().Register(RegisterList);
	PheEngine->Start();
	return 0;
}
#elif defined(PlatformWin32)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	std::vector<std::function<void()>> RegisterList;
	std::unique_ptr<Phe::PEngine> PheEngine = std::make_unique<Phe::PEngine>();
	//Load Test Data To AssetManager
	Phe::PAssetManager::GetSingleton().LoadJsonFile("InSceneData.json");
	Phe::PAssetManager::GetSingleton().LoadJsonFile("Tree.json");
	PMeshDataStruct Data;
	Data.Vertices = { 0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5 };
	Data.Indices = { 0, 1, 2, 1, 3, 2, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23 };
	Data.Normal = { 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0 };
	Data.UVs = { 0.67529296875, 0.984375, 0.98974609375, 0.984375, 0.67529296875, 0.669921875, 0.98974609375, 0.669921875, 0.329833984375, 0.669921875, 0.01537322998046875, 0.669921875, 0.01537322998046875, 0.984375, 0.329833984375, 0.984375, 0.329833984375, 0.33984375, 0.01537322998046875, 0.33984375, 0.01537322998046875, 0.654296875, 0.329833984375, 0.654296875, 0.65966796875, 0.669921875, 0.34521484375, 0.669921875, 0.34521484375, 0.984375, 0.65966796875, 0.984375, 0.329833984375, 0.01001739501953125, 0.01537322998046875, 0.01001739501953125, 0.01537322998046875, 0.324462890625, 0.329833984375, 0.324462890625, 0.65966796875, 0.33984375, 0.34521484375, 0.33984375, 0.34521484375, 0.654296875, 0.65966796875, 0.654296875 };
	Phe::PAssetManager::GetSingleton().AddMeshData("box", Data);

	std::shared_ptr<Phe::PShader> DefaultShader = std::make_shared<Phe::PShader>("DefaultShader", L"Shaders\\color.hlsl");
	std::shared_ptr<Phe::PShader> WPOShader = std::make_shared<Phe::PShader>("WPOShader", L"Shaders\\tree.hlsl");
	std::shared_ptr<Phe::PShader> WPOTrunkShader = std::make_shared<Phe::PShader>("WPOTrunkShader", L"Shaders\\treetrunk.hlsl");
	Phe::PShaderManager::GetSingleton().AddShader(DefaultShader);
	Phe::PShaderManager::GetSingleton().AddShader(WPOShader);
	Phe::PShaderManager::GetSingleton().AddShader(WPOTrunkShader);

	Phe::PAssetManager::GetSingleton().AddTextureData("Texture1", L"Textures\\jacket_diff.dds");
	//	Phe::PAssetManager::GetSingleton().AddTextureData("Texture1", L"Textures\\stone.dds");
	Phe::PAssetManager::GetSingleton().AddTextureData("Texture2", L"Textures\\grass.dds");
	Phe::PAssetManager::GetSingleton().AddTextureData("Texture3", L"Textures\\TreeTrunk.dds");
	Phe::PAssetManager::GetSingleton().AddMaterialData("DefaultMat", "DefaultShader", "Texture1");
	Phe::PAssetManager::GetSingleton().AddMaterialData("WPOTreeLeafMat", "WPOShader", "Texture2");
	Phe::PAssetManager::GetSingleton().AddMaterialData("WPOTreeTruckMat", "WPOTrunkShader", "Texture3");

	//	std::shared_ptr<Phe::PMaterial> DefaultMaterial = std::make_shared<Phe::PMaterial>("DefaultMat", DefaultShader, "test");
	//	std::shared_ptr<Phe::PMaterial> WPOMaterial = std::make_shared<Phe::PMaterial>("WPOMat", WPOShader, "test");

	std::function<void()> f1 = [&]() {
		Phe::PEngine::GetSingleton().GetScene()->ClearScene();
		Phe::PEngine::GetSingleton().GetScene()->AddStaticMesh("box", Transform{ glm::vec3(0,0, 0), glm::vec3(0,0,0), glm::vec3(1,1,1) }, "DefaultMat");
	};
	std::function<void()> f2 = [&]() {
		Phe::PEngine::GetSingleton().GetScene()->ClearScene();
		Phe::PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("InSceneData.json", "DefaultMat");
	};
	std::function<void()> f3 = [&]() {
		Phe::PEngine::GetSingleton().GetScene()->ClearScene();
		Phe::PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("InSceneData.json", "DefaultMat");
		Phe::PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("Tree.json", "WPOTreeLeafMat");
		Phe::PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("Tree.json", "WPOTreeTruckMat");
	};
	Phe::PShaderManager::GetSingleton().CompileAllShader();
	//Register Key
	RegisterList.push_back(f1);

	RegisterList.push_back(f2);

	RegisterList.push_back(f3);

	Phe::PEditor::GetSingleton().Register(RegisterList);
	PheEngine->Start();
	PheEngine->Shutdown();
	return 0;
}

#elif defined(PlatformIOS)


#elif defined(PlatformAndroid)


#endif