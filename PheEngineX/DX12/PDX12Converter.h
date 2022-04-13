#if defined (RenderDeviceDX12)
#pragma once
#include "Engine/Editor/PPipeline.h"
#include "Engine/Editor/PTexture.h"
#define Render_Target_Count 1
namespace Phe
{
	static D3D12_CULL_MODE SwitchCullMode(P_CULL_MODE CullMode)
	{
		switch (CullMode)
		{
		case Phe::P_CULL_MODE::P_CULL_MODE_NONE:
			return D3D12_CULL_MODE_NONE;
		case Phe::P_CULL_MODE::P_CULL_MODE_FRONT:
			return D3D12_CULL_MODE_FRONT;
		case Phe::P_CULL_MODE::P_CULL_MODE_BACK:
			return D3D12_CULL_MODE_BACK;
		default:
			return D3D12_CULL_MODE_NONE;
		}
	}

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

	static D3D12_DEPTH_WRITE_MASK SwitchDepthWriteMask(P_DEPTH_WRITE_MASK DepthWriteMask)
	{
		switch (DepthWriteMask)
		{
		case Phe::P_DEPTH_WRITE_MASK::P_DEPTH_WRITE_MASK_ZERO:
			return D3D12_DEPTH_WRITE_MASK_ZERO;
		case Phe::P_DEPTH_WRITE_MASK::P_DEPTH_WRITE_MASK_ALL:
			return D3D12_DEPTH_WRITE_MASK_ALL;
		default:
			return D3D12_DEPTH_WRITE_MASK_ZERO;
		}
	}

	static D3D12_COMPARISON_FUNC SwitchComparisonFunc(P_COMPARISON_FUNC ComparisonFunc)
	{
		switch (ComparisonFunc)
		{
		case Phe::P_COMPARISON_FUNC::P_COMPARISON_FUNC_NEVER:
			return D3D12_COMPARISON_FUNC_NEVER;
		case Phe::P_COMPARISON_FUNC::P_COMPARISON_FUNC_LESS:
			return D3D12_COMPARISON_FUNC_LESS;
		case Phe::P_COMPARISON_FUNC::P_COMPARISON_FUNC_EQUAL:
			return D3D12_COMPARISON_FUNC_EQUAL;
		case Phe::P_COMPARISON_FUNC::P_COMPARISON_FUNC_LESS_EQUAL:
			return D3D12_COMPARISON_FUNC_LESS_EQUAL;
		case Phe::P_COMPARISON_FUNC::P_COMPARISON_FUNC_GREATER:
			return D3D12_COMPARISON_FUNC_GREATER;
		case Phe::P_COMPARISON_FUNC::P_COMPARISON_FUNC_NOT_EQUAL:
			return D3D12_COMPARISON_FUNC_NOT_EQUAL;
		case Phe::P_COMPARISON_FUNC::P_COMPARISON_FUNC_GREATER_EQUAL:
			return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
		case Phe::P_COMPARISON_FUNC::P_COMPARISON_FUNC_ALWAYS:
			return D3D12_COMPARISON_FUNC_ALWAYS;
		default:
			return D3D12_COMPARISON_FUNC_NEVER;
		}
	}

	static D3D12_STENCIL_OP SwitchStencilOP(P_STENCIL_OP StencilOP)
	{
		switch (StencilOP)
		{
		case Phe::P_STENCIL_OP::P_STENCIL_OP_KEEP:
			return D3D12_STENCIL_OP_KEEP;
		case Phe::P_STENCIL_OP::P_STENCIL_OP_ZERO:
			return D3D12_STENCIL_OP_ZERO;
		case Phe::P_STENCIL_OP::P_STENCIL_OP_REPLACE:
			return D3D12_STENCIL_OP_REPLACE;
		case Phe::P_STENCIL_OP::P_STENCIL_OP_INCR_SAT:
			return D3D12_STENCIL_OP_INCR_SAT;
		case Phe::P_STENCIL_OP::P_STENCIL_OP_DECR_SAT:
			return D3D12_STENCIL_OP_DECR_SAT;
		case Phe::P_STENCIL_OP::P_STENCIL_OP_INVERT:
			return D3D12_STENCIL_OP_INVERT;
		case Phe::P_STENCIL_OP::P_STENCIL_OP_INCR:
			return D3D12_STENCIL_OP_INCR;
		case Phe::P_STENCIL_OP::P_STENCIL_OP_DECR:
			return D3D12_STENCIL_OP_DECR;
		default:
			return D3D12_STENCIL_OP_KEEP;
		}
	}

