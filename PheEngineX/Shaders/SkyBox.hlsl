TextureCube Skybox;

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

VertexOut VS(VertexIn IN)
{
	VertexOut Output;

	Output.ObjPos = IN.PosL;

	VP = mul(gProj, gView);

	Output.PosH = mul(VP, float4(IN.PosL + gCameraPosition, 1.0)).xyww;
	return Output;
}

float4 PS(VertexOut IN) : SV_Target
{
	return pow(Skybox.Sample(gsamLinearWrap, IN.ObjPos), 1 / 2.2);
}

