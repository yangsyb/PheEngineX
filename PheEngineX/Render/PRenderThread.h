#pragma once
#include "pch.h"
#include "Engine/Core/PThread.h"
#include "Engine/Core/PTask.h"
#include "PRenderScene.h"
#include "PRenderer.h"
namespace Phe
{
	class PRenderThread : public PThread
	{
	public:
		static void CreateRenderThread();
		static void DestroyRenderThread();
		static PRenderThread* Get();

		PRenderThread();
		virtual ~PRenderThread();

		virtual void Start() override;
		virtual void Stop() override;
		virtual void Run() override;
		virtual void OnThreadStart();
		void AddTask(PTask* RenderTask);
		void DoTasks();
		void TriggerRender();
		int GetRenderNum() { return PRenderNum; }
		PRenderScene* GetRenderScene() { return PRScene; }
		PRenderer* GetRenderer() { return PMainRenderer; }

		void SetCurrentTotalTime(float TotalTime);
		float GetCurrentTotalTime() { return PTotalTime; }
	public:
		void Render();

	protected:
		std::mutex Rendermtx;
		std::condition_variable RenderCV;

		struct PRenderFrame
		{
			std::queue<PTask*> PTaskQueue;
		};
		UINT32 RenderFrameIndex;
		UINT32 NextFrameIndex;
		PRenderFrame RenderFrame[2];

	private:
		static PRenderThread* pRenderThread;
		PRenderScene* PRScene;
		PRenderer* PMainRenderer;
		int PRenderNum = 0;
		float PTotalTime = 0;
	};
}