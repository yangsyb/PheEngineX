#pragma once
#include "pch.h"

namespace Phe
{
	class PThread
	{
	public:
		PThread();
		virtual ~PThread();

		virtual void Start();
		virtual void Stop();

		virtual void Run() = 0;
		virtual void OnThreadStart() {};
		virtual void OnThreadEnd() {};

		virtual bool GetIsThreadRunning()
		{
			return IsRunning;
		}

		static void* ThreadExecute(void* param);

	protected:
		bool IsRunning;
		std::thread* pThread;

	};
}