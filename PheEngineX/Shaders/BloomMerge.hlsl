#include "StaticSamplers.hlsli"
Texture2D gBloomUp				: register(t0);
Texture2D gBloomDown			: register(t6);

cbuffer cbRenderTargetSize : register(b4)
{
	int2 RenderTargetSize;
	int2 Scalar;
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

float2 Circle(float Start, float Points, float Point)
{
	float Radians = (2.0f * 3.141592f * (1.0f / Points)) * (Start + Point);
	return float2(cos(Radians), sin(Radians));
}


VS_Output VS(VertexIn vin)
{
	VS_Output output;

	output.position = float4(vin.PosL, 1);

	return output;
}

float4 PS(VS_Output Input) : SV_Target
{
	float4 OutColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float ScaleUV = 0.66f / 2.0f;
	float4 BloomColor1 = float4(0.5016f, 0.5016f, 0.5016f, 0.0f);

    int X = floor(Input.position.x);
	int Y = floor(Input.position.y);

    float2 Tex;
	Tex.x = 1.0f * X / RenderTargetSize[0];
	Tex.y = 1.0f * Y / RenderTargetSize[1];

	float DeltaU = 1.0f / RenderTargetSize[0];
	float DeltaV = 1.0f / RenderTargetSize[1];
	float2 DeltaUV = float2(DeltaU, DeltaV);

    float Start = 2.0f / 6.0f;
	float4 Color0 = gBloomDown.Sample(gsamLinearClamp, Tex + DeltaUV * ScaleUV * Circle(Start, 6.0, 0.0f));
	float4 Color1 = gBloomDown.Sample(gsamLinearClamp, Tex + DeltaUV * ScaleUV * Circle(Start, 6.0, 1.0f));
	float4 Color2 = gBloomDown.Sample(gsamLinearClamp, Tex + DeltaUV * ScaleUV * Circle(Start, 6.0, 2.0f));
	float4 Color3 = gBloomDown.Sample(gsamLinearClamp, Tex + DeltaUV * ScaleUV * Circle(Start, 6.0, 3.0f));
	float4 Color4 = gBloomDown.Sample(gsamLinearClamp, Tex + DeltaUV * ScaleUV * Circle(Start, 6.0, 4.0f));
	float4 Color5 = gBloomDown.Sample(gsamLinearClamp, Tex + DeltaUV * ScaleUV * Circle(Start, 6.0, 5.0f));
	float4 Color6 = gBloomDown.Sample(gsamLinearClamp, Tex);

    float ScaleColor1 = 1.0f / 7.0f;
	float ScaleColor2 = 1.0f / 7.0f;
    float4 BloomColor = Color6 * ScaleColor1 +
		Color0 * ScaleColor2 +
		Color1 * ScaleColor2 +
		Color2 * ScaleColor2 +
		Color3 * ScaleColor2 +
		Color4 * ScaleColor2 +
		Color4 * ScaleColor2 * rcp(ScaleColor1 * 1.0f + ScaleColor2 * 6.0f);

    OutColor.rgb = gBloomUp.Sample(gsamLinearClamp, Tex).rgb;

	float ScaleColor3 = 1.0f / 5.0f;
	OutColor.rgb *= ScaleColor3;


	OutColor.rgb += (BloomColor * ScaleColor3 * BloomColor1).rgb;
	OutColor.a = 1.0f;

	return OutColor;
}