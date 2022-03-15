Texture2D    gDiffuseMap : register(t0);
SamplerState gsamLinear  : register(s0);

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
};

cbuffer treeProgressPass : register(b2)
{
	float gTime;
	float3 gCenterPosition;
}

cbuffer cbMaterial : register(b3)
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
	float swayscale = 0.5f;
	float swayspeed = 5.0f;

	float dis = distance(gCenterPosition, vin.PosL) - 2;
	float x = sin(vin.PosL.x * swayscale + gTime * swayspeed);
	float y = sin(vin.PosL.y * swayscale + gTime * swayspeed);

	float3 swayPos = float3(x * dis / 1.5, y * dis, vin.PosL.z);

	float4x4 gWorld = mul(gPosition, mul(gRotation, gScale));

	float4 PosWorld;
	if (vin.PosL.z > 5.2)
	{
		PosWorld = mul(gWorld, float4(swayPos, 1.0f));
	}
	if (vin.PosL.z <= 5.2)
	{
		PosWorld = mul(gWorld, float4(vin.PosL, 1.0f));
	}

	float4 PosView = mul(gView, float4(PosWorld));
	vout.PosH = mul(gProj, PosView);

	vout.Color = float4(normalize(mul(gRotation, vin.Normal).xyz), 1);
	vout.TextCoord = vin.TextCoord;

	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	float4 diffuseAlbedo = gDiffuseMap.Sample(gsamLinear, pin.TextCoord) * gDiffuseAlbedo;
//	float4 OutColor = pow(float4(pin.Color * 0.5f + 0.5f), 1 / 2.2f);
//	return OutColor;
	return pow(diffuseAlbedo, 1 / 2.2f);
}