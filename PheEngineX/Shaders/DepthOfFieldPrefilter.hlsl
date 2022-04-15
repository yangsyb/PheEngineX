#include "StaticSamplers.hlsli"
#include "ShaderDefines.h"

Texture2D gCocTexture	: register(t0);
Texture2D gInputTexture : register(t6);

cbuffer cbDoF : register(b4)
{
	float FocusDistance;
	float FocusRange;
	float ScreenWidth;
	float ScreenHeight;
    float BokeRadius;
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

    float3 duv = float3(InvScreenWidth, InvScreenHeight, InvScreenWidth) * float3(0.5, 0.5, -0.5);
    float2 uv0 = Tex - duv.xy;
	float2 uv1 = Tex - duv.zy;
	float2 uv2 = Tex + duv.zy;
	float2 uv3 = Tex + duv.xy;

	half3 c0 = gInputTexture.SampleLevel(gsamLinearClamp , uv0, 0.0f).rgb;
	half3 c1 = gInputTexture.SampleLevel(gsamLinearClamp , uv1, 0.0f).rgb;
	half3 c2 = gInputTexture.SampleLevel(gsamLinearClamp , uv2, 0.0f).rgb;
	half3 c3 = gInputTexture.SampleLevel(gsamLinearClamp , uv3, 0.0f).rgb;

	half coc0 = gCocTexture.SampleLevel(gsamLinearClamp , uv0, 0.0f).r * 2.0 - 1.0;
	half coc1 = gCocTexture.SampleLevel(gsamLinearClamp , uv1, 0.0f).r * 2.0 - 1.0;
	half coc2 = gCocTexture.SampleLevel(gsamLinearClamp , uv2, 0.0f).r * 2.0 - 1.0;
	half coc3 = gCocTexture.SampleLevel(gsamLinearClamp , uv3, 0.0f).r * 2.0 - 1.0;

    float w0 = abs(coc0) / (max(max(c0.r, c0.g), c0.b) + 1.0);
	float w1 = abs(coc1) / (max(max(c1.r, c1.g), c1.b) + 1.0);
	float w2 = abs(coc2) / (max(max(c2.r, c2.g), c2.b) + 1.0);
	float w3 = abs(coc3) / (max(max(c3.r, c3.g), c3.b) + 1.0);

    half3 avg = c0 * w0 + c1 * w1 + c2 * w2 + c3 * w3;
	avg /= max(w0 + w1 + w2 + w3, 1e-5);

    half coc_min = min(coc0, min(min(coc1, coc2), coc3));
	half coc_max = max(coc0, max(max(coc1, coc2), coc3));
	half coc = (-coc_min > coc_max ? coc_min : coc_max) * 0.05;

	return half4(avg, coc);

//    avg *= smoothstep(0, InvScreenHeight * 2, abs(coc));

//	return half4(avg, coc);
}