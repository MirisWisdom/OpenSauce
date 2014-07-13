#ifndef LIGHTING_REFLECTION_FX
#define LIGHTING_REFLECTION_FX

//#define DEBUG_OUTPUT
#ifdef DEBUG_OUTPUT
	//#define DEBUG_LIGHTING
	//#define DEBUG_LIGHTING_W
	#define DEBUG_SPECULAR
	//#define DEBUG_SPECULAR_W
	//#define DEBUG_REFLECTION
	//#define DEBUG_REFLECTION_W
	//#define DEBUG_BASEMAPUV
	//#define DEBUG_DETAILMAPUV
	//#define DEBUG_MULTIPURPOSEMAPUV
#endif

#define PS_CONSTANT_OFFSET 10

//struct LIGHTING_IN{
//	float3 PositionWorld		: COLOR0;
// 	float4 DiffuseMultiUV		: TEXCOORD0;
// 	float2 DetailUV 			: TEXCOORD1;
// 	float3 Normal	 			: TEXCOORD2;
// 	float3 BiNormal 			: TEXCOORD3;
// 	float3 Tangent	 			: TEXCOORD4;
//};

sampler t_base_normal_map 		: register(s4);
sampler t_detail_normal_map_1		: register(s5);
sampler t_detail_normal_map_2		: register(s6);
sampler t_specular_color_map		: register(s7);


float4 	c_eye_view_position 		: register(c[4 + 0 + PS_CONSTANT_OFFSET]);

float4 	c_unknown_12 			: register(c[7 + 2 + PS_CONSTANT_OFFSET]);

float4 	c_object_parallel_colour 	: register(c[10 + 0 + PS_CONSTANT_OFFSET]);
float4 	c_object_perpendicular_colour 	: register(c[10 + 1 + PS_CONSTANT_OFFSET]);

float4 	c_object_point_light0_position 	: register(c[12 + 0 + PS_CONSTANT_OFFSET]);
float4 	c_object_point_light0_rotation 	: register(c[12 + 1 + PS_CONSTANT_OFFSET]);
float4 	c_object_point_light0_colour 	: register(c[12 + 2 + PS_CONSTANT_OFFSET]);

float4 	c_object_point_light1_position 	: register(c[12 + 3 + PS_CONSTANT_OFFSET]);
float4 	c_object_point_light1_rotation 	: register(c[12 + 4 + PS_CONSTANT_OFFSET]);
float4 	c_object_point_light1_colour 	: register(c[12 + 5 + PS_CONSTANT_OFFSET]);

float4 	c_object_distant_light0_vector 	: register(c[12 + 6 + PS_CONSTANT_OFFSET]);
float4 	c_object_distant_light0_colour 	: register(c[12 + 7 + PS_CONSTANT_OFFSET]);

float4 	c_object_distant_light1_vector 	: register(c[12 + 8 + PS_CONSTANT_OFFSET]);
float4 	c_object_distant_light1_colour 	: register(c[12 + 9 + PS_CONSTANT_OFFSET]);

float3 	c_object_ambient_colour 		: register(c[12 + 10 + PS_CONSTANT_OFFSET]);

float4 	c_shader_interpolation 			: register(c[24 + 0 + PS_CONSTANT_OFFSET]);
float4 	c_shader_multipliers 			: register(c[24 + 1 + PS_CONSTANT_OFFSET]);
float4 	c_shader_miscellaneous 			: register(c[24 + 2 + PS_CONSTANT_OFFSET]);

