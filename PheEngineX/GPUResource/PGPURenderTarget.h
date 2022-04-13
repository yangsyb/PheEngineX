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
		RTBuffer(RTBufferType Type, UINT32 Width, UINT32 Height, P_TextureFormat Format) : PType(Type), PWidth(Width), PHeight(Height), PHandleOffset(0), PRTTexture(nullptr), PTextureFormat(Format)
		{

		}
		virtual ~RTBuffer();

		P_TextureFormat GetTextureFormat() { return PTextureFormat; }

		PGPUTexture* PRTTexture;
		P_TextureFormat PTextureFormat;
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

		virtual void AddColorBuffer(UINT32 BufferNum, P_TextureFormat Format) = 0;
		virtual void AddDepthStencilBuffer(P_TextureFormat Format) = 0;

		virtual UINT32 GetWidth() { return PWidth; }
		virtual UINT32 GetHeight() { return PHeight; }

		virtual std::vector<RTBuffer*> GetColorBuffer() { return PRenderTargetColorBuffer; }
		virtual RTBuffer* GetColorBuffer(UINT32 Index) { return PRenderTargetColorBuffer.size() > Index ? PRenderTargetColorBuffer[Index] : nullptr; }
		virtual RTBuffer* GetDepthStencilBuffer() { return PRenderTargetDepthStencilBuffer; }
	protected:
		std::string PRenderTargetName;
		UINT32 PWidth;
		UINT32 PHeight;

		std::vector<RTBuffer*> PRenderTargetColorBuffer;
		RTBuffer* PRenderTargetDepthStencilBuffer;
	};

}