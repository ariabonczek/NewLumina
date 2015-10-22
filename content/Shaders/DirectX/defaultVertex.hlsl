//----------------------
// Default Vertex Shader
//----------------------

#include "Lumina.hlsli"

MeshVertexOutput main(MeshVertexInput input)
{
	MeshVertexOutput o;

	matrix wvp = mul(world, mul(view, projection));

	o.position = mul(float4(input.position, 1.0), wvp);
	o.worldpos = mul(float4(input.position, 1.0), world).xyz;
	o.texcoord = input.texcoord;
	o.normal = input.normal;
	o.tangent = input.tangent;

	//o.normal = mul(input.normal, (float3x3)worldInverseTranspose);
	//o.tangent = mul(input.tangent, (float3x3)worldInverseTranspose);

	return o;	
}