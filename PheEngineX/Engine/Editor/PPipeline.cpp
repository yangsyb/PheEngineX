#include "pch.h"
#include "PPipeline.h"

namespace Phe
{

	PPipeline::PPipeline(PShader* Shader, P_RasterizerDesc Raster, P_BlendState Blend, P_DepthStencilState DepthStencil) : PPipelineShader(Shader), RasterizerDesc(Raster), BlendState(Blend), DepthStencilState(DepthStencil)
	{

	}

	PPipeline::~PPipeline()
	{
		
	}

}