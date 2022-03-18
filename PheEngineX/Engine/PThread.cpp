#include "pch.h"
#include "PThread.h"

namespace Phe
{
	PThread::PThread() : IsRunning(false), pThread(nullptr)
	{

	}

	PThread::~PThread()
	{
		Stop();
	}

	void PThread::Start()
	{
		if (IsRunning)
		{
			return;
		}

		IsRunning = true;

		pThread = new std::thread(PThread::ThreadExecute, this);
	}

	void PThread::Stop()
	{
		if (pThread != nullptr)
		{
			IsRunning = false;
			pThread->join();
			delete pThread;
			pThread = nullptr;
		}
	}

	void* PThread::ThreadExecute(void* param)
	{
		PThread* t = (PThread*)param;

		t->OnThreadStart();

		while (t->IsRunning)
		{
			t->Run();
		}

		t->OnThreadEnd();

		return 0;
	}

}

