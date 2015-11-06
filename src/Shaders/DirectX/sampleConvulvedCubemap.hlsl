
#include "Lumina.hlsli"

TextureCube _Cubemap : register(t0);
SamplerState _Sampler : register(s0);

GBuffer main(MeshVertexOutput i)
{
	GBuffer o;
	o.albedo = _Cubemap.Sample(_Sampler, i.normal);
	o.albedo.a = 1.0;

	i.normal = i.normal * 0.5f + 0.5f;

	o.normal.rgb = i.normal;
	o.normal.a = 1.0;

	return o;
}