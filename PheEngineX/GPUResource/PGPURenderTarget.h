#pragma once
#include "pch.h"
#include "PGPUResource.h"
#include "PGPUTexture.h"

namespace Phe
{
	enum class RTBufferType
	{
		RTColorBuffer = 0,
		RTDepthStencilBuffer = 1,
	};

	struct RTBuffer
	{
		RTBuffer(RTBufferType Type, UINT32 Width, UINT32 Height) : PType(Type), PWidth(Width), PHeight(Height), PHandleOffset(0), PRTTexture(nullptr)
		{

		}
		virtual ~RTBuffer();

		PGPUTexture* PRTTexture;
		UINT32 PHandleOffset;
		RTBufferType PType;
		UINT32 PWidth;
		UINT32 PHeight;
	};

	class PGPURenderTarget :public PGPUResource
	{
	public:
		PGPURenderTarget(std::string RenderTargetName, UINT32 Width = 1920, UINT32 Height = 1080);
		virtual ~PGPURenderTarget();

		virtual void AddColorBuffer() = 0;
		virtual void AddDepthStencilBuffer() = 0;

		virtual UINT32 GetWidth() { return PWidth; }
		virtual UINT32 GetHeight() { return PHeight; }

		std::vector<RTBuffer*> GetColorBuffer() { return PRenderTargetColorBuffer; }
		RTBuffer* GetDepthStencilBuffer() { return PRenderTargetDepthStencilBuffer; }
	protected:
		std::string PRenderTargetName;
		UINT32 PWidth;
		UINT32 PHeight;

		std::vector<RTBuffer*> PRenderTargetColorBuffer;
		RTBuffer* PRenderTargetDepthStencilBuffer;
	};

}