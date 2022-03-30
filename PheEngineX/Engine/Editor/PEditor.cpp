#include "pch.h"
#include "PEditor.h"
#include "Engine/Core/PEngine.h"

namespace Phe
{

	PEditor::PEditor()
	{

	}

	PEditor::~PEditor()
	{

	}

	void PEditor::Register(std::vector<std::function<void()>> RegisterList)
	{
		for (auto func : RegisterList)
		{
			EditorEvent.push_back(func);
		}
	}

	void PEditor::Update()
	{
		if (PEngine::GetSingleton().GetWindow()->GetInput()->GetKeyDown(KeyCode::Keypad1))
		{
			if (EditorEvent.size() > 0)
			{
				EditorEvent[0]();
			}
		}

		else if (PEngine::GetSingleton().GetWindow()->GetInput()->GetKeyDown(KeyCode::Keypad2))
		{
			if (EditorEvent.size() > 1)
			{
				EditorEvent[1]();
			}
		}

		else if (PEngine::GetSingleton().GetWindow()->GetInput()->GetKeyDown(KeyCode::Keypad3))
		{
			if (EditorEvent.size() > 2)
			{
				EditorEvent[2]();
			}
		}

		else if (PEngine::GetSingleton().GetWindow()->GetInput()->GetKeyDown(KeyCode::Keypad4))
		{
			if (EditorEvent.size() > 3)
			{
				EditorEvent[3]();
			}
		}

		else if (PEngine::GetSingleton().GetWindow()->GetInput()->GetKeyDown(KeyCode::Keypad5))
		{
			if (EditorEvent.size() > 4)
			{
				EditorEvent[4]();
			}
		}
		else if(PEngine::GetSingleton().GetWindow()->GetInput()->GetKeyDown(KeyCode::J))
		{
			PTask* task = CreateTask(PTask, PRenderThread::Get()->GetRenderer()->SetExportDepth());
			PRenderThread::Get()->AddTask(task);
		}
	}

}