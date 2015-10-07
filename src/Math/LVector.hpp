#ifndef LVECTOR_HPP
#define LVECTOR_HPP

#include <intrin.h>

#define NS_BEGIN_LMATH namespace LVector{
#define NS_END }

typedef __m128 VECTOR;

NS_BEGIN_LMATH

inline VECTOR Add(const VECTOR& v1, const VECTOR& v2)
{
	return _mm_add_ps(v1, v2);
}

inline VECTOR Add(const VECTOR& v, const float& f)
{
	return _mm_add_ps(v, _mm_load1_ps(&f));
}

inline VECTOR Divide(const VECTOR& v1, const VECTOR& v2)
{
	return _mm_div_ps(v1, v2);
}

inline VECTOR Divide(const VECTOR& v, const float& f)
{
	return _mm_div_ps(v, _mm_load1_ps(&f));
}

inline float Dot(const VECTOR& v1, const VECTOR& v2)
{
	return *(float*)&_mm_dp_ps(v1, v2, 0xff);
}

inline VECTOR Cross(const VECTOR& v1, VECTOR& v2)
{
	return _mm_sub_ps(
		_mm_mul_ps(_mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3, 1, 0, 2))),
		_mm_mul_ps(_mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3, 0, 2, 1)))
		);
}

inline VECTOR Lerp(const VECTOR& v1, const VECTOR& v2, const float& t)
{
	return _mm_add_ps(v1, _mm_mul_ps(_mm_sub_ps(v1, v2), _mm_set1_ps(t)));
}

inline float Magnitude(const VECTOR& v)
{
	return *(float*)&_mm_sqrt_ps(_mm_dp_ps(v, v, 0xff));
}

inline float MagnitudeSquared(const VECTOR& v)
{
	return *(float*)&_mm_dp_ps(v, v, 0xff);
}

inline VECTOR Multiply(const VECTOR& v1, VECTOR& v2)
{
	return _mm_mul_ps(v1, v2);
}

inline VECTOR Multiply(const VECTOR& v, const float& f)
{
	return _mm_mul_ps(v, _mm_load1_ps(&f));
}

inline VECTOR Normalize(const VECTOR& v)
{
	return _mm_mul_ps(v, _mm_rsqrt_ps(v));
}

inline VECTOR Reflect(const VECTOR& v, const VECTOR& normal)
{
	VECTOR t = _mm_mul_ps(v, _mm_rsqrt_ps(v));
	return _mm_sub_ps(v, _mm_mul_ps(_mm_set1_ps(2.0f), _mm_mul_ps(_mm_dp_ps(v, t, 0xff), t)));
}

inline VECTOR ReflectNormalized(const VECTOR& v, const VECTOR& normal)
{
	return _mm_sub_ps(v, _mm_mul_ps(_mm_set1_ps(2.0f), _mm_mul_ps(_mm_dp_ps(v, normal, 0xff), normal)));
}

inline VECTOR Subtract(const VECTOR& v1, const VECTOR& v2)
{
	return _mm_sub_ps(v1, v2);
}

inline VECTOR Subtract(const VECTOR& v, const float& f)
{
	return _mm_sub_ps(v, _mm_load1_ps(&f));
}

inline VECTOR Load(float* f)
{
	return _mm_load_ps(f);
}

inline void Store(VECTOR& v, float* f)
{
	return _mm_store_ps(f, v);
}

NS_END

#endif