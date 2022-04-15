#include "StaticSamplers.hlsli"
Texture2D gScene			: register(t0);
Texture2D gSunMerge			: register(t6);

cbuffer cbRenderTargetSize : register(b4)
{
	int2 RenderTargetSize;
}

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


float3 ACESToneMapping(float3 color, float adapted_lum)
{
	const float A = 2.51f;
	const float B = 0.03f;
	const float C = 2.43f;
	const float D = 0.59f;
	const float E = 0.14f;

	color *= adapted_lum;
	return (color * (A * color + B)) / (color * (C * color + D) + E);
}

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
	Tex.x = 1.0f * X / RenderTargetSize[0];
	Tex.y = 1.0f * Y / RenderTargetSize[1];

    float4 SceneColor = gScene.Sample(gsamLinearWrap, Tex);
	float4 BloomColor = gSunMerge.Sample(gsamLinearWrap, Tex);

    half3 LinearColor = SceneColor.rgb + BloomColor.rgb;

    float4 OutColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	OutColor.rgb = ACESToneMapping(LinearColor, 1.0f);
	OutColor.a = SceneColor.a;

	return OutColor;
}

