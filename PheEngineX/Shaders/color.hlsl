#include "BRDF.hlsli"

Texture2D    gDiffuseMap : register(t0);
Texture2D    gNormalMap : register(t1);
Texture2D    gShadowMap : register(t4);


SamplerState gsamPointWrap : register(s0);
SamplerState gsamPointClamp : register(s1);
SamplerState gsamLinearWrap : register(s2);
SamplerState gsamLinearClamp : register(s3);
SamplerState gsamAnisotropicWrap : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);
SamplerComparisonState gsamShadow : register(s6);

//#define PI 3.1415927

cbuffer cbPerObject : register(b0)
{
	//	float4x4 gWorld;
	float4x4 gPosition;
	float4x4 gRotation;
	float4x4 gScale;
};

cbuffer cbPass : register(b1)
{
	float4x4 gView;
	float4x4 gProj;
	float4x4 gShadowTransform;
	float3 gCameraPosition;
	float gTime;
};

cbuffer cbMaterial : register(b2)
{
	float4 gBaseColor;
	float3 gFresnelR0;
	float  gRoughness;
}

cbuffer cbLight : register(b3)
{
	float3 gLightPosition;
	float gLightIntensity;
	float3 gLightColor;
	float gLightRadius;
}

struct VertexIn
{
	float3 PosL  : POSITION;
	float4 Normal : NORMAL;
	float4 Tangent : TANGENT;
	float2 TextCoord : TEXTCOORD;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
	float4 Color : COLOR;
	float2 TextCoord : TEXTCOORD;
	float4 ShadowPos : POSITION0;
	float3 WorldPos : POSITION1;
	float3 Norm : NORMAL;
	float3 TangentW : TANGENT;
};

float CalcShadowFactor(float4 shadowPosH)
{
	shadowPosH.xyz /= shadowPosH.w;

	float depth = shadowPosH.z;

	uint width, height, numMips;
	gShadowMap.GetDimensions(0, width, height, numMips);

	//		float2 PixelPos = shadowPosH.xy*width;
	//		float depthInMap = gShadowMap.SampleLevel(gsamAnisotropicWrap, shadowPosH.xy, 0).r;
	//		float depthInMap = gShadowMap.Load(int3(PixelPos, 0)).r;
	//		return depth > depthInMap? 0:1;

	float dx = 1.0f / (float)width;

	float percentLit = 0.0f;
	const float2 offsets[9] =
	{
		float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
		float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
		float2(-dx, +dx), float2(0.0f, +dx), float2(dx, +dx)
	};

	[unroll]
	for (int i = 0; i < 9; ++i)
	{
		//percentLit += gShadowMap.SampleLevel(gsamAnisotropicWrap, shadowPosH.xy + offsets[i], 0).r - depth > 0 ? 1 : 0;
		percentLit += gShadowMap.SampleCmpLevelZero(gsamShadow, shadowPosH.xy + offsets[i], depth).r;
	}

	return percentLit / 9.0f;
}

//float D_GGX(float a2, float NoH)
//{
//	float d = (NoH * a2 - NoH) * NoH + 1; // 2 mad
//	return a2 / (PI * d * d); // 4 mul, 1 rcp
//}

float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
	float3 normalT = 2.0f * normalMapSample - 1.0f;

	float3 N = normalize(unitNormalW);
	float3 T = normalize(tangentW - dot(tangentW, N) * N);
	float3 B = normalize(cross(N, T));

	float3x3 TBN = float3x3(T, B, N);

	float3 bumpedNormalW = mul(normalT, TBN);
	return bumpedNormalW;
}


VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	float4x4 gWorld = mul(gPosition, mul(gRotation, gScale));
	float4 PosWorld = mul(gWorld, float4(vin.PosL, 1.0f));
	float4 PosView = mul(gView, float4(PosWorld));
	vout.PosH = mul(gProj, PosView);

	vout.Color = float4(normalize(mul(gRotation, vin.Normal).xyz), 1);

	vout.WorldPos = PosWorld;

	vout.TextCoord = vin.TextCoord;

	vout.ShadowPos = mul(gShadowTransform, PosWorld);

	vout.Norm = mul(gWorld, float4(vin.Normal.xyz,0)).xyz;

	vout.TangentW = mul((float3x3)gWorld, normalize(vin.Tangent.xyz));

	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	float4 Output = 0.0f;

	float4 BaseColor = gDiffuseMap.Sample(gsamPointWrap, pin.TextCoord);
	BaseColor.rgb *= gBaseColor.rgb;


	float Shadow = CalcShadowFactor(pin.ShadowPos);

	float4 NormalMapSample = gNormalMap.Sample(gsamAnisotropicWrap, pin.TextCoord);
	float3 bumpedNormalW = NormalSampleToWorldSpace(NormalMapSample.rgb, pin.Norm, pin.TangentW);

	BaseColor.rgb *= gBaseColor.rgb;

	float Roughness = gRoughness;
	float Metallic = gFresnelR0;
	float F0 = 0.04f;
	float AO = 20.0f;
	F0 = lerp(F0.rrr, BaseColor.rgb, Metallic);

	//Light
	{
		float3 PointLightPos = gLightPosition;
		float LightRadius = gLightRadius;
		float LightStrenth = gLightIntensity;

		float3 WPos = pin.WorldPos;
		float FallOff = distance(PointLightPos, WPos);
		FallOff = LightRadius / (FallOff * FallOff);
		float3 V = normalize(gCameraPosition - pin.WorldPos);
		float3 N = bumpedNormalW;
//		float3 N = normalize(pin.Norm);
		float3 L = normalize(PointLightPos - WPos);
		float3 H = normalize(V + L);
		float3 R = -reflect(V, N);

		float NoL = saturate(dot(L, N));
		float NoH = saturate(dot(N, H));
		float NoV = saturate(dot(N, V));
		float VoH = saturate(dot(V, H));
		float NoR = saturate(dot(N, R));

		float3 Diffuse = Diffuse_Burley(BaseColor.rgb, Roughness, NoV, NoL, NoH);

		float a2 = Roughness * Roughness * Roughness * Roughness;
		float D = D_GGX(a2, NoH);
		float G = Vis_SmithJointApprox(a2, NoV, NoL);
		float F = FSchlick(VoH, F0);
		float3 Specular = D * G * F;
		Output.rgb += (Diffuse + Specular) * NoL * Shadow * (FallOff * LightStrenth) * 300;
	}

	Output.rgb += 0.03f * BaseColor.rgb * AO;
	return Output;

//	return pow(BaseColor * (Shadow + 0.1), 1 / 2.2f);
}