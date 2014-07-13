#ifndef UTIL_BUMP_MAP_FX
#define UTIL_BUMP_MAP_FX

float3 GetBumpReflect(sampler BumpMap, float2 Tex, float4 M1, float4 M2, float4 M3)
{
	half4 bump = tex2D(BumpMap, Tex);
	bump.rgb = 2 * (bump.rgb - 0.5f);
	
	float3 uvw;
	uvw.x = dot(M1.xyz, bump.rgb);
	uvw.y = dot(M2.xyz, bump.rgb);
	uvw.z = dot(M3.xyz, bump.rgb);
	
	float3 eye = float3(M1.w, M2.w, M3.w);
	uvw = GetReflectionVector(eye, uvw);
	
	return uvw;
}
#endif