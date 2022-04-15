Texture2D gHDRInput : register(t6);

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

float Luminance(float3 InColor)
{
	return dot(InColor, float3(0.3f, 0.59f, 0.11f));
}

VS_Output VS(VertexIn vin)
{
    VS_Output output;

	output.position = float4(vin.PosL,1);

    return output;
}

float4 PS(VS_Output Input) : SV_TARGET
{
    const float BloomThreshold = 1.0f;

    int X = floor(Input.position.x);
	int Y = floor(Input.position.y);

    float Width = RenderTargetSize[0] * 0.25f;
	float Height = RenderTargetSize[1] * 0.25f;

    float DeltaU = 1.0f / RenderTargetSize[0];
	float DeltaV = 1.0f / RenderTargetSize[1];

	float2 Tex;
	Tex.x = 1.0f * X / Width;
	Tex.y = 1.0f * Y / Height;

	float4 Color0 = gHDRInput.Sample(gsamPointWrap, Tex + float2(-DeltaU, -DeltaV));
	float4 Color1 = gHDRInput.Sample(gsamPointWrap, Tex + float2(+DeltaU, -DeltaV));
	float4 Color2 = gHDRInput.Sample(gsamPointWrap, Tex + float2(-DeltaU, +DeltaV));
	float4 Color3 = gHDRInput.Sample(gsamPointWrap, Tex + float2(+DeltaU, +DeltaV));

    float4 AvailableColor = Color0 * 0.25f + Color1 * 0.25f + Color2 * 0.25f + Color3 * 0.25f;

	float TotalLuminance = Luminance(AvailableColor.rgb);
	float BloomLuminance = TotalLuminance - BloomThreshold;
//	float BloomLuminance = TotalLuminance > BloomThreshold? TotalLuminance: 0;
	float Amount = saturate(BloomLuminance * 0.5f);

	float4 OutColor;
	OutColor.rgb = AvailableColor.rgb;
	OutColor.rgb *= Amount;
	OutColor.a = 0.0f;

    return OutColor;
}