#ifdef DEBUG_OUTPUT
float4
#else
void
#endif
		LightingReflectionSpecular(in PS_INPUT IN, out half4 Lighting, out half4 Reflection, out half4 Specular)
{
#ifdef ORIGINAL_VS
	Reflection = 0;
	Specular = IN.Spec;
	Lighting = IN.Diff;

	#ifdef DEBUG_LIGHTING
	return float4(IN.Diff.xyz, 1);
	#endif

	#ifdef DEBUG_LIGHTING_W
	return float4(IN.Diff.www, 1);
	#endif

	#ifdef DEBUG_SPECULAR
	return float4(IN.Spec.xyz, 1);
	#endif

	#ifdef DEBUG_SPECULAR_W
	return float4(IN.Spec.www, 1);
	#endif

	#ifdef DEBUG_REFLECTION
	return float4(IN.Tex3.xyz, 1);
	#endif

	#ifdef DEBUG_REFLECTION_W
	return float4(IN.Tex3.www, 1);
	#endif

	#ifdef DEBUG_BASEMAPUV
	return float4(IN.Tex0, 0, 1);
	#endif

	#ifdef DEBUG_DETAILMAPUV
	return float4(IN.Tex1, 0, 1);
	#endif

	#ifdef DEBUG_MULTIPURPOSEMAPUV
	return float4(IN.Tex2, 0, 1);
	#endif
#else
	Reflection = 0;
	Specular = 1;
	Lighting = 0;

	float3x3 WorldToTangentSpace;
	WorldToTangentSpace[0] = normalize(IN.Tangent);
	WorldToTangentSpace[1] = normalize(IN.BiNormal);
	WorldToTangentSpace[2] = normalize(IN.Normal);

	float4 BaseBump = 		tex2D(t_base_normal_map, IN.DiffuseMultiUV.xy);
	float3 DetailBump1 = 		tex2D(t_detail_normal_map_1, IN.DiffuseMultiUV.xy * c_shader_miscellaneous.y).xyz;
	float3 DetailBump2 = 		tex2D(t_detail_normal_map_2, IN.DetailUV.xy * c_shader_miscellaneous.z).xyz;
	float4 SpecularColor = 		tex2D(t_specular_color_map, IN.DiffuseMultiUV.xy);

	SpecularColor.xyz =		pow(abs(SpecularColor.xyz), c_shader_miscellaneous.x) * c_shader_multipliers.x;
	BaseBump.xyz = 			(2.0 * BaseBump.xyz) - 1.0;
	DetailBump1 = 			(2.0 * DetailBump1) - 1.0;
	DetailBump2 = 			(2.0 * DetailBump2) - 1.0;

	SpecularColor = 		lerp(float4(1,1,1,1), 	SpecularColor, 	c_shader_interpolation.x);
	BaseBump = 			lerp(float4(0,0,1,1), 	BaseBump, 	c_shader_interpolation.y);
	DetailBump1 = 			lerp(float3(0,0,1), 	DetailBump1, 	c_shader_interpolation.z);
	DetailBump2 = 			lerp(float3(0,0,1), 	DetailBump2, 	c_shader_interpolation.w);

	SpecularColor.xyz =		SpecularColor.xyz * c_shader_multipliers.x;
	BaseBump.xyz = 			BaseBump.xyz * float3( c_shader_multipliers.y, c_shader_multipliers.y, clamp(1.0f / c_shader_multipliers.x, 0.0f, 1.0f));
	DetailBump1 = 			DetailBump1 * float3(c_shader_multipliers.z, c_shader_multipliers.z, 0) * BaseBump.w;
	DetailBump2 = 			DetailBump2 * float3(c_shader_multipliers.w, c_shader_multipliers.w, 0) * (1.0f - BaseBump.w);


	float3 NormalBumpWS = 	mul(transpose(WorldToTangentSpace), BaseBump.xyz + DetailBump1 + DetailBump2);

	float4 EyeVectorWS = float4(c_eye_view_position.xyz - IN.PositionWorld, c_eye_view_position.w);
	float4 EyeVectorWSNorm = normalize(EyeVectorWS);

	float4 PointLight0VectorWS = float4(c_object_point_light0_position.xyz - IN.PositionWorld, c_object_point_light0_position.w);
	float4 PointLight0RotationWS = c_object_point_light0_rotation;

	float4 PointLight1VectorWS = float4(c_object_point_light1_position.xyz - IN.PositionWorld, c_object_point_light1_position.w);
	float4 PointLight1RotationWS = c_object_point_light1_rotation;

	float3 DistantLight0VectorWS = c_object_distant_light0_vector.xyz;
	float3 DistantLight1VectorWS = c_object_distant_light1_vector.xyz;

	Reflection.xyz = ((dot(EyeVectorWS.xyz, NormalBumpWS.xyz) * NormalBumpWS.xyz) * EyeVectorWS.w) + -EyeVectorWS.xyz;

        float4 Spec = float4(saturate(SpecularColor.xyz) * c_object_perpendicular_colour.xyz, c_object_perpendicular_colour.w);
	Specular = (dot(EyeVectorWSNorm, NormalBumpWS) * c_object_parallel_colour) + Spec;

#ifdef DEBUG_OUTPUT
	return float4(Specular.xyz, 1.0f);
#endif

	float VectorLength;
	float3 VectorNorm;
	float2 PointAttenuation;
	float2 DistantAttenuation;

	//Calculate the attenuation of Point Light 0
	VectorNorm = normalize(PointLight0VectorWS.xyz);
	PointAttenuation.x = (dot(PointLight0VectorWS, PointLight0VectorWS) * -PointLight0VectorWS.w) + IN.DetailUV.w;
	PointAttenuation.x *= saturate(dot(NormalBumpWS, VectorNorm.xyz));
	PointAttenuation.x *= saturate((dot(VectorNorm.xyz, -PointLight0RotationWS.xyz) * PointLight0RotationWS.w) + c_object_point_light0_colour.w);

	//Calculate the attenuation of Point Light 1
	VectorNorm = normalize(PointLight1VectorWS.xyz);
	PointAttenuation.y = (dot(PointLight1VectorWS, PointLight1VectorWS) * -PointLight1VectorWS.w) + IN.DetailUV.w;
	PointAttenuation.y *= saturate(dot(NormalBumpWS, VectorNorm.xyz));
	PointAttenuation.y *= saturate((dot(VectorNorm.xyz, -PointLight1RotationWS.xyz) * PointLight1RotationWS.w) + c_object_point_light1_colour.w);  

	//Calculate the attenuation of Distant Light 0
	DistantAttenuation.x = dot(-DistantLight0VectorWS, NormalBumpWS);
	DistantAttenuation.x = max(DistantAttenuation.x, DistantAttenuation.x * c_unknown_12.z);

	//Calculate the attenuation of Distant Light 1
	DistantAttenuation.y = dot(-DistantLight1VectorWS, NormalBumpWS);

	//Clamp all attenuations
	PointAttenuation = max(PointAttenuation, IN.DetailUV.z);
	DistantAttenuation = max(DistantAttenuation, IN.DetailUV.z);
	PointAttenuation = min(PointAttenuation, IN.DetailUV.w);
	DistantAttenuation = min(DistantAttenuation, IN.DetailUV.w);

	//Build the lighting
	Lighting.xyz += c_object_ambient_colour.xyz;
	Lighting.xyz += (half3)(PointAttenuation.x * c_object_point_light0_colour.xyz);
	Lighting.xyz += (half3)(PointAttenuation.y * c_object_point_light1_colour.xyz);
	Lighting.xyz += (half3)(DistantAttenuation.x * c_object_distant_light0_colour.xyz);
	Lighting.xyz += (half3)(DistantAttenuation.y * c_object_distant_light1_colour.xyz);

	//Fog
	Lighting.w = IN.DetailUV.z;


	#ifdef DEBUG_LIGHTING
	return float4(Lighting.xyz, 1);
	#endif

	#ifdef DEBUG_LIGHTING_W
	return float4(Lighting.www, 1);
	#endif

	#ifdef DEBUG_SPECULAR
	return float4(Specular.xyz, 1);
	#endif

	#ifdef DEBUG_SPECULAR_W
	return float4(Specular.www, 1);
	#endif

	#ifdef DEBUG_REFLECTION
	return float4(Reflection.xyz, 1);
	#endif

	#ifdef DEBUG_REFLECTION_W
	return float4(Reflection.www, 1);
	#endif

	#ifdef DEBUG_BASEMAPUV
	return float4(IN.DiffuseMultiUV.xy, 0, 1);
	#endif

	#ifdef DEBUG_DETAILMAPUV
	return float4(IN.DetailUV.xy, 0, 1);
	#endif

	#ifdef DEBUG_MULTIPURPOSEMAPUV
	return float4(IN.DiffuseMultiUV.zw, 0, 1);
	#endif
#endif
}

#endif
