#include "pch.h"
#include "PEngine.h"
#include "PInput.h"

namespace Phe
{
	PEngine::PEngine(UINT32 width, UINT32 height) : PWidth(width), PHeight(height), IsRunning(false), PCurrentPlatform(Undefined)
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
		Input::Update();
		PMainEditor->Update();
		PMainScene->Update();
	}

	void PEngine::EndFrame()
	{
		PRenderThread* renderthread = PRenderThread::Get();
		while(renderthread->GetRenderNum()>=1)
		{
			std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		}
		renderthread->TriggerRender();
	}

}

