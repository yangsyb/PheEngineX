#include "StaticSamplers.hlsli"
#include "ShaderDefines.h"

Texture2D gDepthInput	: register(t0);

cbuffer cbDoF : register(b4)
{
	float DOFFilmHeight;
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

float ViewDepth(float depth)
{
	return (FAR_Z * NEAR_Z) / (FAR_Z - depth * (FAR_Z - NEAR_Z));
}

VS_Output VS(VertexIn vin)
{
	VS_Output output;

	output.position = float4(vin.PosL, 1);

	return output;
}

float PS(VS_Output Input) : SV_Target
{
    int X = floor(Input.position.x);
	int Y = floor(Input.position.y);

	float2 Tex;
	Tex.x = 1.0f * X / ScreenWidth;
	Tex.y = 1.0f * Y / ScreenHeight;

	float ScaledFilmHeight = DOFFilmHeight * (ScreenHeight / 1080.f);
	float F = 1000.f/1000.f;
	float Distance = max(2000.f, F);
	float Aspect = ScreenHeight/ScreenWidth;
	float Coeff = F * F/(3.5 * (Distance - F) * ScaledFilmHeight * 2.0f);
	float RadiusInPixels = 10.f;
	float MaxCoc = min(0.05f, RadiusInPixels / ScreenHeight);
	return gDepthInput.SampleLevel(gsamAnisotropicWrap, Tex, 0.0f).r;
    float Depth = ViewDepth(gDepthInput.SampleLevel(gsamLinearClamp, Tex, 0.0f).r);
	return Depth;
    half Coc = (Depth - Distance) * Coeff / max(Depth, 1e-5);
	return saturate(Coc * 0.5 * MaxCoc + 0.5);
}