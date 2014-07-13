#ifndef UTIL_INTERPOLATION_FX
#define UTIL_INTERPOLATION_FX

float4 Lerp(float4 Interp, float4 Value1, float4 Value2)
{
	return Interp * (Value1 - Value2) + Value2;
}
float3 Lerp(float3 Interp, float3 Value1, float3 Value2)
{
	return Interp * (Value1 - Value2) + Value2;
}
float2 Lerp(float2 Interp, float2 Value1, float2 Value2)
{
	return Interp * (Value1 - Value2) + Value2;
}
float Lerp(float Interp, float Value1, float Value2)
{
	return Interp * (Value1 - Value2) + Value2;
}
#endif