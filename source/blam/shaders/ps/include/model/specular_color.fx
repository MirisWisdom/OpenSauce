#ifndef MODEL_SPECULAR_COLOR_FX
#define MODEL_SPECULAR_COLOR_FX

float4 GetSpecularColor(float3 Normal, float3 EyeVector,
	float3 SpecularMap, float4 PerpendicularColor, float4 ParallelColor)
{
	float4 Specular = (dot(EyeVector, Normal) * ParallelColor) + PerpendicularColor;
	Specular.rgb *= SpecularMap;
	
	return Specular;
}

#endif
