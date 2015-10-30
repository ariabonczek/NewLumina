//---------------------
// Default Pixel Shader
//---------------------
#include "Lumina.hlsli"

cbuffer Properties : register(b1)
{
	float4 tint;
	float roughness;
	float metalness;
	uint tileX;
	uint tileY;
}

Texture2D _Albedo : register(t0);

sampler _Sampler : register(s0);

GBuffer main(MeshVertexOutput i)
{
	GBuffer o;

	float3 viewvector = normalize(i.worldpos - eyePos);

	i.normal = i.normal * 0.5f + 0.5f;

	float4 diffuse = _Albedo.Sample(_Sampler, i.texcoord * float2(tileX, tileY));

	o.albedo.rgb = diffuse.rgb;
	o.albedo.a = roughness;
	o.normal.rgb = i.normal;
	o.normal.a = metalness;
	return o;
}