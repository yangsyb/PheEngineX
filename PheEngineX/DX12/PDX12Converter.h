#if defined (RenderDeviceDX12)
#pragma once
#include "Engine/Editor/PShader.h"
#define Render_Target_Count 1
namespace Phe
{
	static D3D12_BLEND SwitchBlend(P_BLEND Blend)
	{
		switch (Blend)
		{
		case P_BLEND::P_BLEND_ZERO:
			return D3D12_BLEND_ZERO;
			break;
		case P_BLEND::P_BLEND_ONE:
			return D3D12_BLEND_ONE;
			break;
		case P_BLEND::P_BLEND_SRC_COLOR:
			return D3D12_BLEND_SRC_COLOR;
			break;
		case P_BLEND::P_BLEND_INV_SRC_COLOR:
			return D3D12_BLEND_INV_SRC_COLOR;
			break;
		case P_BLEND::P_BLEND_SRC_ALPHA:
			return D3D12_BLEND_SRC_ALPHA;
			break;
		case P_BLEND::P_BLEND_INV_SRC_ALPHA:
			return D3D12_BLEND_INV_SRC_ALPHA;
			break;
		case P_BLEND::P_BLEND_DEST_ALPHA:
			return D3D12_BLEND_DEST_ALPHA;
			break;
		case P_BLEND::P_BLEND_INV_DEST_ALPHA:
			return D3D12_BLEND_INV_DEST_ALPHA;
			break;
		case P_BLEND::P_BLEND_DEST_COLOR:
			return D3D12_BLEND_DEST_COLOR;
			break;
		case P_BLEND::P_BLEND_INV_DEST_COLOR:
			return D3D12_BLEND_INV_DEST_COLOR;
			break;
		case P_BLEND::P_BLEND_SRC_ALPHA_SAT:
			return D3D12_BLEND_SRC_ALPHA_SAT;
			break;
		case P_BLEND::P_BLEND_BLEND_FACTOR:
			return D3D12_BLEND_BLEND_FACTOR;
			break;
		case P_BLEND::P_BLEND_INV_BLEND_FACTOR:
			return D3D12_BLEND_INV_BLEND_FACTOR;
			break;
		case P_BLEND::P_BLEND_SRC1_COLOR:
			return D3D12_BLEND_SRC1_COLOR;
			break;
		case P_BLEND::P_BLEND_INV_SRC1_COLOR:
			return D3D12_BLEND_INV_SRC1_COLOR;
			break;
		case P_BLEND::P_BLEND_SRC1_ALPHA:
			return D3D12_BLEND_SRC1_ALPHA;
			break;
		case P_BLEND::P_BLEND_INV_SRC1_ALPHA:
			return D3D12_BLEND_INV_SRC1_ALPHA;
			break;
		default:
			return D3D12_BLEND_ZERO;
		}
	}

	static D3D12_BLEND_OP SwitchBlendOP(P_BLEND_OP BlendOP)
	{
		switch (BlendOP)
		{
		case P_BLEND_OP::P_BLEND_OP_ADD:
			return D3D12_BLEND_OP_ADD;
			break;
		case P_BLEND_OP::P_BLEND_OP_SUBTRACT:
			return D3D12_BLEND_OP_SUBTRACT;
			break;
		case P_BLEND_OP::P_BLEND_OP_REV_SUBTRACT:
			return D3D12_BLEND_OP_REV_SUBTRACT;
			break;
		case P_BLEND_OP::P_BLEND_OP_MIN:
			return D3D12_BLEND_OP_MIN;
			break;
		case P_BLEND_OP::P_BLEND_OP_MAX:
			return D3D12_BLEND_OP_MAX;
			break;
		default:
			return D3D12_BLEND_OP_ADD;
		}
	}

