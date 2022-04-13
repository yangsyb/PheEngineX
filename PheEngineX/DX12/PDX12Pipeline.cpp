#include "pch.h"
#if defined (RenderDeviceDX12)
#include "PDX12Pipeline.h"
#include "PDX12Converter.h"

namespace Phe
{
	PDX12Pipeline::PDX12Pipeline(PShader* Shader, P_RasterizerDesc Raster, P_BlendState Blend, P_DepthStencilState DepthStencil) : 
		PPipeline(Shader, Raster, Blend, DepthStencil)
	{
		MakeDX12RasterizerState(RasterizerDesc, PRasterizerState);
		MakeDX12BlendState(BlendState, PBlendState);
		MakeDX12DepthStencilState(DepthStencilState, PDepthStencilState);
	}

	PDX12Pipeline::~PDX12Pipeline()
	{
		PPipelineState = nullptr;
	}

	void PDX12Pipeline::SetRasterizerDesc(P_RasterizerDesc Raster)
	{
		RasterizerDesc = Raster;
		MakeDX12RasterizerState(RasterizerDesc, PRasterizerState);
	}

	void PDX12Pipeline::SetBlendState(P_BlendState Blend)
	{
		BlendState = Blend;
		MakeDX12BlendState(BlendState, PBlendState);
	}

}
#endif