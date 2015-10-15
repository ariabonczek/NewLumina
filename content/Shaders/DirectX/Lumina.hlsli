struct MeshVertexInput
{
	float3 position : POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal   : NORMAL;
	float3 tangent  : TANGENT;
};

struct MeshVertexOutput
{
	float4 position : SV_POSITION;
	float3 worldpos : POSITION0;
	float2 texcoord	: TEXCOORD0;
	float3 normal   : NORMAL;
	float3 tangent  : TANGENT;
};

cbuffer perFrame : register(b0)
{
	matrix view;
	matrix projection;
	float3 eyePos;
	float elapsedTime;
}

cbuffer perObject : register(b2)
{
	matrix world;
	matrix worldInverseTranspose;
}