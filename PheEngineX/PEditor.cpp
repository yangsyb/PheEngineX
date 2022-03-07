#include "pch.h"
#include "PEditor.h"
#include "PStaticMesh.h"
#include "PEngine.h"

namespace Phe
{

	PEditor::PEditor()
	{
		EditorEvent.push_back([]() {
			PEngine::GetSingleton().GetScene()->ClearScene();
			std::vector<float> Vertices1{ 0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5 };
			std::vector<UINT32> Indices1{ 0, 1, 2, 1, 3, 2, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23 };
			std::vector<float> Normal1{ 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0 };

			std::shared_ptr<PStaticMesh> mesh1 = std::make_shared<PStaticMesh>("box", Vertices1, Indices1, Normal1);
			PEngine::GetSingleton().GetScene()->AddStaticMesh(mesh1, Transform{ glm::vec3(-50,0, 0), glm::vec3(0,0,0), glm::vec3(1,1,1) });
			});


		EditorEvent.push_back([]() {
			PEngine::GetSingleton().GetScene()->ClearScene();
			PEngine::GetSingleton().GetScene()->AddStaticMeshFromFile("InSceneData.json");
		});

		EditorEvent.push_back([]() {});
	}

	PEditor::~PEditor()
	{

	}

	void PEditor::Update()
	{
		if (Input::GetKeyDown(KeyCode::Keypad1))
		{
			if (EditorEvent.size() > 0)
			{
				EditorEvent[0]();
			}
		}

		else if (Input::GetKeyDown(KeyCode::Keypad2))
		{
			if (EditorEvent.size() > 1)
			{
				EditorEvent[1]();
			}
		}

		else if (Input::GetKeyDown(KeyCode::Keypad3))
		{
			if (EditorEvent.size() > 2)
			{
				EditorEvent[2]();
			}
		}
	}

}