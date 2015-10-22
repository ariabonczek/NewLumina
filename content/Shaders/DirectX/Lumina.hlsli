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
	LightData directionalLight[1];
	LightData pointLight[3];
	LightData spotLight[3];
	uint numDirectionalLights;
	uint numPointLights;
	uint numSpotLights;
}

float3 UnpackNormals(float3 samplevalue)
{
	return samplevalue * 2.0f - 1.0f;
}

//------------------//
// Forward Lighting //
//------------------//

float4 CalculateDirectionalLight(LightData dLight, float3 normal, float3 view)
{
	float3 lightVector = -dLight.direction;
	
	// Diffuse
	float diffuseFactor = max(0.0f, dot(lightVector, normal));
	float4 diffuseLight = saturate(dLight.color * dLight.intensity * diffuseFactor);

	// Specular
	float3 halfVec = normalize(reflect(lightVector, normal));
	float specularFactor = pow(max(0.0f, dot(view, halfVec)), 64);		// hardcoded specular factor
	float4 specularLight = saturate(dLight.color * dLight.intensity * specularFactor);

	return diffuseLight + specularLight;
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
	float specularFactor = pow(max(0.0f, dot(view, halfVec)), 64);		// hardcoded specular factor
	float4 specularLight = saturate(pLight.color * pLight.intensity * specularFactor);

	float attenuation = 1.0f / (1.0f + 0.1f * d + 0.01f * d * d);
	diffuseLight *= attenuation;
	specularLight *= attenuation;

	return diffuseLight +specularLight;
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

	return diffuseLight + specularLight;
}

float4 CalculateAllLights(
	LightData directionalLights[1], uint numDL,
	LightData pointLights[3], uint numPL,
	LightData spotLights[3], uint numSL,
	float3 position, float3 normal, float3 view)
{
	float4 ret = float4(0.0f, 0.0f, 0.0f, 1.0f);

	for (uint i = 0; i < numDL; i++)
	{
		ret += CalculateDirectionalLight(directionalLights[i], normal, view);
	}

	for (uint j = 0; j < numPL; j++)
	{
		ret += CalculatePointLight(pointLights[j], position, normal, view);
	}

	for (uint k = 0; k < numSL; k++)
	{
		ret += CalculateSpotLight(spotLight[k], position, normal, view);
	}

	return ret;
}

//---------------------------//
// Physically-Based Lighting //
//---------------------------//

float NormalDistribution(float roughness, float3 normal, float3 halfvector)
{
	float alpha = roughness * roughness;
	float alpha2 = alpha*alpha;
	float ndotd = dot(normal, halfvector);
	float denom = ((ndotd*ndotd) * (alpha2 - 1) + 1);

	return alpha2 / denom*denom;
}

float GaussianFresnelReflectance(float metalness, float3 viewvector, float3 halfvector)
{
	float vdoth = saturate(dot(viewvector, halfvector));

	return metalness + (1 - metalness) * exp2((5.55473 * vdoth - 6.98316) * vdoth);
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
	return
		(NormalDistribution(roughness, normal, halfvector) * GaussianFresnelReflectance(metalness, viewvector, halfvector) * GeometricAttenuation(roughness, normal, viewvector, lightvector)) /
		(4 * saturate(dot(normal, lightvector)) * saturate(dot(normal, viewvector)));
}

float4 PBRCalculateFinalColor(float4 diffuse, float3 normal, float roughness, float metalness, float3 viewvector, float3 lightVector, LightData light)
{
	float3 halfvector = normalize(lightVector + viewvector);

	float4 specular = CookTorrenceMicrofacetSpecular(roughness, metalness, normal, viewvector, halfvector, lightVector);

	return (diffuse /*+ specular*/) * (light.color * light.intensity * dot(normal, lightVector));
}