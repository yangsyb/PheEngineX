#include "pch.h"
#include "PRenderThread.h"
#include "Engine/PEngine.h"
#include "PRenderer.h"
#include "RHI/PRHI.h"
namespace Phe
{
	PRenderThread* PRenderThread::pRenderThread = nullptr;

	void PRenderThread::CreateRenderThread()
	{
		pRenderThread = new PRenderThread();
		pRenderThread->Start();
	}

	void PRenderThread::DestroyRenderThread()
	{
		pRenderThread->Stop();
	}

	PRenderThread* PRenderThread::Get()
	{
		return pRenderThread;
	}

	PRenderThread::PRenderThread() : PRenderNum(0), RenderFrameIndex(0), NextFrameIndex(0), PRScene(nullptr), PMainRenderer(nullptr)
	{

	}

	PRenderThread::~PRenderThread()
	{

	}

	void PRenderThread::Start()
	{
		if (IsRunning)
		{
			return;
		}

		IsRunning = true;

		pThread = new std::thread(PThread::ThreadExecute, this);
	}

	void PRenderThread::Stop()
	{
		if (pThread)
		{
			std::unique_lock<std::mutex> RenderLock(Rendermtx);
			++PRenderNum;
			NextFrameIndex = (NextFrameIndex + 1) % 2;
			IsRunning = false;
			RenderCV.notify_all();
			RenderLock.unlock();

			pThread->join();
			
			PMainRenderer->DestroyRenderer();
			delete PMainRenderer;
			PMainRenderer = nullptr;
			PRScene->DestroyRenderScene();
			delete PRScene;
			PRScene = nullptr;
			delete pThread;
			pThread = nullptr;
			delete pRenderThread;
			pRenderThread = nullptr;
			PRHI::DestroyRHI();
		}
	}

	void PRenderThread::Run()
	{
		std::unique_lock<std::mutex> RenderLock(Rendermtx);
		PRenderNum--;
		RenderCV.wait(RenderLock);
		DoTasks();
		Render();
	}

	void PRenderThread::OnThreadStart()
	{
		std::unique_lock<std::mutex> RenderLock(Rendermtx);
		
		PRHI::CreateRHI();
		PMainRenderer = new PRenderer();
		PRScene = new PRenderScene();
		PRenderNum = 0;
		PMainRenderer->Initialize();
	}

	void PRenderThread::AddTask(PTask* RenderTask)
	{
		if (IsRunning)
		{
			RenderFrame[NextFrameIndex].PTaskQueue.push(RenderTask);
		}
	}

	void PRenderThread::DoTasks()
	{
		PTask* CurrentTask = nullptr;
		while (!RenderFrame[RenderFrameIndex].PTaskQueue.empty())
		{
			CurrentTask = RenderFrame[RenderFrameIndex].PTaskQueue.front();
			CurrentTask->Execute();
			RenderFrame[RenderFrameIndex].PTaskQueue.pop();
			delete CurrentTask;
			CurrentTask = nullptr;
		}
		RenderFrameIndex = (RenderFrameIndex + 1) % 2;
	}

	void PRenderThread::TriggerRender()
	{
		std::unique_lock<std::mutex> RenderLock(Rendermtx);
		++PRenderNum;
		NextFrameIndex = (NextFrameIndex + 1) % 2;
		RenderCV.notify_one();
	}

	void PRenderThread::SetCurrentTotalTime(float TotalTime)
	{
		PTotalTime = TotalTime;
	}

	void PRenderThread::Render()
	{
		PMainRenderer->RenderFrameBegin(PRScene);
		PMainRenderer->Render(PRScene);
		PMainRenderer->RenderFrameEnd(PRScene);
	}

}