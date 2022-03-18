#pragma once

namespace Phe
{
	class PTask
	{
	public:
		PTask(const std::function<void()>& Func);
		virtual ~PTask();

		virtual void Execute();

	protected:
		typedef std::function<void()> TaskFunc;
		TaskFunc ExecuteFunc;
	};
}