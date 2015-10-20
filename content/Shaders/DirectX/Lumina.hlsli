//--------------
// Shader Common
//--------------
float PI = 3.14159265359;

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

struct LightData
{
	float4 color;
	float3 direction;
	float  intensity;
	float3 position;
	float  range;
	float  spot;
	float3 pad;
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

cbuffer lighting : register(b3)
{
	float4 ambientLight;
	LightData directionalLight;
	LightData pointLight;
	LightData spotLight;
}

float3 UnpackNormals(float3 samplevalue)
{
	return samplevalue * 2.0f - 1.0f;
}