	static DXGI_FORMAT SwitchFormat(P_TextureFormat TextureFormat)
	{
		switch (TextureFormat)
		{
		case Phe::P_TextureFormat::P_FORMAT_UNKNOWN:
			return DXGI_FORMAT_UNKNOWN;

		case Phe::P_TextureFormat::P_FORMAT_R32G32B32A32_TYPELESS:
			return DXGI_FORMAT_R32G32B32A32_TYPELESS;

		case Phe::P_TextureFormat::P_FORMAT_R32G32B32A32_FLOAT:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;

		case Phe::P_TextureFormat::P_FORMAT_R32G32B32A32_UINT:
			return DXGI_FORMAT_R32G32B32A32_UINT;

		case Phe::P_TextureFormat::P_FORMAT_R32G32B32A32_SINT:
			return DXGI_FORMAT_R32G32B32A32_SINT;

		case Phe::P_TextureFormat::P_FORMAT_R32G32B32_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32G32B32_FLOAT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32G32B32_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32G32B32_SINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16G16B16A16_TYPELESS:
			break;

		case Phe::P_TextureFormat::P_FORMAT_R16G16B16A16_FLOAT:
			return DXGI_FORMAT_R16G16B16A16_FLOAT;

		case Phe::P_TextureFormat::P_FORMAT_R16G16B16A16_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16G16B16A16_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16G16B16A16_SNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16G16B16A16_SINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32G32_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32G32_FLOAT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32G32_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32G32_SINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32G8X24_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_D32_FLOAT_S8X24_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32_FLOAT_X8X24_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_X32_TYPELESS_G8X24_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R10G10B10A2_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R10G10B10A2_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R10G10B10A2_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R11G11B10_FLOAT:
			return DXGI_FORMAT_R11G11B10_FLOAT;

		case Phe::P_TextureFormat::P_FORMAT_R8G8B8A8_TYPELESS:
			break;

		case Phe::P_TextureFormat::P_FORMAT_R8G8B8A8_UNORM:
			return DXGI_FORMAT_R8G8B8A8_UNORM;

		case Phe::P_TextureFormat::P_FORMAT_R8G8B8A8_UNORM_SRGB:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8G8B8A8_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8G8B8A8_SNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8G8B8A8_SINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16G16_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16G16_FLOAT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16G16_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16G16_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16G16_SNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16G16_SINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_D32_FLOAT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32_FLOAT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32_SINT:
			break;

		case Phe::P_TextureFormat::P_FORMAT_R24G8_TYPELESS:
			return DXGI_FORMAT_R24G8_TYPELESS;

		case Phe::P_TextureFormat::P_FORMAT_D24_UNORM_S8_UINT:
			return DXGI_FORMAT_D24_UNORM_S8_UINT;

		case Phe::P_TextureFormat::P_FORMAT_R24_UNORM_X8_TYPELESS:
			return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;

		case Phe::P_TextureFormat::P_FORMAT_X24_TYPELESS_G8_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8G8_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8G8_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8G8_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8G8_SNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8G8_SINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16_FLOAT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_D16_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16_SNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16_SINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8_SNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8_SINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_A8_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R1_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R9G9B9E5_SHAREDEXP:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8G8_B8G8_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_G8R8_G8B8_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC1_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC1_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC1_UNORM_SRGB:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC2_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC2_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC2_UNORM_SRGB:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC3_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC3_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC3_UNORM_SRGB:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC4_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC4_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC4_SNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC5_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC5_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC5_SNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_B5G6R5_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_B5G5R5A1_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_B8G8R8A8_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_B8G8R8X8_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_B8G8R8A8_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_B8G8R8A8_UNORM_SRGB:
			break;
		case Phe::P_TextureFormat::P_FORMAT_B8G8R8X8_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_B8G8R8X8_UNORM_SRGB:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC6H_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC6H_UF16:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC6H_SF16:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC7_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC7_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC7_UNORM_SRGB:
			break;
		case Phe::P_TextureFormat::P_FORMAT_AYUV:
			break;
		case Phe::P_TextureFormat::P_FORMAT_Y410:
			break;
		case Phe::P_TextureFormat::P_FORMAT_Y416:
			break;
		case Phe::P_TextureFormat::P_FORMAT_NV12:
			break;
		case Phe::P_TextureFormat::P_FORMAT_P010:
			break;
		case Phe::P_TextureFormat::P_FORMAT_P016:
			break;
		case Phe::P_TextureFormat::P_FORMAT_420_OPAQUE:
			break;
		case Phe::P_TextureFormat::P_FORMAT_YUY2:
			break;
		case Phe::P_TextureFormat::P_FORMAT_Y210:
			break;
		case Phe::P_TextureFormat::P_FORMAT_Y216:
			break;
		case Phe::P_TextureFormat::P_FORMAT_NV11:
			break;
		case Phe::P_TextureFormat::P_FORMAT_AI44:
			break;
		case Phe::P_TextureFormat::P_FORMAT_IA44:
			break;
		case Phe::P_TextureFormat::P_FORMAT_P8:
			break;
		case Phe::P_TextureFormat::P_FORMAT_A8P8:
			break;
		case Phe::P_TextureFormat::P_FORMAT_B4G4R4A4_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_P208:
			break;
		case Phe::P_TextureFormat::P_FORMAT_V208:
			break;
		case Phe::P_TextureFormat::P_FORMAT_V408:
			break;
		case Phe::P_TextureFormat::P_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE:
			break;
		case Phe::P_TextureFormat::P_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE:
			break;
		default:
			return DXGI_FORMAT_UNKNOWN;
		}
	}

