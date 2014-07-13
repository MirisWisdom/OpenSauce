#ifndef MODEL_MODEL_EXTENSION_FX
#define MODEL_MODEL_EXTENSION_FX

struct LightVector
{
	float3 m_vector;
	float m_length;
};

struct LightVectorSet
{
	LightVector m_point_lights[2];
	LightVector m_distant_lights[2];
};

float GetPointLightAtten(float3 Normals, float3 LightVector, float VectorLength, float3 LightDirection, float InvRadSq, float SpotValA, float SpotValB)
{	
	float DistanceAtten = VectorLength * -InvRadSq + 1.0f;
	float AngleAtten = saturate(dot(Normals, LightVector));	
	float SpotAtten = saturate((dot(LightVector, -LightDirection) * SpotValA) + SpotValB);
	
	return saturate(DistanceAtten * AngleAtten * SpotAtten);
}

float GetDistantLightAtten(float3 Normal, float3 LightVector)
{
	//Calculate the attenuation of distant light
	return saturate(dot(Normal, -LightVector));
}

float GetSpecularLight(float3 Normals, float3 EyeVector, float3 LightVector)
{
	float3 ReflectionVector = GetReflectionVector(LightVector, Normals);
	return dot(ReflectionVector, EyeVector);
}

LightVectorSet GetLightVectors(float3 WorldPosition)
{
	LightVectorSet Vectors = (LightVectorSet)0;
	
	float3 temp_vector;
	
	temp_vector = c_object_point_light0_position.xyz - WorldPosition;	
	Vectors.m_point_lights[0].m_vector = normalize(temp_vector);
	Vectors.m_point_lights[0].m_length = dot(temp_vector, temp_vector);
	
	temp_vector = c_object_point_light1_position.xyz - WorldPosition;	
	Vectors.m_point_lights[1].m_vector = normalize(temp_vector);
	Vectors.m_point_lights[1].m_length = dot(temp_vector, temp_vector);
	
	Vectors.m_distant_lights[0].m_vector = c_object_distant_light0_vector.xyz;
	Vectors.m_distant_lights[0].m_length = 1;
	
	Vectors.m_distant_lights[1].m_vector = c_object_distant_light1_vector.xyz;
	Vectors.m_distant_lights[1].m_length = 1;
	
	return Vectors;
};

float4 GetLightAttenuations(LightVectorSet LightVectors, float3 Normals, float Translucency)
{
	float4 Attenuations = 0;
	
	// calculate the lighting contributions
	// point light 0
	Attenuations.x = GetPointLightAtten(Normals,
		LightVectors.m_point_lights[0].m_vector,
		LightVectors.m_point_lights[0].m_length,
		c_object_point_light0_rotation.xyz,
		c_object_point_light0_position.w,
		c_object_point_light0_rotation.w,
		c_object_point_light0_colour.w);
	
	// point light 1
	Attenuations.y = GetPointLightAtten(Normals,
		LightVectors.m_point_lights[1].m_vector,
		LightVectors.m_point_lights[1].m_length,
		c_object_point_light1_rotation.xyz,
		c_object_point_light1_position.w,
		c_object_point_light1_rotation.w,
		c_object_point_light1_colour.w);
	
	// distant light 0
	Attenuations.z = GetDistantLightAtten(Normals, LightVectors.m_distant_lights[0].m_vector);	
	
	// distant light 1
	Attenuations.w = GetDistantLightAtten(Normals, LightVectors.m_distant_lights[1].m_vector);
	
	// clamp the light
	Attenuations.xy = max(Attenuations.xy, 0);
	Attenuations.z = max(Attenuations.z, -Attenuations.z * Translucency);
	Attenuations.w = max(Attenuations.w, 0);	
	Attenuations = min(Attenuations, 1);
	return Attenuations;
}

float3 GetDiffuseLighting(float4 Attenuations)
{	
	// calculate the diffuse lighting value
	float3 DiffuseLighting = 0;
	DiffuseLighting += c_object_ambient_colour.xyz;
	DiffuseLighting += Attenuations.x * c_object_point_light0_colour.xyz;
	DiffuseLighting += Attenuations.y * c_object_point_light1_colour.xyz;
	DiffuseLighting += Attenuations.z * c_object_distant_light0_colour.xyz;
	DiffuseLighting += Attenuations.w * c_object_distant_light1_colour.xyz;
	return DiffuseLighting;
}

float3 GetSpecularLighting(float3 Normals, float3 EyeVector, LightVectorSet LightVectors, float4 LightAttenuations,
	float3 SpecularColor, float SpecularExponentMask, float SpecularCoefficientMask)
{
	float4 PhongAttenuation = 0;
	// point light 0
	PhongAttenuation.x = GetSpecularLight(Normals, EyeVector, LightVectors.m_point_lights[0].m_vector);
	// point light 1
	PhongAttenuation.y = GetSpecularLight(Normals, EyeVector, LightVectors.m_point_lights[1].m_vector);
	// distant light 0
	PhongAttenuation.z = GetSpecularLight(Normals, EyeVector, -LightVectors.m_distant_lights[0].m_vector);
	// distant light 1
	PhongAttenuation.w = GetSpecularLight(Normals, EyeVector, -LightVectors.m_distant_lights[1].m_vector);

	PhongAttenuation = max(0, PhongAttenuation);
	PhongAttenuation = pow(PhongAttenuation, c_specular_lighting_exponent * SpecularExponentMask) * c_specular_lighting_coefficient;
	PhongAttenuation *= LightAttenuations;
	
	float3 SpecularLighting = 0;
	SpecularLighting += (PhongAttenuation.x * c_object_point_light0_colour.xyz);
	SpecularLighting += (PhongAttenuation.y * c_object_point_light1_colour.xyz);
	SpecularLighting += (PhongAttenuation.z * c_object_distant_light0_colour.xyz);
	SpecularLighting += (PhongAttenuation.w * c_object_distant_light1_colour.xyz);
	SpecularLighting = saturate(SpecularColor * SpecularLighting) * SpecularCoefficientMask;	
	return SpecularLighting;
}
#endif