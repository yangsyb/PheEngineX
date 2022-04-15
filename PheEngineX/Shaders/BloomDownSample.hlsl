Texture2D gBloomInput : register(t6);

SamplerState gsamPointWrap : register(s0);
SamplerState gsamPointClamp : register(s1);
SamplerState gsamLinearWrap : register(s2);
SamplerState gsamLinearClamp : register(s3);
SamplerState gsamAnisotropicWrap : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);
SamplerComparisonState gsamShadow : register(s6);

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

float4 PS(VS_Output Input) : SV_TARGET
{
	float BloomDownScale = 2.5f;	

    int X = floor(Input.position.x);
	int Y = floor(Input.position.y);

	float Width = RenderTargetSize[0] * 0.5f;
	float Height = RenderTargetSize[1] * 0.5f;

	float DeltaU = 1.0f / RenderTargetSize[0];
	float DeltaV = 1.0f / RenderTargetSize[1];

	float2 Tex;
	Tex.x = 1.0f * X / Width;
	Tex.y = 1.0f * Y / Height;


	float2 DeltaUV = float2(DeltaU, DeltaV);

	float StartRaduas = 2.0f / 14.0f;
	float4 Color0 = gBloomInput.Sample(gsamLinearWrap, Tex + DeltaUV * BloomDownScale * Circle(StartRaduas, 14.0f, 0.0f));
	float4 Color1 = gBloomInput.Sample(gsamLinearWrap, Tex + DeltaUV * BloomDownScale * Circle(StartRaduas, 14.0f, 1.0f));
	float4 Color2 = gBloomInput.Sample(gsamLinearWrap, Tex + DeltaUV * BloomDownScale * Circle(StartRaduas, 14.0f, 2.0f));
	float4 Color3 = gBloomInput.Sample(gsamLinearWrap, Tex + DeltaUV * BloomDownScale * Circle(StartRaduas, 14.0f, 3.0f));
	float4 Color4 = gBloomInput.Sample(gsamLinearWrap, Tex + DeltaUV * BloomDownScale * Circle(StartRaduas, 14.0f, 4.0f));
	float4 Color5 = gBloomInput.Sample(gsamLinearWrap, Tex + DeltaUV * BloomDownScale * Circle(StartRaduas, 14.0f, 5.0f));
	float4 Color6 = gBloomInput.Sample(gsamLinearWrap, Tex + DeltaUV * BloomDownScale * Circle(StartRaduas, 14.0f, 6.0f));
	float4 Color7 = gBloomInput.Sample(gsamLinearWrap, Tex + DeltaUV * BloomDownScale * Circle(StartRaduas, 14.0f, 7.0f));
	float4 Color8 = gBloomInput.Sample(gsamLinearWrap, Tex + DeltaUV * BloomDownScale * Circle(StartRaduas, 14.0f, 8.0f));
	float4 Color9 = gBloomInput.Sample(gsamLinearWrap, Tex + DeltaUV * BloomDownScale * Circle(StartRaduas, 14.0f, 9.0f));
	float4 Color10 = gBloomInput.Sample(gsamLinearWrap, Tex + DeltaUV * BloomDownScale * Circle(StartRaduas, 14.0f, 10.0f));
	float4 Color11 = gBloomInput.Sample(gsamLinearWrap, Tex + DeltaUV * BloomDownScale * Circle(StartRaduas, 14.0f, 11.0f));
	float4 Color12 = gBloomInput.Sample(gsamLinearWrap, Tex + DeltaUV * BloomDownScale * Circle(StartRaduas, 14.0f, 12.0f));
	float4 Color13 = gBloomInput.Sample(gsamLinearWrap, Tex + DeltaUV * BloomDownScale * Circle(StartRaduas, 14.0f, 13.0f));

	float4 Color = gBloomInput.Sample(gsamLinearWrap, Tex);

	float Weight = 1.0f / 15.0f;
	
	Color = Weight * (Color + Color0 + Color1 + Color2 + Color3
					+ Color4 + Color5 + Color6 + Color7
					+ Color8 + Color9 + Color10 + Color11 + Color12 + Color13);

	return Color;
}

