
#include "Lumina.hlsli"

Texture2D _Source : register(t0);
sampler _Sampler : register(s0);

float4 main(FullScreenQuadOutput input) : SV_Target0
{
	return _Source.Sample(_Sampler, input.texcoord);
}