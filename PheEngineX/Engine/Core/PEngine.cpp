#include "pch.h"
#include "PEngine.h"
#include "PInput.h"

namespace Phe
{
	PEngine::PEngine(UINT32 width, UINT32 height) : PWidth(width), PHeight(height), IsRunning(false), PCurrentPlatform(Platform::Undefined), PMainScene(nullptr)
	{
#ifdef PlatformWin32
		PCurrentPlatform = Platform::Win32;
#elif defined(PlatformIOS)
		PCurrentPlatform = Platform::IOS;
#elif defined(PlatformAndroid)
		PCurrentPlatform = Platform::Android;
#endif
		PheWindow = PWindow::Create("PheEngine", PWidth, PHeight);
		PRenderThread::CreateRenderThread();
		PMainEditor = new PEditor();
		PMainAssetManager = std::make_unique<PAssetManager>();
		PShaderManager::CreateShaderManager();
	}


	PEngine::~PEngine()
	{

	}



	void PEngine::Start()
	{
		PMainScene = new PScene();
		PTimer.Reset();
		PTimer.Start();
		IsRunning = true;
		while (IsRunning && PheWindow->Run())
		{
			Tick();
		}

	}

	void PEngine::Shutdown()
	{
		PRenderThread::Get()->Stop();
		PWindow::DestroyWindow(PheWindow);
		PShaderManager::DestroyShaderManager();
		ReleasePtr(PMainEditor);
		ReleasePtr(PMainScene);
	}

	void PEngine::Tick()
	{
		BeginFrame();
		EndFrame();
	}

	void PEngine::BeginFrame()
	{
		PTimer.Tick();
		PTask* task = CreateTask(PTask, PRenderThread::Get()->SetCurrentTotalTime(PTimer.TotalTime()));
		PRenderThread::Get()->AddTask(task);
		PMainEditor->Update();
		PMainScene->Update();
	}

	void PEngine::EndFrame()
	{
		PRenderThread* RenderThread = PRenderThread::Get();
		while (RenderThread->GetRenderNum() >= 0)
		{
			std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		}
		RenderThread->TriggerRender();
	}

}