	static DXGI_FORMAT SwitchSupportFormat(P_TextureFormat TextureFormat)
	{
		switch (TextureFormat)
		{
		case Phe::P_TextureFormat::P_FORMAT_UNKNOWN:
			return DXGI_FORMAT_UNKNOWN;

		case Phe::P_TextureFormat::P_FORMAT_R32G32B32A32_TYPELESS:
			return DXGI_FORMAT_R32G32B32A32_TYPELESS;

		case Phe::P_TextureFormat::P_FORMAT_R32G32B32A32_FLOAT:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;

		case Phe::P_TextureFormat::P_FORMAT_R32G32B32A32_UINT:
			return DXGI_FORMAT_R32G32B32A32_UINT;

		case Phe::P_TextureFormat::P_FORMAT_R32G32B32A32_SINT:
			return DXGI_FORMAT_R32G32B32A32_SINT;

		case Phe::P_TextureFormat::P_FORMAT_R32G32B32_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32G32B32_FLOAT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32G32B32_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32G32B32_SINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16G16B16A16_TYPELESS:
			break;

		case Phe::P_TextureFormat::P_FORMAT_R16G16B16A16_FLOAT:
			return DXGI_FORMAT_R16G16B16A16_FLOAT;

		case Phe::P_TextureFormat::P_FORMAT_R16G16B16A16_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16G16B16A16_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16G16B16A16_SNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16G16B16A16_SINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32G32_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32G32_FLOAT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32G32_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32G32_SINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32G8X24_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_D32_FLOAT_S8X24_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32_FLOAT_X8X24_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_X32_TYPELESS_G8X24_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R10G10B10A2_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R10G10B10A2_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R10G10B10A2_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R11G11B10_FLOAT:
			return DXGI_FORMAT_R11G11B10_FLOAT;

		case Phe::P_TextureFormat::P_FORMAT_R8G8B8A8_TYPELESS:
			break;

		case Phe::P_TextureFormat::P_FORMAT_R8G8B8A8_UNORM:
			return DXGI_FORMAT_R8G8B8A8_UNORM;

		case Phe::P_TextureFormat::P_FORMAT_R8G8B8A8_UNORM_SRGB:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8G8B8A8_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8G8B8A8_SNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8G8B8A8_SINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16G16_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16G16_FLOAT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16G16_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16G16_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16G16_SNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16G16_SINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_D32_FLOAT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32_FLOAT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R32_SINT:
			break;

		case Phe::P_TextureFormat::P_FORMAT_R24G8_TYPELESS:
			return DXGI_FORMAT_R24G8_TYPELESS;

		case Phe::P_TextureFormat::P_FORMAT_D24_UNORM_S8_UINT:
			return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;

		case Phe::P_TextureFormat::P_FORMAT_R24_UNORM_X8_TYPELESS:
			return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;

		case Phe::P_TextureFormat::P_FORMAT_X24_TYPELESS_G8_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8G8_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8G8_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8G8_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8G8_SNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8G8_SINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16_FLOAT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_D16_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16_SNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R16_SINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8_UINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8_SNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8_SINT:
			break;
		case Phe::P_TextureFormat::P_FORMAT_A8_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R1_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R9G9B9E5_SHAREDEXP:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R8G8_B8G8_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_G8R8_G8B8_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC1_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC1_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC1_UNORM_SRGB:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC2_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC2_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC2_UNORM_SRGB:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC3_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC3_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC3_UNORM_SRGB:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC4_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC4_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC4_SNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC5_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC5_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC5_SNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_B5G6R5_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_B5G5R5A1_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_B8G8R8A8_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_B8G8R8X8_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_B8G8R8A8_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_B8G8R8A8_UNORM_SRGB:
			break;
		case Phe::P_TextureFormat::P_FORMAT_B8G8R8X8_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_B8G8R8X8_UNORM_SRGB:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC6H_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC6H_UF16:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC6H_SF16:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC7_TYPELESS:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC7_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_BC7_UNORM_SRGB:
			break;
		case Phe::P_TextureFormat::P_FORMAT_AYUV:
			break;
		case Phe::P_TextureFormat::P_FORMAT_Y410:
			break;
		case Phe::P_TextureFormat::P_FORMAT_Y416:
			break;
		case Phe::P_TextureFormat::P_FORMAT_NV12:
			break;
		case Phe::P_TextureFormat::P_FORMAT_P010:
			break;
		case Phe::P_TextureFormat::P_FORMAT_P016:
			break;
		case Phe::P_TextureFormat::P_FORMAT_420_OPAQUE:
			break;
		case Phe::P_TextureFormat::P_FORMAT_YUY2:
			break;
		case Phe::P_TextureFormat::P_FORMAT_Y210:
			break;
		case Phe::P_TextureFormat::P_FORMAT_Y216:
			break;
		case Phe::P_TextureFormat::P_FORMAT_NV11:
			break;
		case Phe::P_TextureFormat::P_FORMAT_AI44:
			break;
		case Phe::P_TextureFormat::P_FORMAT_IA44:
			break;
		case Phe::P_TextureFormat::P_FORMAT_P8:
			break;
		case Phe::P_TextureFormat::P_FORMAT_A8P8:
			break;
		case Phe::P_TextureFormat::P_FORMAT_B4G4R4A4_UNORM:
			break;
		case Phe::P_TextureFormat::P_FORMAT_P208:
			break;
		case Phe::P_TextureFormat::P_FORMAT_V208:
			break;
		case Phe::P_TextureFormat::P_FORMAT_V408:
			break;
		case Phe::P_TextureFormat::P_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE:
			break;
		case Phe::P_TextureFormat::P_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE:
			break;
		default:
			return DXGI_FORMAT_UNKNOWN;
		}
	}

