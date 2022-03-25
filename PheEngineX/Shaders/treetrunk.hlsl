Texture2D    gDiffuseMap : register(t0);
Texture2D    gNormalMap : register(t1);
Texture2D    gShadowMap : register(t2);


SamplerState gsamPointWrap : register(s0);
SamplerState gsamPointClamp : register(s1);
SamplerState gsamLinearWrap : register(s2);
SamplerState gsamLinearClamp : register(s3);
SamplerState gsamAnisotropicWrap : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);
SamplerComparisonState gsamShadow : register(s6);


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
	float4x4 gCameraPositionMat;
	float4x4 gShadowTransform;
	float gTime;
};

cbuffer cbMaterial : register(b2)
{
	float4 gDiffuseAlbedo;
	float3 gFresnelR0;
	float  gRoughness;
}

struct VertexIn
{
	float3 PosL  : POSITION;
	float4 Normal : NORMAL;
	float2 TextCoord : TEXTCOORD;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
	float4 Color : COLOR;
	float2 TextCoord : TEXTCOORD;
	float4 ShadowPos : POSITION0;
	float3 WorldPos : POSITION1;
};

float CalcShadowFactor(float4 shadowPosH)
{
	shadowPosH.xyz /= shadowPosH.w;

	float depth = shadowPosH.z;

	uint width, height, numMips;
	gShadowMap.GetDimensions(0, width, height, numMips);

	//	float2 PixelPos = shadowPosH.xy*width;
	//	float depthInMap = gShadowMap.SampleLevel(gsamAnisotropicWrap, shadowPosH.xy, 0).r;
	//	return depth > depthInMap? 0:1;

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


VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	float4x4 gWorld = mul(gPosition, mul(gRotation, gScale));
	float4 PosWorld = mul(gWorld, float4(vin.PosL, 1.0f));
	float4 PosView = mul(gView, float4(PosWorld));
	float4 TPosH = mul(gProj, PosView);
	if (vin.PosL.z > 5.2)
	{
		vout.PosH = float4(TPosH.xyz, 0);
	}
	if (vin.PosL.z <= 5.2)
	{
		vout.PosH = TPosH;
	}
	if (distance(float3(0, 0, 0), float3(vin.PosL.x, vin.PosL.y, 0)) > 2)
	{
		vout.PosH = float4(TPosH.xyz, 0);
	}

	vout.Color = float4(normalize(mul(gRotation, vin.Normal).xyz), 1);

	vout.TextCoord = vin.TextCoord;

	vout.ShadowPos = mul(gShadowTransform, PosWorld);
	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	float4 diffuseAlbedo = gDiffuseMap.Sample(gsamPointWrap, pin.TextCoord) * gDiffuseAlbedo;
	float4 NormalAlbedo = gNormalMap.Sample(gsamPointWrap, pin.TextCoord) * gDiffuseAlbedo;

	float Shadow = CalcShadowFactor(pin.ShadowPos);


	return pow(diffuseAlbedo * (Shadow + 0.1), 1 / 2.2f);
}