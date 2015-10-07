#include <Math\MathCommon.hpp>

float DegToRad(float d)
{
	return PI * d / 180.0f;
}

float RadToDeg(float r)
{
	return r * 180.0f / PI;
}