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


	float BloomWeightScalar = 1.0f / 10;
	float BloomWeightScalar1 = 1.0f / 10;

	float BloomUpScale = 1.32f;

	int X = floor(Input.position.x);
	int Y = floor(Input.position.y);

	float2 Tex;
	Tex.x = 1.0f * X / RenderTargetSize[0];
	Tex.y = 1.0f * Y / RenderTargetSize[1];

	float DeltaU = 1.0f / RenderTargetSize[0];
	float DeltaV = 1.0f / RenderTargetSize[1];
	float2 DeltaUV = float2(DeltaU, DeltaV);

	float Start = 2.0 / 7.0;
	float4 Color0 = gBloomUp.Sample(gsamLinearWrap, Tex + DeltaUV * BloomUpScale * Circle(Start, 7.0, 0.0f));
	float4 Color1 = gBloomUp.Sample(gsamLinearWrap, Tex + DeltaUV * BloomUpScale * Circle(Start, 7.0, 1.0f));
	float4 Color2 = gBloomUp.Sample(gsamLinearWrap, Tex + DeltaUV * BloomUpScale * Circle(Start, 7.0, 2.0f));
	float4 Color3 = gBloomUp.Sample(gsamLinearWrap, Tex + DeltaUV * BloomUpScale * Circle(Start, 7.0, 3.0f));
	float4 Color4 = gBloomUp.Sample(gsamLinearWrap, Tex + DeltaUV * BloomUpScale * Circle(Start, 7.0, 4.0f));
	float4 Color5 = gBloomUp.Sample(gsamLinearWrap, Tex + DeltaUV * BloomUpScale * Circle(Start, 7.0, 5.0f));
	float4 Color6 = gBloomUp.Sample(gsamLinearWrap, Tex + DeltaUV * BloomUpScale * Circle(Start, 7.0, 6.0f));
	float4 Color7 = gBloomUp.Sample(gsamLinearWrap, Tex);

	float4 Color8 = gBloomDown.Sample(gsamLinearWrap, Tex + DeltaUV * BloomUpScale * Circle(Start, 7.0, 0.0f));
	float4 Color9 = gBloomDown.Sample(gsamLinearWrap, Tex + DeltaUV * BloomUpScale * Circle(Start, 7.0, 1.0f));
	float4 Color10 = gBloomDown.Sample(gsamLinearWrap, Tex + DeltaUV * BloomUpScale * Circle(Start, 7.0, 2.0f));
	float4 Color11 = gBloomDown.Sample(gsamLinearWrap, Tex + DeltaUV * BloomUpScale * Circle(Start, 7.0, 3.0f));
	float4 Color12 = gBloomDown.Sample(gsamLinearWrap, Tex + DeltaUV * BloomUpScale * Circle(Start, 7.0, 4.0f));
	float4 Color13 = gBloomDown.Sample(gsamLinearWrap, Tex + DeltaUV * BloomUpScale * Circle(Start, 7.0, 5.0f));
	float4 Color14 = gBloomDown.Sample(gsamLinearWrap, Tex + DeltaUV * BloomUpScale * Circle(Start, 7.0, 6.0f));
	float4 Color15 = gBloomDown.Sample(gsamLinearWrap, Tex);

	float4 BloomWight = float4(BloomWeightScalar, BloomWeightScalar, BloomWeightScalar, 0.0f);
	float4 BloomWight1 = float4(BloomWeightScalar1, BloomWeightScalar1, BloomWeightScalar1, 0.0f);

	OutColor = (Color0 + Color1 + Color2 + Color3 + Color4 + Color5 + Color6 + Color7) * BloomWight +
		(Color8 + Color9 + Color10 + Color11 + Color12 + Color13 + Color14 + Color15) * BloomWight1;
	OutColor.a = 0.0f;


	return OutColor;
}