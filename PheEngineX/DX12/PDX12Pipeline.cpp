#include "pch.h"
#if defined (RenderDeviceDX12)
#include "PDX12Pipeline.h"

namespace Phe
{
	PDX12Pipeline::PDX12Pipeline(PShader* Shader) : PPipeline(Shader)
	{
		
	}

	PDX12Pipeline::~PDX12Pipeline()
	{
		PPipelineState = nullptr;
	}

}
#endif