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
		void Register(std::vector<std::function<void()>> RegisterList);
		void Update();

	private:
		std::vector<std::function<void()>> EditorEvent;
	};
}