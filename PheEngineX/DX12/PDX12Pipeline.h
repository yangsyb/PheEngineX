#if defined (RenderDeviceDX12)
#pragma once
#include "pch.h"
#include "Engine/Editor/PPipeline.h"

namespace Phe
{
	class PDX12Pipeline : public PPipeline
	{
	friend class PDX12RHI;
	public:
		PDX12Pipeline(PShader* Shader, P_RasterizerDesc Raster, P_BlendState Blend, P_DepthStencilState DepthStencil);
		virtual ~PDX12Pipeline();

		virtual void SetRasterizerDesc(P_RasterizerDesc Raster) override;
		virtual void SetBlendState(P_BlendState Blend) override;

		void SetPipelineState(ComPtr<ID3D12PipelineState> PSO) { PPipelineState = PSO; }
		ComPtr<ID3D12PipelineState> GetPipelineState() { return PPipelineState; }

	private:
		ComPtr<ID3D12PipelineState> PPipelineState;
		D3D12_RASTERIZER_DESC PRasterizerState;
		D3D12_DEPTH_STENCIL_DESC PDepthStencilState;
		D3D12_BLEND_DESC PBlendState;
	};
}
#endif