#pragma once
#include "pch.h"
#include "PThread.h"
#include "PStaticMesh.h"
#include "PTask.h"
#include "PRenderScene.h"
#include "GraphicsContext.h"
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

		void SetCurrentTotalTime(float TotalTime);
		float GetCurrentTotalTime() { return PTotalTime; }
	public:

		void Initialize();
		void Render();
		void OnResize();
	public:
		ID3D12Resource* CurrentBackBuffer() const;
		D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
		D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;

		void FlushCommandQueue();

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
		std::unique_ptr<GraphicContext> PGraphicContext;
		PRenderScene* PRScene;
		int PRenderNum = 0;
		float PTotalTime = 0;

		UINT PWidth = 1920;
		UINT PHeight = 1080;

		ComPtr<IDXGIFactory4> PDXGIFactory;

		ComPtr<ID3D12Device> PDevice;

		HANDLE PFenceEvent;
		ComPtr<ID3D12Fence> PFence;
		UINT64 PFenceValue;

		static const int SwapChainBufferCount = 3;
		float RTVColor[4] = { 0,0,0,0 };

		ComPtr<IDXGISwapChain> PSwapChain;
		ComPtr<ID3D12Resource> PSwapChainBuffer[SwapChainBufferCount];
		ComPtr<ID3D12Resource> PDepthStencilBuffer;
		int PCurrBackBuffer = 0;

		ComPtr<ID3D12CommandQueue> PCommandQueue;
		ComPtr<ID3D12CommandAllocator> PCommandAllocator;
		ComPtr<ID3D12GraphicsCommandList> PCommandList;

		UINT PRtvDescriptorSize;
		UINT PDsvDescriptorSize;
		UINT PCbvSrvUavDescriptorSize;
		ComPtr<ID3D12DescriptorHeap> PRtvHeap;
		ComPtr<ID3D12DescriptorHeap> PDsvHeap;

		CD3DX12_VIEWPORT PViewport;
		CD3DX12_RECT PScissorRect;

	};
}