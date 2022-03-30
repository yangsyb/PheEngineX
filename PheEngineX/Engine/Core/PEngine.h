#pragma once
#include "pch.h"
#include "PWindow.h"
#include "Render/PRenderThread.h"
#include "PGameTimer.h"
#include "Engine/Scene/PScene.h"
#include "Engine/Editor/PEditor.h"
#include "Engine/Editor/PAssetManager.h"
#include "Engine/Editor/PShaderManager.h"

namespace Phe
{
	enum class Platform
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

		PWindow* GetWindow() { return PheWindow; }
		PScene* GetScene() { return PMainScene; }
		PGameTimer& GetTimer() { return PTimer; }
	private:
		UINT32 PWidth;
		UINT32 PHeight;
		bool IsRunning;
		Platform PCurrentPlatform;

		PWindow* PheWindow;
//		PRenderThread* PRender;

		PScene* PMainScene;
		PEditor* PMainEditor;
		PGameTimer PTimer;
		std::unique_ptr<PAssetManager> PMainAssetManager;
	};
}