#include "pch.h"
#include "PEditor.h"
#include "Engine/Core/PEngine.h"

namespace Phe
{

	PEditor::PEditor()
	{
		PFunc<void()> Func = []() 
		{
		PTask* task = CreateTask(PTask, PRenderThread::Get()->GetRenderer()->SetExportDepth());
		PRenderThread::Get()->AddTask(task);
		};
		Register(KeyCode::J, Func);
	}

	PEditor::~PEditor()
	{

	}


	void PEditor::Register(KeyCode Key, PFunc<void()> KeyEvent)
	{
		EditorEventTable.insert({Key, KeyEvent});
	}

	void PEditor::Register(PHashMap<KeyCode, PFunc<void()>> KeyEvents)
	{
		for(auto KeyEvent : KeyEvents)
		{
			Register(KeyEvent.first, KeyEvent.second);
		}
	}

	void PEditor::Update()
	{
		for(auto Event : EditorEventTable)
		{
			if(PEngine::GetSingleton().GetWindow()->GetInput()->GetKeyDown(Event.first))
			{
				Event.second();
				return;
			}
		}
	}

}