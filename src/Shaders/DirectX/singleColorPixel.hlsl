#include "Lumina.hlsli"

cbuffer Properties : register(b1)
{
	float4 tint;
	float roughness;
	float metalness;
}

GBuffer main(MeshVertexOutput i)
{
	GBuffer o;

	i.normal = i.normal * 0.5f + 0.5f;

	o.albedo.rgb = tint.rgb;
	o.albedo.a = roughness;
	o.normal.rgb = i.normal;
	o.normal.a = metalness;

	return o;
}