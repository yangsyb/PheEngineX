#pragma once
#include "pch.h"
#include "PWindow.h"
#include "PRenderThread.h"
#include "PGameTimer.h"
#include "PScene.h"
#include "PEditor.h"

namespace Phe
{
	enum Platform
	{
		Win32,
		IOS,
		Android,
		Undefined,
	};

	class PEngine : public Singleton<PEngine>
	{
	public:
		PEngine(UINT32 width = 1920, UINT32 height = 1080);
		~PEngine();

		void Start();
		void Shutdown();
		void Tick();

		void BeginFrame();
		void EndFrame();

		PScene* GetScene() { return PMainScene; }
	private:
		UINT32 PWidth;
		UINT32 PHeight;
		bool IsRunning;
		Platform PCurrentPlatform;

		PWindow* PheWindow;
		PRenderThread* PRender;

		PScene* PMainScene;
		PEditor* PMainEditor;
		PGameTimer PTimer;


	};
}