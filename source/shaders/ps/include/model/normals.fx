#ifndef MODEL_NORMALS_FX
#define MODEL_NORMALS_FX

float3 UnbiasNormal(float3 Normal)
{
	return (2.0f * Normal) - 1.0f;
}
float2 UnbiasNormal(float2 Normal)
{
	return (2.0f * Normal) - 1.0f;
}

float3 GetNormals(uniform const bool bBaseNormal, uniform const bool bDetailNormal,
	float3x3 TangentToWorldSpace, float4 BaseNormal, float2 DetailNormal1, float2 DetailNormal2)
{
	float4 BaseBump = 		float4(0, 0, 1, 1);
	float2 DetailBump1 = 	float2(0, 0);
	float2 DetailBump2 = 	float2(0, 0);
		
	if(bBaseNormal)
	{
		BaseBump.xyz =	UnbiasNormal(BaseNormal.xyz);
		BaseBump.xy *= c_base_normal_map_coefficient;
		BaseBump.z *= c_base_normal_map_z_coefficient;
		BaseBump.w = BaseNormal.w;
	}
	
	float2 DetailNormal = 0;
	if(bDetailNormal)
	{
		DetailBump1 = 	UnbiasNormal(DetailNormal1) * c_detail_normal_map_1_interpolation * c_detail_normal_map_1_coefficient;
		
		if(bBaseNormal)
		{
			DetailBump2 =	UnbiasNormal(DetailNormal2) * c_detail_normal_map_2_interpolation * c_detail_normal_map_2_coefficient;
			
			float InterpValue = BaseBump.w * 3.0f;
			
			DetailNormal = lerp(0, DetailBump1, clamp(InterpValue, 0, 1));
			DetailNormal = lerp(DetailNormal, DetailBump2, clamp(InterpValue - 1, 0, 1));
			DetailNormal = lerp(DetailNormal, 0, clamp(InterpValue - 2, 0, 1));
		}
		else
			DetailNormal = DetailBump1;
	}
	
	BaseBump.xy += DetailNormal;
	
	return mul(TangentToWorldSpace, BaseBump.xyz);
}

#endif