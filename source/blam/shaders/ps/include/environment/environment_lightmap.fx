static const float3 BumpBasis[3] =
{
	float3(	sqrt(2.0) / sqrt(3.0),				0.0,	1.0 / sqrt(3.0)),
	float3(	-1.0 / sqrt(6.0),		1.0 / sqrt(2.0),	1.0 / sqrt(3.0)),
	float3(	-1.0 / sqrt(6.0),		-1.0 / sqrt(2.0),	1.0 / sqrt(3.0))
};

float3 CalculateDLMContribution(float3 TangentNormal)
{
	TangentNormal.y = -TangentNormal.y;
	
	float3 lightmapContribution = 0;
	lightmapContribution.x = dot(BumpBasis[0], TangentNormal);
	lightmapContribution.y = dot(BumpBasis[1], TangentNormal);
	lightmapContribution.z = dot(BumpBasis[2], TangentNormal);
	lightmapContribution = saturate(lightmapContribution);
	
	return lightmapContribution;
}
	
float3 CalculateDLMDiff(float3 TangentNormal,
	float3 DLightmap0,
	float3 DLightmap1,
	float3 DLightmap2)
{
	float3 lightmapContribution = CalculateDLMContribution(TangentNormal);
	lightmapContribution *= lightmapContribution;

	float3 light = 0;
	
	float sum = dot(lightmapContribution, float3(1.0f, 1.0f, 1.0f));
	light += DLightmap0 * lightmapContribution.x;
	light += DLightmap1 * lightmapContribution.y;
	light += DLightmap2 * lightmapContribution.z;
	light /= sum;
	
	return light;
}
	
float3 CalculateDLMSpec(float3 TangentNormal,
	float3 EyeVectorTS,
	float3 DLightmap0,
	float3 DLightmap1,
	float3 DLightmap2,
	float3 SpecularTint,
	float SpecularExponent,
	float SpecularExponentMask,
	float SpecularCoefficient,
	float SpecularIntensity)
{		
	float3 reflectedEye = -reflect(normalize(EyeVectorTS), TangentNormal);	
	float3 lightmapContribution = CalculateDLMContribution(reflectedEye);
	
	float3 light = 0;
	
	float sum = dot(lightmapContribution, float3(1.0f, 1.0f, 1.0f));
	light += DLightmap0 * lightmapContribution.x;
	light += DLightmap1 * lightmapContribution.y;
	light += DLightmap2 * lightmapContribution.z;
	light *= saturate(sum);
	
	float attenuation = dot(light, float3(0.33f, 0.33f, 0.33f));
	attenuation = saturate(pow(attenuation, SpecularExponent * SpecularExponentMask)) * SpecularCoefficient;
	
	light *= attenuation;
	light *= SpecularTint.rgb;
	light *= SpecularIntensity;
	
	return light;
}

float4 GetSpecularColor(float3 Normal, float3 EyeVector,
	float3 SpecularMap, float4 PerpendicularColor, float4 ParallelColor)
{
	float4 Specular = (dot(EyeVector, Normal) * ParallelColor) + PerpendicularColor;
	Specular.rgb *= SpecularMap;
	
	return Specular;
}