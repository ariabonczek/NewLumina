struct vin
{
	float3 position : POSITION;
};

struct toPixel
{
	float3 position : SV_POSITION;
};

toPixel vert(vin input)
{
	toPixel o;

	o.position = input.position;

	return o;	
}

float4 pix(toPixel input)
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}