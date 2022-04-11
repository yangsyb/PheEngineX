TextureCube Skybox : register(t5);

SamplerState gsamPointWrap : register(s0);
SamplerState gsamPointClamp : register(s1);
SamplerState gsamLinearWrap : register(s2);
SamplerState gsamLinearClamp : register(s3);
SamplerState gsamAnisotropicWrap : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);
SamplerComparisonState gsamShadow : register(s6);

cbuffer cbPerObject : register(b0)
{
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

struct VertexIn
{
	float3 PosL  : POSITION;
	float4 Normal : NORMAL;
	float4 Tangent : TANGENT;
	float4 TangentY : TANGENTY;
	float2 TextCoord : TEXTCOORD;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
	float3 ObjPos : POSITION;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	float4x4 gWorld = mul(gPosition, mul(gRotation, gScale));
	float4 PosWorld = mul(gWorld, float4(vin.PosL, 1.0f));
	float4x4 gViewProj = mul(gProj, gView);
	PosWorld.xyz += gCameraPosition;


	vout.PosH = mul(gProj, mul(gView, float4(gCameraPosition + vin.PosL, 1.0))).xyww;
	vout.ObjPos = vin.PosL;

	return vout;
}


float4 PS(VertexOut IN) : SV_Target
{
	return pow(Skybox.Sample(gsamLinearWrap, IN.ObjPos), 1 / 2.2);
}
