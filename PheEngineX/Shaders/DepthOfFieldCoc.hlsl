#include "StaticSamplers.hlsli"
#include "ShaderDefines.h"

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
//far->up   close->down
float ViewDepth(float depth)
{
	return (FAR_Z * NEAR_Z) / (FAR_Z - depth * (FAR_Z - NEAR_Z));
}

float ScaleDepth(float depth)
{

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

	float SampleDepth = gDepthInput.SampleLevel(gsamAnisotropicWrap, Tex, 0.0f).r;

	float Depth = ViewDepth(SampleDepth);

	float coc = (Depth - FocusDistance) / FocusRange;
	coc = clamp(coc, -1, 1);
	if (coc < 0) {
		return coc * -half4(1, 0, 0, 1);
	}
	return coc;

}