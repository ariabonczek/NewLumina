#ifndef MATH_COMMON_HPP
#define MATH_COMMON_HPP

#define NS_MATH namespace LMath {
#define NS_MATH_END }

#include <algorithm>

namespace LMath
{
	struct Vector2;
	struct Vector3;
	struct Vector4;
	struct Matrix;
	struct Quaternion;
}
#define lerp(f1, f2, t) f1 + (f2 - f1) * t

typedef unsigned int uint;

#define PI 3.14159265f

float DegToRad(float d);
float RadToDeg(float r);


#endif