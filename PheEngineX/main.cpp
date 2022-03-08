#include "pch.h"
#include "PEngine.h"

std::function<void()> f1 = [](){
	Phe::PEngine::GetSingleton().GetScene()->ClearScene();
	std::vector<float> Vertices1{ 0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5 };
	std::vector<UINT32> Indices1{ 0, 1, 2, 1, 3, 2, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23 };
	std::vector<float> Normal1{ 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0 };

	std::shared_ptr<Phe::PStaticMesh> mesh1 = std::make_shared<Phe::PStaticMesh>("box", Vertices1, Indices1, Normal1);
	Phe::PEngine::GetSingleton().GetScene()->AddStaticMesh(mesh1, Transform{ glm::vec3(-50,0, 0), glm::vec3(0,0,0), glm::vec3(1,1,1) });
};
std::function<void()> f2 = [](){
	Phe::PEngine::GetSingleton().GetScene()->ClearScene();
	Phe::PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("InSceneData.json");
};
std::function<void()> f3 = []() {
	Phe::PEngine::GetSingleton().GetScene()->ClearScene();
	Phe::PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("InSceneData.json");
	Phe::PEngine::GetSingleton().GetScene()->AddStaticMeshWPOFromFile("Tree.json");
};

#ifdef PlatformUndef
int main()
{
	std::vector<std::function<void()>> RegisterList;
	std::unique_ptr<Phe::PEngine> PheEngine = std::make_unique<Phe::PEngine>();

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