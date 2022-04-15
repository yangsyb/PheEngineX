#include "StaticSamplers.hlsli"
#include "ShaderDefines.h"
#define PI 3.1415927
Texture2D gDepthInput	: register(t0);

static const int kSampleCount = 22;
static const float2 kDiskKernel[kSampleCount] = {
	float2(0,0),
	float2(0.53333336,0),
	float2(0.3325279,0.4169768),
	float2(-0.11867785,0.5199616),
	float2(-0.48051673,0.2314047),
	float2(-0.48051673,-0.23140468),
	float2(-0.11867763,-0.51996166),
	float2(0.33252785,-0.4169769),
	float2(1,0),
	float2(0.90096885,0.43388376),
	float2(0.6234898,0.7818315),
	float2(0.22252098,0.9749279),
	float2(-0.22252095,0.9749279),
	float2(-0.62349,0.7818314),
	float2(-0.90096885,0.43388382),
	float2(-1,0),
	float2(-0.90096885,-0.43388376),
	float2(-0.6234896,-0.7818316),
	float2(-0.22252055,-0.974928),
	float2(0.2225215,-0.9749278),
	float2(0.6234897,-0.7818316),
	float2(0.90096885,-0.43388376),
};

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

    half4 DepthSample = gDepthInput.SampleLevel(gsamAnisotropicWrap, Tex, 0.0f);

    half4 BackGroundAcc;
    half4 ForeGroundAcc;

    float Aspect = ScreenHeight/ScreenWidth;
    [loop]
	for (int si = 0; si < kSampleCount; si++)
	{
		float2 disp = kDiskKernel[si] * 0.05;
		float dist = length(disp);

		float2 duv = float2(disp.x * Aspect, disp.y);
		half4 DepthSampleR = gDepthInput.SampleLevel(gsamAnisotropicWrap, Tex + duv, 0.0f);

		half BackGroundCoC = max(min(DepthSample.a, DepthSampleR.a), 0.0);


		const half Margin = InvScreenHeight * 2;
		half BackGroundWeight = saturate((BackGroundCoC - dist + Margin) / Margin);
		half ForeGroundWeight = saturate((-DepthSampleR.a - dist + Margin) / Margin);


		ForeGroundWeight *= step(InvScreenHeight, -DepthSampleR.a);

		BackGroundAcc += half4(DepthSampleR.rgb, 1.0) * BackGroundWeight;
		ForeGroundAcc += half4(DepthSampleR.rgb, 1.0) * ForeGroundWeight;
	}

    BackGroundAcc.rgb /= BackGroundAcc.a + (BackGroundAcc.a == 0.0); 
	ForeGroundAcc.rgb /= ForeGroundAcc.a + (ForeGroundAcc.a == 0.0);

	BackGroundAcc.a = smoothstep(InvScreenHeight, InvScreenHeight * 2.0, DepthSample.a);

	ForeGroundAcc.a *= PI / kSampleCount;

	half Alpha = saturate(ForeGroundAcc.a);
	half3 RGB = lerp(BackGroundAcc.rgb, ForeGroundAcc.rgb, Alpha);

	return half4(RGB, Alpha);
}