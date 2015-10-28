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

struct GBuffer
{
	float4 albedo	: SV_Target0;
	float4 normal	: SV_Target1;
};

struct FullScreenQuadInput
{
	float3 position : POSITION;
	float2 texcoord : TEXCOORD0;
};

struct FullScreenQuadOutput
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

struct LightData
{
	float4 color;
	float3 direction;
	float  intensity;
	float3 position;
	float  range;
	float3 bidirectionalColor;
	float  spot;
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
	//float4 ambientLight;
	//LightData directionalLight[1];
	//LightData pointLight[3];
	//LightData spotLight[3];
	//uint numDirectionalLights;
	//uint numPointLights;
	//uint numSpotLights;

	LightData light;

}

float3x3 MakeView(float3 view)
{
	view = normalize(view);
	float3 x, y, z;
	z = -view;
	x = normalize(float3(z.z, 0, -z.x));
	y = cross(z, x);
	return float3x3(x, y, z);
}

float4 EncodeNormal(float3 normal, float3 view)
{
	return normalize(float4(mul(MakeView(view), normal).xy * 0.5 + 0.5, 0, 0));
}

float3 DecodeNormal(float4 encoded, float3 view)
{
	float3 n;
	n.xy = encoded * 2.0 - 1.0;
	n.z = sqrt(1.0 + dot(n.xy, -n.xy));
	n = mul(n, MakeView(view));
	return n;
}

float3 UnpackNormals(float3 samplevalue)
{
	return saturate(samplevalue * 2.0f - 1.0f);
}

//------------------//
// Forward Lighting //
//------------------//

float4 CalculateDirectionalLight(LightData dLight, float3 normal, float3 view)
{
	float3 lightVector = -dLight.direction;
	
	// Diffuse
	float diffuseFactor = max(0.0f, dot(lightVector, normal));
	float bidirectionalFactor = max(0.0f, dot(-lightVector, normal));
	float4 diffuseLight = saturate(dLight.color * dLight.intensity * diffuseFactor);
	float4 bidirectionalLight = saturate(float4(dLight.bidirectionalColor, 1.0) * dLight.intensity * bidirectionalFactor);

	// Specular
	float3 halfVec = normalize(reflect(lightVector, normal));
	float specularFactor = pow(max(0.0f, dot(view, halfVec)), 32);		// hardcoded specular factor
	float4 specularLight = saturate(dLight.color * dLight.intensity * specularFactor);

	return diffuseLight + bidirectionalLight;// +specularLight;
}

float4 CalculatePointLight(LightData pLight, float3 position, float3 normal, float3 view)
{
	float3 lightVector = pLight.position - position;
	float d = length(lightVector);

	if (d > pLight.range)
		return float4(0.0f, 0.0f, 0.0f, 1.0f);

	lightVector /= d;

	// Diffuse
	float diffuseFactor = max(0.0f, dot(lightVector, normal));
	float4 diffuseLight = saturate(pLight.color * pLight.intensity * diffuseFactor);

	// Specular
	float3 halfVec = normalize(reflect(lightVector, normal));
	float specularFactor = pow(max(0.0f, dot(view, halfVec)), 8);		// hardcoded specular factor
	float4 specularLight = saturate(pLight.color * pLight.intensity * specularFactor);

	float attenuation = 1.0f / (1.0f + 0.1f * d +0.01f * d * d);

	diffuseLight *= attenuation;
	specularLight *= attenuation;

	return diffuseLight;// +specularLight;
}

