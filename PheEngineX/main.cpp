#include "pch.h"
#include "PEngine.h"
#include "RHI/PShader.h"
#include "RHI/PMaterial.h"


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

#elif defined(PlatformIOS)


#elif defined(PlatformAndroid)


#endif