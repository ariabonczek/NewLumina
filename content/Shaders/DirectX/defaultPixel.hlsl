//---------------------
// Default Pixel Shader
//---------------------
#include "Lumina.hlsli"

cbuffer Properties : register(b1)
{
	float4 tint;
	float roughness;
	float metalness;
}

Texture2D _Albedo : register(t0);
Texture2D _Normal : register(t1);
Texture2D _Metalness : register(t2);

sampler _Sampler : register(s0);

float4 main(MeshVertexOutput i) : SV_Target0
{
	float3 normalT = UnpackNormals(_Normal.Sample(_Sampler, i.texcoord).rgb);
	float3 N = i.normal;
	float3 T = normalize(i.tangent - dot(i.tangent, N) * N);
	float3 B = cross(N, T);
	float3x3 TBN = float3x3(T, B, N);
	float3 bumpedNormal = normalize(mul(normalT, TBN));
	
	float diffFactor = dot(-directionalLight.direction, bumpedNormal);

	float4 lightFactor = diffFactor * directionalLight.intensity * directionalLight.color;
	
	return _Albedo.Sample(_Sampler, i.texcoord) * tint * lightFactor + ambientLight;
}