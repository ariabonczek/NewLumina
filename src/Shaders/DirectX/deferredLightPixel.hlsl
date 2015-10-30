//---------------------
// Default Pixel Shader
//---------------------
#include "Lumina.hlsli"

cbuffer properties : register(b1)
{
	matrix invViewProjection;
	float width;
	float height;
	uint lightType;
}

Texture2D _Albedo : register(t0);
Texture2D _Normal : register(t1);
Texture2D _Depth : register(t2);

sampler _Sampler : register(s0);
sampler _DepthSampler : register(s1);

float4 main(MeshVertexOutput i) : SV_Target0
{
	float2 uv = i.position.xy / float2(width, height);

	float4 albedo = _Albedo.Sample(_Sampler, uv);
	float4 normal = _Normal.Sample(_Sampler, uv);
	float depth = _Depth.Sample(_DepthSampler, uv);

	float x = uv.x * 2.0f - 1.0f;
	float y = (1.0 - uv.y) * 2.0f - 1.0f;
	float4 vProjectedPos = float4(x, y, depth, 1.0f);

	float4 worldpos = mul(vProjectedPos, invViewProjection);
	worldpos.xyz /= worldpos.w;

	float3 view = normalize(worldpos - eyePos);

	normal.xyz = normal.xyz * 2.0f - 1.0f;

	float4 lightColor = float4(0.0, 0.0, 0.0, 1.0);
	if (lightType == 0)
	{
		//lightColor = CalculateDirectionalLight(light, normal.rgb, view);
		lightColor = float4(PBRCalculateFinalColor(
			float4(albedo.rgb, 1.0), normal.rgb, albedo.a, normal.a, view, -light.direction, light).rgb, 1.0);
	}
	else if (lightType == 1)
	{
		lightColor = CalculatePointLight(light, worldpos, normal.rgb, view);
		//lightColor = PBRCalculateFinalColor(
		//	float4(albedo.rbg, 1.0), normal.rgb, albedo.a, normal.a, view, -normalize(worldpos - light.position), light);
	}
	else if (lightType == 2)
	{
		lightColor = CalculateSpotLight(light, worldpos, normal.rgb, view);
	}

	return float4(lightColor.rgb, 1.0f);
}