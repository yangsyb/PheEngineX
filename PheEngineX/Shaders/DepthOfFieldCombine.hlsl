#include "StaticSamplers.hlsli"
#include "ShaderDefines.h"
#define PI 3.1415927
Texture2D CocTexture   : register(t0);
Texture2D SceneTexture : register(t6);
Texture2D DofTexture   : register(t7);

cbuffer cbDoF : register(b4)
{
	float FocusDistance;
	float FocusRange;
	float ScreenWidth;
	float ScreenHeight;
};

struct VertexIn
{
	float3 PosL  : POSITION;
	float4 Normal : NORMAL;
	float4 Tangent : TANGENT;
	float4 TangentY : TANGENTY;
	float2 TextCoord : TEXTCOORD;
};

struct VS_Output {
	float4 position : SV_Position;
};

VS_Output VS(VertexIn vin)
{
	VS_Output output;

	output.position = float4(vin.PosL, 1);

	return output;
}

float4 PS(VS_Output Input) : SV_Target
{
    int X = floor(Input.position.x);
	int Y = floor(Input.position.y);

	float2 Tex;
	Tex.x = 1.0f * X / ScreenWidth;
	Tex.y = 1.0f * Y / ScreenHeight;

    float InvScreenWidth = 1/ScreenWidth;
    float InvScreenHeight = 1/ScreenHeight;

    float4 Dof = DofTexture.SampleLevel(gsamLinearClamp, Tex, 0.0f);
	float Coc = CocTexture.SampleLevel(gsamLinearClamp, Tex, 0.0f).r;

    Coc = (Coc - 0.5) * 2.0 * 0.05;

    float FFa = smoothstep(gInvRenderTargetSize.y * 2.0, gInvRenderTargetSize.y * 4.0, coc);

}