	inline void MakeDX12RasterizerState(P_RasterizerDesc PDesc, D3D12_RASTERIZER_DESC& D3DRasterizerDesc)
	{
		D3DRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
		D3DRasterizerDesc.CullMode = SwitchCullMode(PDesc.CullMode);
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

	inline void MakeDX12DepthStencilState(P_DepthStencilState PDepthStencilState, D3D12_DEPTH_STENCIL_DESC& D3DDepthStencilState)
	{
		D3DDepthStencilState.DepthEnable = PDepthStencilState.DepthEnable;
		D3DDepthStencilState.DepthWriteMask = SwitchDepthWriteMask(PDepthStencilState.DepthWriteMask);
		D3DDepthStencilState.DepthFunc = SwitchComparisonFunc(PDepthStencilState.DepthFunc);
		D3DDepthStencilState.StencilEnable = PDepthStencilState.StencilEnable;
		D3DDepthStencilState.StencilReadMask = PDepthStencilState.StencilReadMask;
		D3DDepthStencilState.StencilWriteMask = PDepthStencilState.StencilWriteMask;
		D3DDepthStencilState.FrontFace.StencilFailOp = SwitchStencilOP(PDepthStencilState.FrontStencilFailOp);
		D3DDepthStencilState.FrontFace.StencilDepthFailOp = SwitchStencilOP(PDepthStencilState.FrontStencilDepthFailOp);
		D3DDepthStencilState.FrontFace.StencilPassOp = SwitchStencilOP(PDepthStencilState.FrontStencilPassOp);
		D3DDepthStencilState.FrontFace.StencilFunc = SwitchComparisonFunc(PDepthStencilState.FrontStencilFunc);
		D3DDepthStencilState.BackFace.StencilFailOp = SwitchStencilOP(PDepthStencilState.BackStencilFailOp);
		D3DDepthStencilState.BackFace.StencilDepthFailOp = SwitchStencilOP(PDepthStencilState.BackStencilDepthFailOp);
		D3DDepthStencilState.BackFace.StencilPassOp = SwitchStencilOP(PDepthStencilState.BackStencilPassOp);
		D3DDepthStencilState.BackFace.StencilFunc = SwitchComparisonFunc(PDepthStencilState.BackStencilFunc);

	}
}


#endif