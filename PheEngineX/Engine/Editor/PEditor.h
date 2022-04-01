#pragma once
#include "pch.h"
#include "Engine/Core/PInput.h"

namespace Phe
{
	class PEditor : public Singleton<PEditor>
	{
	public:
		PEditor();
		~PEditor();
		void Register(PHashMap<KeyCode, PFunc<void()>> KeyEvents);
		void Register(KeyCode Key, PFunc<void()> KeyEvent);
		void Update();

	private:
		std::vector<std::function<void()>> EditorEvent;
		PHashMap<KeyCode, PFunc<void()>> EditorEventTable;
	};
}