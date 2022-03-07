#include "pch.h"
#include "PEditor.h"
#include "PStaticMesh.h"
#include "PEngine.h"

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
		for(auto func : RegisterList)
		{
			EditorEvent.push_back(func);
		}
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