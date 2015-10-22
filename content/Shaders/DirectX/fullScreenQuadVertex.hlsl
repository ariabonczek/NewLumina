
#include "Lumina.hlsli"

FullScreenQuadOutput main(FullScreenQuadInput input)
{
	FullScreenQuadOutput o;

	o.position = float4(input.position, 1.0);
	o.texcoord = input.texcoord;

	return o;
}