float4 CalculateSpotLight(LightData sLight, float3 position, float3 normal, float3 view)
{
	float3 lightVector = sLight.position - position;
	float d = length(lightVector);

	if (d > sLight.range)
		return float4(0.0f, 0.0f, 0.0f, 1.0f);
	lightVector /= d;

	// Diffuse
	float diffuseFactor = max(0.0f, dot(lightVector, normal));
	float4 diffuseLight = saturate(sLight.color * sLight.intensity * diffuseFactor);

	// Specular
	float3 halfVec = normalize(reflect(lightVector, normal));
	float specularFactor = pow(max(0.0f, dot(view, halfVec)), 64);		// hardcoded specular factor
	float4 specularLight = saturate(sLight.color * sLight.intensity * specularFactor);

	float spot = pow(max(dot(-lightVector, sLight.direction), 0.0f), sLight.spot);

	float attenuation = spot / (1.0f + 0.1f * d + 0.01f * d * d);
	diffuseLight *= attenuation;
	specularLight *= attenuation;

	return diffuseLight;// +specularLight;
}

//float4 CalculateAllLights(
//	LightData directionalLights[1], uint numDL,
//	LightData pointLights[3], uint numPL,
//	LightData spotLights[3], uint numSL,
//	float3 position, float3 normal, float3 view)
//{
//	float4 ret = float4(0.0f, 0.0f, 0.0f, 1.0f);
//
//	for (uint i = 0; i < numDL; i++)
//	{
//		ret += CalculateDirectionalLight(directionalLights[i], normal, view);
//	}
//
//	for (uint j = 0; j < numPL; j++)
//	{
//		ret += CalculatePointLight(pointLights[j], position, normal, view);
//	}
//
//	for (uint k = 0; k < numSL; k++)
//	{
//		ret += CalculateSpotLight(spotLight[k], position, normal, view);
//	}
//
//	return ret;
//}

//---------------------------//
// Physically-Based Lighting //
//---------------------------//

float4 LambertDiffuse(float4 diffuse)
{
	return diffuse;
}

float NormalDistribution(float roughness, float3 normal, float3 halfvector)
{
	float alpha = roughness * roughness;
	float alpha2 = alpha*alpha;
	float ndotd = saturate(dot(normal, halfvector));
	float denom = ((ndotd*ndotd) * (alpha2 - 1) + 1);

	return alpha2 / denom*denom;
}

float GaussianFresnelReflectance(float metalness, float3 normal, float3 halfvector)
{
	return metalness + (1 - metalness) * pow((1 - dot(normal, halfvector)), 5);
}

float GeometricAttenuationInternal(float k, float3 normal, float3 _vector)
{
	float ndotv = saturate(dot(normal, _vector));

	return ndotv / (ndotv * (1 - k) + k);
}

float GeometricAttenuation(float roughness, float3 normal, float3 viewvector, float3 lightvector)
{
	float k = ((roughness + 1) * (roughness + 1)) / 8;

	return GeometricAttenuationInternal(k, normal, viewvector) * GeometricAttenuationInternal(k, normal, lightvector);
}

float CookTorrenceMicrofacetSpecular(float roughness, float metalness, float3 normal, float3 viewvector, float3 halfvector, float3 lightvector)
{
	float N = NormalDistribution(roughness, normal, halfvector);
	float G = GeometricAttenuation(roughness, normal, viewvector, lightvector);
	
	float denom = 1.0f / (4 * saturate(dot(normal, lightvector)) * saturate(dot(normal, viewvector)));

	return N * G * denom;
}

float4 PBRCalculateFinalColor(float4 diffuse, float3 normal, float roughness, float metalness, float3 viewvector, float3 lightVector, LightData light)
{
	float3 halfvector = normalize(lightVector + viewvector);

	float alpha = exp2(10 * (1 - roughness) + 1);

	float spec = CookTorrenceMicrofacetSpecular(roughness, metalness, normal, viewvector, halfvector, lightVector);

	return (diffuse + spec) * (light.color * light.intensity);
	//return (diffuse * (dot(normal, lightVector)) +
	//	GaussianFresnelReflectance(metalness, lightVector, halfvector) * ((alpha + 2) / 8) * 
	//	pow(saturate(dot(normal, halfvector)), alpha) * dot(normal, lightVector));
}