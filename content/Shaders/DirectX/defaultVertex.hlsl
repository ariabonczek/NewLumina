//----------------------
// Default Vertex Shader
//----------------------

#include "Lumina.hlsli"

MeshVertexOutput main(MeshVertexInput input)
{
	MeshVertexOutput o;

	matrix wvp = mul(world, mul(view, projection));

	o.position = mul(float4(input.position, 1.0), wvp);
	//o.position = float4(input.position, 1.0);
	o.worldpos = input.position;
	o.texcoord = input.texcoord;
	o.normal = input.normal;
	o.tangent = input.tangent;

	return o;	
}