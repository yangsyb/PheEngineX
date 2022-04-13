Texture2D gHDRInput : register(t6);


SamplerState gsamPointWrap : register(s0);
SamplerState gsamPointClamp : register(s1);
SamplerState gsamLinearWrap : register(s2);
SamplerState gsamLinearClamp : register(s3);
SamplerState gsamAnisotropicWrap : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);
SamplerComparisonState gsamShadow : register(s6);

int2 RenderTargetSize : register(b4);

float Luminance(float3 InColor)
{
	return dot(InColor, float3(0.3f, 0.59f, 0.11f));
}

float4 PS(float4 position : SV_POSITION) : SV_TARGET
{
    const float BloomThreshold = 1.0f;

    
    float4 OutColor;

    return OutColor;
}