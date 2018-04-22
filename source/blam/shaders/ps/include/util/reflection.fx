#ifndef UTIL_REFLECTION_FX
#define UTIL_REFLECTION_FX

float3 GetReflectionVector(float3 Vector, float3 Normal)
{
	return ((dot(Vector, Normal) * Normal) * 2) + -Vector;
}
#endif