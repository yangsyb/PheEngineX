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
		PShaderManager::CreateShaderManager();
		PRenderThread::CreateRenderThread();
		PMainEditor = new PEditor();
		PMainAssetManager = std::make_unique<PAssetManager>();
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
		PWindow::DestroyWindow(PheWindow);
		ReleasePtr(PMainEditor);
		ReleasePtr(PMainScene);
		PMainAssetManager = nullptr;
		PShaderManager::DestroyShaderManager();
		PRenderThread::Get()->Stop();
	}

	void PEngine::Tick()
	{
		BeginFrame();
		EndFrame();
	}

	void PEngine::BeginFrame()
	{
		PTimer.Tick();
		PMainEditor->Update();
		PMainScene->Update();
	}

	void PEngine::EndFrame()
	{
		PRenderThread* RenderThread = PRenderThread::Get();
		RenderThread->TriggerRender();
		while (RenderThread->GetRenderNum() > 0)
		{
			std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		}
	}

}