	static D3D12_LOGIC_OP SwitchLogicOP(P_LOGIC_OP LogicOP)
	{
		switch (LogicOP)
		{
		case Phe::P_LOGIC_OP::P_LOGIC_OP_CLEAR:
			return D3D12_LOGIC_OP_CLEAR;
			break;
		case Phe::P_LOGIC_OP::P_LOGIC_OP_SET:
			return D3D12_LOGIC_OP_SET;
			break;
		case Phe::P_LOGIC_OP::P_LOGIC_OP_COPY:
			return D3D12_LOGIC_OP_COPY;
			break;
		case Phe::P_LOGIC_OP::P_LOGIC_OP_COPY_INVERTED:
			return D3D12_LOGIC_OP_COPY_INVERTED;
			break;
		case Phe::P_LOGIC_OP::P_LOGIC_OP_NOOP:
			return D3D12_LOGIC_OP_NOOP;
			break;
		case Phe::P_LOGIC_OP::P_LOGIC_OP_INVERT:
			return D3D12_LOGIC_OP_INVERT;
			break;
		case Phe::P_LOGIC_OP::P_LOGIC_OP_AND:
			return D3D12_LOGIC_OP_AND;
			break;
		case Phe::P_LOGIC_OP::P_LOGIC_OP_NAND:
			return D3D12_LOGIC_OP_NAND;
			break;
		case Phe::P_LOGIC_OP::P_LOGIC_OP_OR:
			return D3D12_LOGIC_OP_OR;
			break;
		case Phe::P_LOGIC_OP::P_LOGIC_OP_NOR:
			return D3D12_LOGIC_OP_NOR;
			break;
		case Phe::P_LOGIC_OP::P_LOGIC_OP_XOR:
			return D3D12_LOGIC_OP_XOR;
			break;
		case Phe::P_LOGIC_OP::P_LOGIC_OP_EQUIV:
			return D3D12_LOGIC_OP_EQUIV;
			break;
		case Phe::P_LOGIC_OP::P_LOGIC_OP_AND_REVERSE:
			return D3D12_LOGIC_OP_AND_REVERSE;
			break;
		case Phe::P_LOGIC_OP::P_LOGIC_OP_AND_INVERTED:
			return D3D12_LOGIC_OP_AND_INVERTED;
			break;
		case Phe::P_LOGIC_OP::P_LOGIC_OP_OR_REVERSE:
			return D3D12_LOGIC_OP_OR_REVERSE;
			break;
		case Phe::P_LOGIC_OP::P_LOGIC_OP_OR_INVERTED:
			return D3D12_LOGIC_OP_OR_INVERTED;
			break;
		default:
			return D3D12_LOGIC_OP_CLEAR;
		}
	}

	static D3D12_COLOR_WRITE_ENABLE SwitchColorWriteEnable(P_COLOR_WRITE_ENABLE ColorWriteEnable)
	{
		switch (ColorWriteEnable)
		{
		case Phe::P_COLOR_WRITE_ENABLE::P_COLOR_WRITE_ENABLE_RED:
			return D3D12_COLOR_WRITE_ENABLE_RED;
			break;
		case Phe::P_COLOR_WRITE_ENABLE::P_COLOR_WRITE_ENABLE_GREEN:
			return D3D12_COLOR_WRITE_ENABLE_GREEN;
			break;
		case Phe::P_COLOR_WRITE_ENABLE::P_COLOR_WRITE_ENABLE_BLUE:
			return D3D12_COLOR_WRITE_ENABLE_BLUE;
			break;
		case Phe::P_COLOR_WRITE_ENABLE::P_COLOR_WRITE_ENABLE_ALPHA:
			return D3D12_COLOR_WRITE_ENABLE_ALPHA;
			break;
		case Phe::P_COLOR_WRITE_ENABLE::P_COLOR_WRITE_ENABLE_ALL:
			return D3D12_COLOR_WRITE_ENABLE_ALL;
			break;
		default:
			return D3D12_COLOR_WRITE_ENABLE_RED;
		}
	}

	inline void MakeDX12RasterizerState(P_RasterizerDesc PDesc, D3D12_RASTERIZER_DESC& D3DRasterizerDesc)
	{
		D3DRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
		D3DRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
		D3DRasterizerDesc.FrontCounterClockwise = FALSE;
		D3DRasterizerDesc.DepthBias = PDesc.DepthBias;
		D3DRasterizerDesc.DepthBiasClamp = PDesc.DepthBiasClamp;
		D3DRasterizerDesc.SlopeScaledDepthBias = PDesc.SlopeScaledDepthBias;
		D3DRasterizerDesc.DepthClipEnable = TRUE;
		D3DRasterizerDesc.MultisampleEnable = FALSE;
		D3DRasterizerDesc.AntialiasedLineEnable = FALSE;
		D3DRasterizerDesc.ForcedSampleCount = 0;
		D3DRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	}

	inline void MakeDX12BlendState(P_BlendState PBlend, D3D12_BLEND_DESC& D3DBlendState)
	{
		D3DBlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		D3D12_RENDER_TARGET_BLEND_DESC BlendDesc;
		BlendDesc.BlendEnable = PBlend.BlendEnable;
		BlendDesc.LogicOpEnable = PBlend.LogicEnable;
		BlendDesc.SrcBlend = SwitchBlend(PBlend.SrcBlend);
		BlendDesc.DestBlend = SwitchBlend(PBlend.DestBlend);
		BlendDesc.BlendOp = SwitchBlendOP(PBlend.BlendOp);
		BlendDesc.SrcBlendAlpha = SwitchBlend(PBlend.SrcBlend);
		BlendDesc.DestBlendAlpha = SwitchBlend(PBlend.DestBlendAlpha);
		BlendDesc.BlendOpAlpha = SwitchBlendOP(PBlend.BlendOpAlpha);
		BlendDesc.LogicOp = SwitchLogicOP(PBlend.LogicOp);
		BlendDesc.RenderTargetWriteMask = SwitchColorWriteEnable(PBlend.RenderTargetWriteMask);
		for(int i = 0; i < Render_Target_Count; ++i)
		{
			D3DBlendState.RenderTarget[i] = BlendDesc;
		}
	}
}


#endif