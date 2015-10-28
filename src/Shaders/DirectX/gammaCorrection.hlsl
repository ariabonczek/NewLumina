
#include "Lumina.hlsli"

cbuffer Properties : register(b1)
{
	float4 tint;
}

Texture2D _Source : register(t0);
sampler _Sampler : register(s0);

float4 main(FullScreenQuadOutput input) : SV_Target0
{
	float4 blah = pow(_Source.Sample(_Sampler, input.texcoord), 1.0 / 2.2);
	return blah;
}