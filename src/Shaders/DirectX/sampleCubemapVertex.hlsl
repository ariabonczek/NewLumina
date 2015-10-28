cbuffer worldviewproj : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};

struct VertexShaderInput
{
	float3 position		: POSITION;
};

struct VertexToPixel
{
	float4 positionH		: SV_POSITION;
	float4 positionL		: POSITION;
};

VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel o;

	matrix worldViewProj = mul(mul(world, view), projection);

	o.positionH = mul(float4(input.position, 1.0f), worldViewProj).xyww;
	o.positionL = float4(input.position,1.0f);

	return o;
}