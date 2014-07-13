static const float3 BumpBasis[3] =
{
	float3(	sqrt(2.0) / sqrt(3.0),				0.0,	1.0 / sqrt(3.0)),
	float3(	-1.0 / sqrt(6.0),		1.0 / sqrt(2.0),	1.0 / sqrt(3.0)),
	float3(	-1.0 / sqrt(6.0),		-1.0 / sqrt(2.0),	1.0 / sqrt(3.0))
};
	
float3 CalculateDLM(float3 TangentNormal,
	float3 DLightmap0,
	float3 DLightmap1,
	float3 DLightmap2)
{
	TangentNormal.y = -TangentNormal.y;
	
	float3 LightmapContribution = 0;
	LightmapContribution.x = dot(BumpBasis[0], TangentNormal);
	LightmapContribution.y = dot(BumpBasis[1], TangentNormal);
	LightmapContribution.z = dot(BumpBasis[2], TangentNormal);
	LightmapContribution = saturate(LightmapContribution);
	LightmapContribution *= LightmapContribution;

	float3 diffuseLight = 0;
	
	float diffuseSum = dot(LightmapContribution, float3(1.0f, 1.0f, 1.0f));
	diffuseLight += DLightmap0 * LightmapContribution.x;
	diffuseLight += DLightmap1 * LightmapContribution.y;
	diffuseLight += DLightmap2 * LightmapContribution.z;
	diffuseLight /= diffuseSum;
	
	return diffuseLight;
}