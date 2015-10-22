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
Texture2D _Metalness : register(t2);

sampler _Sampler : register(s0);

float4 main(MeshVertexOutput i) : SV_Target0
{
	float3 normalT = UnpackNormals(_Normal.Sample(_Sampler, i.texcoord * float2(tileX, tileY)).rgb);
	float3 N = i.normal;
	float3 T = normalize(i.tangent - dot(i.tangent, N) * N);
	float3 B = cross(N, T);
	float3x3 TBN = float3x3(T, B, N);
	float3 bumpedNormal = normalize(mul(normalT, TBN));

	float3 view = normalize(i.worldpos - eyePos);

	float4 lightFactor = CalculateAllLights(
		directionalLight, numDirectionalLights,
		pointLight,       numPointLights,
		spotLight,        numSpotLights,
		i.worldpos,		  bumpedNormal, view);

	//return PBRCalculateFinalColor(_Albedo.Sample(_Sampler, i.texcoord), bumpedNormal, 
	//	roughness, metalness, view, directionalLight[0].direction, directionalLight[0]);

	float4 diffuse = _Albedo.Sample(_Sampler, i.texcoord * float2(tileX, tileY));
    return diffuse *  tint * lightFactor + (diffuse * ambientLight);
}