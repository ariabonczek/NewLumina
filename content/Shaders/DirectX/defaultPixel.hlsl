//---------------------
// Default Pixel Shader
//---------------------
#include "Lumina.hlsli"

//cbuffer Properties : register(b1)
//{
//	float4 tint;
//	float roughness;
//	float metalness;
//}
//
//Texture2D _Albedo : register(t0);
//Texture2D _Normal : register(t1);
//Texture2D _Metalness : register(t2);
//
//sampler _Sampler : register(s0);

float4 main(MeshVertexOutput i) : SV_Target0
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}