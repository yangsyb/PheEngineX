Texture2D    gDiffuseMap : register(t0);
Texture2D    gNormalMap : register(t1);
SamplerState gsamLinear  : register(s0);


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
};


VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	float4x4 gWorld = mul(gPosition, mul(gRotation, gScale));
	float4 PosWorld = mul(gWorld, float4(vin.PosL, 1.0f));
	float4 PosView = mul(gView, float4(PosWorld));
	float4 TPosH = mul(gProj, PosView);
	if(vin.PosL.z > 5.2)
	{
		vout.PosH = float4(TPosH.xyz, 0);
	}
	if(vin.PosL.z <= 5.2)
	{
		vout.PosH = TPosH;
	}
	
	vout.Color = float4(normalize(mul(gRotation, vin.Normal).xyz), 1);

	vout.TextCoord = vin.TextCoord;
	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	float4 diffuseAlbedo = gDiffuseMap.Sample(gsamLinear, pin.TextCoord) * gDiffuseAlbedo;
	float4 NormalAlbedo = gNormalMap.Sample(gsamLinear, pin.TextCoord) * gDiffuseAlbedo;

	return pow(NormalAlbedo * diffuseAlbedo, 1 / 2.2f);

	//	float4 OutColor = pow(float4(pin.Color*0.5f+0.5f), 1/2.2f);
	//	return OutColor;

	//  return pow(diffuseAlbedo, 1 / 2.2f);
}