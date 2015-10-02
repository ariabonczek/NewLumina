struct vin
{
	float3 position : POSITION;
};

struct toPixel
{
	float4 position : SV_POSITION;
};

toPixel main(vin input)
{
	toPixel o;

	o.position = float4(input.position, 1.0);

	return o;	
}