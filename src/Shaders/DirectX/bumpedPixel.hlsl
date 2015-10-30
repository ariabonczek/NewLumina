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
Texture2D _Normal : register(t1);

sampler _Sampler : register(s0);

GBuffer main(MeshVertexOutput i)
{
	GBuffer o;

	float3 normalT = UnpackNormals(_Normal.Sample(_Sampler, i.texcoord * float2(tileX, tileY)).rgb);
	float3 N = i.normal;
	float3 T = normalize(i.tangent - dot(i.tangent, N) * N);
	float3 B = cross(N, T);
	float3x3 TBN = float3x3(T, B, N);
	float3 bumpedNormal = normalize(mul(normalT, TBN));

	float3 viewvector = normalize(i.worldpos - eyePos);

	bumpedNormal = bumpedNormal * 0.5f + 0.5f;

	float4 diffuse = _Albedo.Sample(_Sampler, i.texcoord * float2(tileX, tileY));

	o.albedo.rgb = diffuse.rgb;
	o.albedo.a = roughness;
	o.normal.rgb = bumpedNormal;
	o.normal.a = metalness;
	return o;
}