#pragma once
#include "pch.h"

namespace Phe
{

	struct P_RasterizerDesc
	{
		UINT32 DepthBias;
		float DepthBiasClamp;
		float SlopeScaledDepthBias;

		P_RasterizerDesc() : DepthBias(0), DepthBiasClamp(0.f), SlopeScaledDepthBias(0.f)
		{

		}
		P_RasterizerDesc(UINT32 DB, float DBC, float SSDB) : DepthBias(DB), DepthBiasClamp(DBC), SlopeScaledDepthBias(SSDB)
		{

		}
	};

	enum class P_BLEND
	{
		P_BLEND_ZERO = 1,
		P_BLEND_ONE = 2,
		P_BLEND_SRC_COLOR = 3,
		P_BLEND_INV_SRC_COLOR = 4,
		P_BLEND_SRC_ALPHA = 5,
		P_BLEND_INV_SRC_ALPHA = 6,
		P_BLEND_DEST_ALPHA = 7,
		P_BLEND_INV_DEST_ALPHA = 8,
		P_BLEND_DEST_COLOR = 9,
		P_BLEND_INV_DEST_COLOR = 10,
		P_BLEND_SRC_ALPHA_SAT = 11,
		P_BLEND_BLEND_FACTOR = 14,
		P_BLEND_INV_BLEND_FACTOR = 15,
		P_BLEND_SRC1_COLOR = 16,
		P_BLEND_INV_SRC1_COLOR = 17,
		P_BLEND_SRC1_ALPHA = 18,
		P_BLEND_INV_SRC1_ALPHA = 19
	};

	enum class P_BLEND_OP
	{
		P_BLEND_OP_ADD = 1,
		P_BLEND_OP_SUBTRACT = 2,
		P_BLEND_OP_REV_SUBTRACT = 3,
		P_BLEND_OP_MIN = 4,
		P_BLEND_OP_MAX = 5
	};

	enum class P_LOGIC_OP
	{
		P_LOGIC_OP_CLEAR = 0,
		P_LOGIC_OP_SET = (P_LOGIC_OP_CLEAR + 1),
		P_LOGIC_OP_COPY = (P_LOGIC_OP_SET + 1),
		P_LOGIC_OP_COPY_INVERTED = (P_LOGIC_OP_COPY + 1),
		P_LOGIC_OP_NOOP = (P_LOGIC_OP_COPY_INVERTED + 1),
		P_LOGIC_OP_INVERT = (P_LOGIC_OP_NOOP + 1),
		P_LOGIC_OP_AND = (P_LOGIC_OP_INVERT + 1),
		P_LOGIC_OP_NAND = (P_LOGIC_OP_AND + 1),
		P_LOGIC_OP_OR = (P_LOGIC_OP_NAND + 1),
		P_LOGIC_OP_NOR = (P_LOGIC_OP_OR + 1),
		P_LOGIC_OP_XOR = (P_LOGIC_OP_NOR + 1),
		P_LOGIC_OP_EQUIV = (P_LOGIC_OP_XOR + 1),
		P_LOGIC_OP_AND_REVERSE = (P_LOGIC_OP_EQUIV + 1),
		P_LOGIC_OP_AND_INVERTED = (P_LOGIC_OP_AND_REVERSE + 1),
		P_LOGIC_OP_OR_REVERSE = (P_LOGIC_OP_AND_INVERTED + 1),
		P_LOGIC_OP_OR_INVERTED = (P_LOGIC_OP_OR_REVERSE + 1)
	};

	enum class P_COLOR_WRITE_ENABLE
	{
		P_COLOR_WRITE_ENABLE_RED = 1,
		P_COLOR_WRITE_ENABLE_GREEN = 2,
		P_COLOR_WRITE_ENABLE_BLUE = 4,
		P_COLOR_WRITE_ENABLE_ALPHA = 8,
		P_COLOR_WRITE_ENABLE_ALL = (((P_COLOR_WRITE_ENABLE_RED | P_COLOR_WRITE_ENABLE_GREEN) | P_COLOR_WRITE_ENABLE_BLUE) | P_COLOR_WRITE_ENABLE_ALPHA)
	};

	struct P_BlendState
	{
		bool BlendEnable;
		bool LogicEnable;
		P_BLEND SrcBlend;
		P_BLEND DestBlend;
		P_BLEND_OP BlendOp;
		P_BLEND SrcBlendAlpha;
		P_BLEND DestBlendAlpha;
		P_BLEND_OP BlendOpAlpha;
		P_LOGIC_OP LogicOp;
		P_COLOR_WRITE_ENABLE RenderTargetWriteMask;

		P_BlendState()
			: BlendEnable(false)
			, LogicEnable(false)
			, SrcBlend(P_BLEND::P_BLEND_ONE)
			, DestBlend(P_BLEND::P_BLEND_ZERO)
			, BlendOp(P_BLEND_OP::P_BLEND_OP_ADD)
			, SrcBlendAlpha(P_BLEND::P_BLEND_ONE)
			, DestBlendAlpha(P_BLEND::P_BLEND_ZERO)
			, BlendOpAlpha(P_BLEND_OP::P_BLEND_OP_ADD)
			, LogicOp(P_LOGIC_OP::P_LOGIC_OP_NOOP)
			, RenderTargetWriteMask(P_COLOR_WRITE_ENABLE::P_COLOR_WRITE_ENABLE_ALL)
		{}
	};

	class PShader
	{
	public:
		PShader(const std::string ShaderName, const std::wstring FilePath, std::string VS = "VS", std::string PS = "PS", P_RasterizerDesc Raster = P_RasterizerDesc(), P_BlendState Blend = P_BlendState());
		virtual ~PShader();

		void RegisterResourceBinding(UINT32 PrimitiveId, std::string ParameterName, UINT32 BindingOffset);
		void UnRegisterResourceBinding(UINT32 PrimitiveId);

		virtual std::string GetShaderName() { return PShaderName; }
		virtual std::wstring GetShaderFilePath() { return PFilePath; }
		virtual void SetRasterizerDesc(P_RasterizerDesc Raster) { RasterizerDesc = Raster; }
		virtual void SetBlendState(P_BlendState Blend) { BlendState = Blend; }
//		virtual std::vector<ShaderParameter> GetShaderParameters() { return Params; }
//		virtual std::unordered_map<std::string, UINT32> GetResourceBindingByPrimitiveName(UINT32 PrimitiveId) { return ResourceBinding.at(PrimitiveId); }
	protected:
		std::string PShaderName;
		std::wstring PFilePath;
		std::string PVSEntry;
		std::string PPSEntry;
		P_RasterizerDesc RasterizerDesc;
		P_BlendState BlendState;
//		std::unordered_map<UINT32, std::unordered_map<std::string, UINT32>> ResourceBinding;
	};
}