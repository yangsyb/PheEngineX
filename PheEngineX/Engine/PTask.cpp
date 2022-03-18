#include "pch.h"
#include "PTask.h"

namespace Phe
{
	PTask::PTask(const std::function<void()>& Func) : ExecuteFunc(Func)
	{

	}

	PTask::~PTask()
	{

	}

	void PTask::Execute()
	{
		if (ExecuteFunc)
		{
			ExecuteFunc();
		}
	}

}