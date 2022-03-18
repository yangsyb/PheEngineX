#if defined (RenderDeviceDX12)
#pragma once
#include "pch.h"
#include "GPUResource/PPipeline.h"

namespace Phe
{
	class PDX12Pipeline : public PPipeline
	{
	public:
		PDX12Pipeline(PShader* Shader);
		virtual ~PDX12Pipeline();
		void SetPipelineState(ComPtr<ID3D12PipelineState> PSO) { PPipelineState = PSO; }
		ComPtr<ID3D12PipelineState> GetPipelineState() { return PPipelineState; }

	private:
		ComPtr<ID3D12PipelineState> PPipelineState;
	};
}
#endif