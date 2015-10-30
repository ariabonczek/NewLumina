
#include "Lumina.hlsli"

struct VertexToPixel
{
	float4 positionH		: SV_POSITION;
	float4 positionL		: POSITION;
};

VertexToPixel main(MeshVertexInput input)
{
	VertexToPixel o;

	matrix worldViewProj = mul(mul(world, view), projection);

	o.positionH = mul(float4(input.position, 1.0f), worldViewProj).xyww;
	o.positionL = float4(input.position,1.0f);

	return o;
}