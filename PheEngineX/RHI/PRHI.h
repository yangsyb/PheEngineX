#pragma once
#include "pch.h"

namespace Phe
{
	class PRHI
	{
	public:
		static PRHI* Get();
		static void CreateRHI();
		static void DestroyRHI();

		virtual void InitRHI() = 0;
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void InitGraphicsPipeline() = 0;

		virtual void AddRenderResourceMesh() = 0;


		virtual void Flush() = 0;

		static PRHI* RHI;
		PRHI();
		virtual ~PRHI();

	protected:
	};
}