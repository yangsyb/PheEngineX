#include "pch.h"
#if defined (RenderDeviceDX12)
#include "PDX12ShaderManager.h"
#include "RHI/PRHI.h"
namespace Phe
{
	PDX12Shadermanager::PDX12Shadermanager() : PShaderManager()
	{
	//Default Shaders, You can Add shader whenever you want in runtime
		PShader* DefaultShader = new PShader("DefaultShader", L"Shaders\\color.hlsl");
		PShader* WPOShader = new PShader("WPOShader", L"Shaders\\tree.hlsl");
		PShader* WPOTrunkShader = new PShader("WPOTrunkShader", L"Shaders\\treetrunk.hlsl");
		PShader* TowerShader = new PShader("PBRShader", L"Shaders\\BRDFCommon.hlsl");
 		PShader* SkyBoxShader = new PShader("SkyShader", L"Shaders\\SkyBox.hlsl", "VS", "PS");
		PShader* HighLightShader = new PShader("HighLightShader", L"Shaders\\HighLight.hlsl", "", "PS");


		AddShader(DefaultShader);
		AddShader(WPOShader);
		AddShader(WPOTrunkShader);
		AddShader(TowerShader);
		AddShader(SkyBoxShader);
		AddShader(HighLightShader);
		PropertyID.reserve(64);
	}

	PDX12Shadermanager::~PDX12Shadermanager()
	{

	}


}


/*
P_RasterizerDesc(P_CULL_MODE::P_CULL_MODE_NONE,0,0,0),
		P_BlendState(),
		P_DepthStencilState(
			true,
			P_DEPTH_WRITE_MASK::P_DEPTH_WRITE_MASK_ALL,
			P_COMPARISON_FUNC::P_COMPARISON_FUNC_LESS_EQUAL,
			false,
			0xff,
			0xff,
			P_STENCIL_OP::P_STENCIL_OP_KEEP,
			P_STENCIL_OP::P_STENCIL_OP_KEEP,
			P_STENCIL_OP::P_STENCIL_OP_KEEP,
			P_COMPARISON_FUNC::P_COMPARISON_FUNC_ALWAYS,
			P_STENCIL_OP::P_STENCIL_OP_KEEP,
			P_STENCIL_OP::P_STENCIL_OP_KEEP,
			P_STENCIL_OP::P_STENCIL_OP_KEEP,
			P_COMPARISON_FUNC::P_COMPARISON_FUNC_ALWAYS
		)

*/


#endif