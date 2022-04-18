#include "StaticSamplers.hlsli"
#include "ShaderDefines.h"
#define PI 3.1415927
Texture2D gDepthInput	: register(t0);

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

half4 PS(VS_Output Input) : SV_Target
{
    int X = floor(Input.position.x);
	int Y = floor(Input.position.y);

	float2 Tex;
	Tex.x = 1.0f * X / ScreenWidth;
	Tex.y = 1.0f * Y / ScreenHeight;

    float InvScreenWidth = 1/ScreenWidth;
    float InvScreenHeight = 1/ScreenHeight;

	return gDepthInput.SampleLevel(gsamLinearClamp, Tex, 0.0f);

//    const float4 duv = float4(InvScreenWidth, InvScreenHeight, InvScreenWidth, InvScreenHeight) * 2.0f * float4(0.5f, 0.5f, -0.5f, 0.0f);
//	half4 Acc;
//	Acc = gDepthInput.SampleLevel(gsamLinearClamp, Tex - duv.xy, 0.0f);
//	Acc += gDepthInput.SampleLevel(gsamLinearClamp, Tex - duv.zy, 0.0f);
//	Acc += gDepthInput.SampleLevel(gsamLinearClamp, Tex + duv.zy, 0.0f);
//	Acc += gDepthInput.SampleLevel(gsamLinearClamp, Tex + duv.xy, 0.0f);
//	return Acc / 4.0;
}