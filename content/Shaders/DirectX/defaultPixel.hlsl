struct input
{
	float4 position : SV_POSITION;
};

float4 main() : SV_Target
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}