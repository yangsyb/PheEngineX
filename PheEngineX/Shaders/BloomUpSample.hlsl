Texture2D gBloomChain				: register(t0);
Texture2D gBloomInput				: register(t1);


struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv           : TEXCOORD0;
};

float4 UpsampleTent(Texture2D tex, float2 uv, float2 texelSize, float sampleScale)
{
	float4 d = texelSize.xyxy * float4(1.0, 1.0, -1.0, 0.0) * sampleScale;

	float4 s;
	s = tex.Sample(gsamLinearClamp, uv - d.xy);
	s += tex.Sample(gsamLinearClamp, uv - d.wy) * 2.0;
	s += tex.Sample(gsamLinearClamp, uv - d.zy);

	s += tex.Sample(gsamLinearClamp, uv + d.zw) * 2.0;
	s += tex.Sample(gsamLinearClamp, uv) * 4.0;
	s += tex.Sample(gsamLinearClamp, uv + d.xw) * 2.0;

	s += tex.Sample(gsamLinearClamp, uv + d.zy);
	s += tex.Sample(gsamLinearClamp, uv + d.wy) * 2.0;
	s += tex.Sample(gsamLinearClamp, uv + d.xy);

	return s * (1.0 / 16.0);
}

float4 UpsampleBox(Texture2D tex, float2 uv, float2 texelSize, float sampleScale)
{
	float4 d = texelSize.xyxy * float4(-1.0, -1.0, 1.0, 1.0) * (sampleScale * 0.5);

	float4 s;
	s = (tex.Sample(gsamLinearClamp, uv + d.xy));
	s += (tex.Sample(gsamLinearClamp, uv + d.zy));
	s += (tex.Sample(gsamLinearClamp, uv + d.xw));
	s += (tex.Sample(gsamLinearClamp, uv + d.zw));

	return s * (1.0 / 4.0);
}

float4 Combine(float4 bloom, float2 uv)
{
	float4 color = gBloomInput.Sample(linearClampSampler, uv);
	return bloom + color;
}

float4 PS(VertexToPixel i) : SV_Target
{
#if BLOOM_USE_TENT_UPSAMPLE
	float4 bloom = UpsampleTent(gBloomChain, i.uv, float2(_BloomTexelSizeX, _BloomTexelSizeY), _SampleScale);
	return Combine(bloom, i.uv);
#else
	float4 bloom = UpsampleBox(gBloomChain, i.uv, float2(_BloomTexelSizeX, _BloomTexelSizeY), _SampleScale);
	return Combine(bloom, i.uv);
#endif
}