#include "pch.h"
#include "PEngine.h"
#include "PInput.h"

namespace Phe
{
	PEngine::PEngine(UINT32 width, UINT32 height) : PWidth(width), PHeight(height), IsRunning(false), PCurrentPlatform(Undefined), PMainScene(nullptr), PRender(nullptr)
	{
#ifdef PlatformWin32
		PCurrentPlatform = Win32;
#elif defined(PlatformIOS)
		PCurrentPlatform = IOS;
#elif defined(PlatformAndroid)
		PCurrentPlatform = Android;
#endif
		PheWindow = PWindow::Create("PheEngine", PWidth, PHeight);
		PRenderThread::CreateRenderThread();
		PMainEditor = new PEditor();
	}


	PEngine::~PEngine()
	{
		PRenderThread::Get()->Stop();
		PWindow::DestroyWindow(PheWindow);
		delete PMainEditor;
		PMainEditor = nullptr;
		delete PMainScene;
		PMainScene = nullptr;
	}



	void PEngine::Start()
	{
		PMainScene = new PScene();
		PTimer.Reset();
		PTimer.Start();
		IsRunning = true;
		while(IsRunning && PheWindow->Run())
		{
			Tick();
		}

	}

	void PEngine::Shutdown()
	{

	}

	void PEngine::Tick()
	{
		BeginFrame();
		EndFrame();
	}

	void PEngine::BeginFrame()
	{
		PTimer.Tick();
		PTask* task = new PTask([=]() {PRenderThread::Get()->SetCurrentTotalTime(PTimer.TotalTime()); });
		PRenderThread::Get()->AddTask(task);
		Input::Update();
		PMainEditor->Update();
		PMainScene->Update();
	}

	void PEngine::EndFrame()
	{
		PRenderThread* renderthread = PRenderThread::Get();
		while(renderthread->GetRenderNum()>=0)
		{
			std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		}
		renderthread->TriggerRender();
	}

}

