float3 SchlickFresnel(float3 R0, float3 normal, float3 lightVec)
{
    float cosIncidentAngle = saturate(dot(normal, lightVec));

    float f0 = 1.0f - cosIncidentAngle;
    float3 reflectPercent = R0 + (1.0f - R0)*(f0*f0*f0*f0*f0);

    return reflectPercent;
}

float3 BlinnPhong(float3 lightStrength, float3 lightDir, float3 normal, float3 view, float Roughness, float4 Diffuse, float3 R0)
{
    const float ShinyExp = (1- Roughness) * 256.0f;
    float3 halfVec = normalize(view + lightDir);

    float roughnessFactor = (ShinyExp + 8.0f) * pow(max(dot(halfVec, normal), 0.0f), ShinyExp) / 8.0f;


    float3 fresnelFactor = SchlickFresnel(R0, halfVec, lightDir);

    float3 specAlbedo = fresnelFactor*0.1;


    return (Diffuse.rgb + specAlbedo) * lightStrength;
}


float3 ComputeDirectionalLight(float3 lightDir, float3 normal, float lightStrength, float3 view, float Roughness, float4 Diffuse, float3 R0)
{
    float3 n = normalize(normal);
    float3 l = normalize(lightDir);
//   float NdotL = max(dot(lightDir, normal), 0.0f);
    float NdotL = clamp(dot(n, l), 0.0, 1.0);

    float3 LightStrength = lightStrength * NdotL;

    return BlinnPhong(LightStrength, lightDir, normal, view, Roughness, Diffuse, R0);
}