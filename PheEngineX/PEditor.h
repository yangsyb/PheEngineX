#pragma once
#include "pch.h"
#include "PInput.h"

namespace Phe
{
	class PEditor
	{
	public:
		PEditor();
		~PEditor();
		void Update();

	private:
		std::vector<std::function<void()>> EditorEvent;
	};
}