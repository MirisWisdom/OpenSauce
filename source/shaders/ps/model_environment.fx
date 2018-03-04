#include "include/util/reflection.fx"

#include "include/model/model_shared_variables.fx"
#include "include/model/normals.fx"
#include "include/model/lighting.fx"
#include "include/model/specular_color.fx"

struct PS_INPUT{
 	float  		Fog				: FOG;
 	float3 		Position3D		: TEXCOORD0;
 	float4 		DiffuseMultiUV	: TEXCOORD1;
 	float4 		DetailUV 		: TEXCOORD2;
	float3x3 	TBNTranspose 	: TEXCOORD3;
	float4 		NormalDetailUV	: TEXCOORD6; 
};

///////////////////////////////////////////////////////////////////////////////
// Pixel Shader 2.0 shaders
///////////////////////////////////////////////////////////////////////////////
half4 ModelEnvironmentNoMask(PS_INPUT IN,
    uniform const int nDetailFunction,
    uniform const bool bDetailBeforeReflection,
    uniform const bool bPlanarAtmosphericFog,
    uniform const bool bInverse,
	uniform const bool bBaseNorm,
	uniform const bool bDetailNorm,
	uniform const bool bSpecMap,
	uniform const bool bSpecLighting) : COLOR
{
	half4 Diff = 0;
	half3 Reflect = 0;
	half4 Spec = 1;
	
	half4 T0 = tex2D( TexSampler0, IN.DiffuseMultiUV.xy );
	half4 T1 = tex2D( TexSampler1, IN.DetailUV.xy );
	half4 T2 = tex2D( TexSampler2, IN.DiffuseMultiUV.xy );
	half4 T4 = tex2D( TexSampler4, IN.DiffuseMultiUV.xy );
	half4 T5 = tex2D( TexSampler5, IN.NormalDetailUV.xy );
	half4 T6 = tex2D( TexSampler6, IN.NormalDetailUV.zw );
	
	// get the world space normals
	float3 Normals = GetNormals(bBaseNorm, bDetailNorm,	IN.TBNTranspose, T4, T5.xy, T6.xy);

	float3 EyeVector = c_eye_view_position.xyz - IN.Position3D;
	float3 EyeVectorNorm = normalize(EyeVector);

	float4 SpecularMap = 1;
	if(bSpecMap)
	{
		SpecularMap = tex2D(TexSampler7, IN.DiffuseMultiUV.xy);
		SpecularMap.xyz = saturate(pow(SpecularMap.xyz, c_specular_color_map_exponent) * c_specular_color_map_coefficient);
		SpecularMap.w = lerp(1, SpecularMap.w, c_spec_color_exponent_mask_toggle);
	}
	
	Spec = GetSpecularColor(Normals, EyeVectorNorm,	SpecularMap, c_object_perpendicular_colour, c_object_parallel_colour);
	
	// calculate the lighting contributions
	LightVectorSet LightVectors = GetLightVectors(IN.Position3D);
	float4 LightAttenuations = GetLightAttenuations(LightVectors, Normals, IN.DetailUV.w);

	float3 DiffuseLighting = GetDiffuseLighting(LightAttenuations);
	float3 SpecularLighting = 0;
	if(bSpecLighting)
	{
		SpecularLighting = GetSpecularLighting(Normals, EyeVectorNorm,
			LightVectors, LightAttenuations,
			Spec.rgb, SpecularMap.w, T2.b);
	}		
	Diff.rgb = DiffuseLighting;
	Diff.a = IN.DetailUV.z;
	
	// get the reflection vector
	Reflect = GetReflectionVector(EyeVector, Normals);	
	
	half4 T3 = texCUBE( TexSampler3, Reflect );
	
	half4 R0;
	half4 R1;
	half4 D0 = Diff;
	half4 D1 = Spec;
	half3 SRCCOLOR;
	half SRCALPHA;
		
	// combiner 0
	
	// combiner 1

	// combiner 2
	R1.a	= T2.b*D1.a;
	
	// combiner 4
	T3.rgb	= T3*D1;
	
	if(bDetailBeforeReflection)
	{
		// combiner 6
		R0 = (T0*D0) + (T3*R1.a);
		
		// combiner 7
		if(detail_function_biased_multiply==nDetailFunction)
		{
			R0.rgb	= (R0*T1)*2;
		}
		else if(detail_function_multiply==nDetailFunction)
		{
			R0.rgb	= R0*T1;
		}
		else if(detail_function_biased_add==nDetailFunction)
		{
			R0.rgb	= R0+((2*T1)-1);
		}
	}
	else
	{
		// combiner 6
		if(detail_function_biased_multiply==nDetailFunction)
		{
			T0.rgb	= (T0*T1)*2;
		}
		else if(detail_function_multiply==nDetailFunction)
		{
			T0.rgb	= T0*T1;
		}
		else if(detail_function_biased_add==nDetailFunction)
		{
			T0.rgb	= T0+((2*T1)-1);
		}
		
		// combiner 7
		R0 = (T0*D0) + (T3*R1.a);
	}
	
	R0.rgb += SpecularLighting;

	if(bPlanarAtmosphericFog)
	{
		// combiner 5
		R1.rgb	= saturate(c_fog_color_correction_1 - (D0.a*c_fog_color_correction_E));

		// final combiner
		SRCCOLOR = lerp((R0*c_fog_color_correction_0.a), c_fog_color_correction_0, D0.a) + R1;
	}
	else
	{
		// final combiner
		SRCCOLOR = R0;
	}

	SRCALPHA = T2.a;

	return half4( SRCCOLOR, SRCALPHA );
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiply
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, false, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c12.w
    mad r1.w, r0.w, -r1.w, c0.x
    dp3_sat r0.x, t3, r1
    dp3 r0.y, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.y, r0.y, r0.w, c14.w
    mul r0.x, r0.x, r1.w
    mul r0.x, r0.y, r0.x
    mov r1.w, c15.w
    add r1.xyz, -t0, c15
    dp4 r1.w, r1, r1
    nrm r2.xyz, r1
    mov r2.w, c15.w
    mad r2.w, r1.w, -r2.w, c0.x
    dp3_sat r0.z, t3, r2
    dp3 r0.w, r2, -c16
    mov r1.w, c16.w
    mad_sat r0.w, r0.w, r1.w, c17.w
    mul r0.z, r0.z, r2.w
    mul r0.y, r0.w, r0.z
    max r1.xy, r0, t2.z
    min r0.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r0.xzw, r0.x, r1.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    dp3 r0.w, -c18, t3
    mul r1.x, r0.w, c9.z
    max r2.x, r0.w, r1.x
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xyz, r2.x, c19, r0
    mad_pp r0.xyz, r2.y, c21, r0
    add r1.xyz, -t0, c8
    dp3 r0.w, r1, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r1
    nrm r3.xyz, r1
    dp3 r0.w, t3, r3
    mov r1, c10
    mad_pp r1, r0.w, r1, c11
    mov r3.xy, t1.zwzw
    texld_pp r2, r2, s3
    texld_pp r3, r3, s2
    mul_pp r1.xyz, r1, r2
    mul_pp r0.w, r1.w, r3.z
    mul_pp r1.xyz, r0.w, r1
    texld_pp r2, t2, s1
    texld_pp r4, t1, s0
    mad_pp r0.xyz, r4, r0, r1
    mul_pp r0.xyz, r2, r0
    add_pp r3.xyz, r0, r0
    mov_pp oC0, r3

// approximately 63 instruction slots used (4 texture, 59 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiply
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, false, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c12.w
    mad r1.w, r0.w, -r1.w, c0.x
    dp3_sat r0.x, t3, r1
    dp3 r0.y, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.y, r0.y, r0.w, c14.w
    mul r0.x, r0.x, r1.w
    mul r0.x, r0.y, r0.x
    mov r1.w, c15.w
    add r1.xyz, -t0, c15
    dp4 r1.w, r1, r1
    nrm r2.xyz, r1
    mov r2.w, c15.w
    mad r2.w, r1.w, -r2.w, c0.x
    dp3_sat r0.z, t3, r2
    dp3 r0.w, r2, -c16
    mov r1.w, c16.w
    mad_sat r0.w, r0.w, r1.w, c17.w
    mul r0.z, r0.z, r2.w
    mul r0.y, r0.w, r0.z
    max r1.xy, r0, t2.z
    min r0.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r0.xzw, r0.x, r1.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    dp3 r0.w, -c18, t3
    mul r1.x, r0.w, c9.z
    max r2.x, r0.w, r1.x
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xyz, r2.x, c19, r0
    mad_pp r0.xyz, r2.y, c21, r0
    add r1.xyz, -t0, c8
    dp3 r0.w, r1, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r1
    nrm r3.xyz, r1
    dp3 r0.w, t3, r3
    mov r1, c10
    mad_pp r1, r0.w, r1, c11
    mov r3.xy, t1.zwzw
    texld_pp r2, r2, s3
    texld_pp r3, r3, s2
    mul_pp r1.xyz, r1, r2
    mul_pp r0.w, r1.w, r3.z
    mul_pp r1.xyz, r0.w, r1
    texld_pp r2, t2, s1
    texld_pp r4, t1, s0
    mad_pp r0.xyz, r4, r0, r1
    mul_pp r3.xyz, r2, r0
    mov_pp oC0, r3

// approximately 62 instruction slots used (4 texture, 58 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAdd
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, false, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 1, 2, -1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    dp3 r0.z, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.z, r0.z, r0.w, c14.w
    mul r0.x, r0.y, r0.x
    mul r0.x, r0.z, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r1.xyz, r2
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r0.z, t3, r1
    dp3 r0.w, r1, -c16
    mov r2.w, c16.w
    mad_sat r0.w, r0.w, r2.w, c17.w
    mul r0.z, r0.z, r1.w
    mul r0.y, r0.w, r0.z
    max r1.xy, r0, t2.z
    min r0.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r0.xzw, r0.x, r1.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    dp3 r0.w, -c18, t3
    mul r1.x, r0.w, c9.z
    max r2.x, r0.w, r1.x
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xyz, r2.x, c19, r0
    mad_pp r0.xyz, r2.y, c21, r0
    add r1.xyz, -t0, c8
    dp3 r0.w, r1, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r1
    nrm r3.xyz, r1
    dp3 r0.w, t3, r3
    mov r1, c10
    mad_pp r1, r0.w, r1, c11
    mov r3.xy, t1.zwzw
    texld_pp r2, r2, s3
    texld_pp r3, r3, s2
    mul_pp r1.xyz, r1, r2
    mul_pp r0.w, r1.w, r3.z
    mul_pp r1.xyz, r0.w, r1
    texld_pp r2, t2, s1
    texld_pp r4, t1, s0
    mad_pp r0.xyz, r4, r0, r1
    mad_pp r0.xyz, r2, c0.y, r0
    add_pp r3.xyz, r0, c0.z
    mov_pp oC0, r3

// approximately 62 instruction slots used (4 texture, 58 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiply
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, false, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c12.w
    mad r1.w, r0.w, -r1.w, c0.x
    dp3_sat r0.x, t3, r1
    dp3 r0.y, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.y, r0.y, r0.w, c14.w
    mul r0.x, r0.x, r1.w
    mul r0.x, r0.y, r0.x
    mov r1.w, c15.w
    add r1.xyz, -t0, c15
    dp4 r1.w, r1, r1
    nrm r2.xyz, r1
    mov r2.w, c15.w
    mad r2.w, r1.w, -r2.w, c0.x
    dp3_sat r0.z, t3, r2
    dp3 r0.w, r2, -c16
    mov r1.w, c16.w
    mad_sat r0.w, r0.w, r1.w, c17.w
    mul r0.z, r0.z, r2.w
    mul r0.y, r0.w, r0.z
    max r1.xy, r0, t2.z
    min r0.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r0.xzw, r0.x, r1.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    dp3 r0.w, -c18, t3
    mul r1.x, r0.w, c9.z
    max r2.x, r0.w, r1.x
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xyz, r2.x, c19, r0
    mad_pp r0.xyz, r2.y, c21, r0
    texld_pp r1, t2, s1
    texld_pp r2, t1, s0
    mul_pp r1.xyz, r1, r2
    mul_pp r0.xyz, r0, r1
    add_pp r0.xyz, r0, r0
    add r1.xyz, -t0, c8
    dp3 r0.w, r1, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r1
    nrm r3.xyz, r1
    dp3 r0.w, t3, r3
    mov r1, c10
    mad_pp r1, r0.w, r1, c11
    mov r3.xy, t1.zwzw
    texld_pp r2, r2, s3
    texld_pp r3, r3, s2
    mul_pp r1.xyz, r1, r2
    mul_pp r0.w, r1.w, r3.z
    mad_pp r3.xyz, r1, r0.w, r0
    mov_pp oC0, r3

// approximately 63 instruction slots used (4 texture, 59 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiply
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, false, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c12.w
    mad r1.w, r0.w, -r1.w, c0.x
    dp3_sat r0.x, t3, r1
    dp3 r0.y, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.y, r0.y, r0.w, c14.w
    mul r0.x, r0.x, r1.w
    mul r0.x, r0.y, r0.x
    mov r1.w, c15.w
    add r1.xyz, -t0, c15
    dp4 r1.w, r1, r1
    nrm r2.xyz, r1
    mov r2.w, c15.w
    mad r2.w, r1.w, -r2.w, c0.x
    dp3_sat r0.z, t3, r2
    dp3 r0.w, r2, -c16
    mov r1.w, c16.w
    mad_sat r0.w, r0.w, r1.w, c17.w
    mul r0.z, r0.z, r2.w
    mul r0.y, r0.w, r0.z
    max r1.xy, r0, t2.z
    min r0.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r0.xzw, r0.x, r1.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    dp3 r0.w, -c18, t3
    mul r1.x, r0.w, c9.z
    max r2.x, r0.w, r1.x
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xyz, r2.x, c19, r0
    mad_pp r0.xyz, r2.y, c21, r0
    add r1.xyz, -t0, c8
    dp3 r0.w, r1, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r1
    nrm r3.xyz, r1
    dp3 r0.w, t3, r3
    mov r1, c10
    mad_pp r1, r0.w, r1, c11
    mov r3.xy, t1.zwzw
    texld_pp r2, r2, s3
    texld_pp r3, r3, s2
    mul_pp r1.xyz, r1, r2
    mul_pp r0.w, r1.w, r3.z
    mul_pp r1.xyz, r0.w, r1
    texld_pp r2, t2, s1
    texld_pp r4, t1, s0
    mul_pp r2.xyz, r2, r4
    mad_pp r3.xyz, r2, r0, r1
    mov_pp oC0, r3

// approximately 62 instruction slots used (4 texture, 58 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAdd
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, false, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 1, 2, -1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    dp3 r0.z, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.z, r0.z, r0.w, c14.w
    mul r0.x, r0.y, r0.x
    mul r0.x, r0.z, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r1.xyz, r2
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r0.z, t3, r1
    dp3 r0.w, r1, -c16
    mov r2.w, c16.w
    mad_sat r0.w, r0.w, r2.w, c17.w
    mul r0.z, r0.z, r1.w
    mul r0.y, r0.w, r0.z
    max r1.xy, r0, t2.z
    min r0.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r0.xzw, r0.x, r1.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    dp3 r0.w, -c18, t3
    mul r1.x, r0.w, c9.z
    max r2.x, r0.w, r1.x
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xyz, r2.x, c19, r0
    mad_pp r0.xyz, r2.y, c21, r0
    add r1.xyz, -t0, c8
    dp3 r0.w, r1, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r1
    nrm r3.xyz, r1
    dp3 r0.w, t3, r3
    mov r1, c10
    mad_pp r1, r0.w, r1, c11
    mov r3.xy, t1.zwzw
    texld_pp r2, r2, s3
    texld_pp r3, r3, s2
    mul_pp r1.xyz, r1, r2
    mul_pp r0.w, r1.w, r3.z
    mul_pp r1.xyz, r0.w, r1
    texld_pp r2, t2, s1
    texld_pp r4, t1, s0
    mad_pp r2.xyz, r2, c0.y, r4
    add_pp r2.xyz, r2, c0.z
    mad_pp r3.xyz, r2, r0, r1
    mov_pp oC0, r3

// approximately 62 instruction slots used (4 texture, 58 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyComplexFog
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, true, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 1, 2, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c12.w
    mad r1.w, r0.w, -r1.w, c3.x
    dp3_sat r0.x, t3, r1
    dp3 r0.y, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.y, r0.y, r0.w, c14.w
    mul r0.x, r0.x, r1.w
    mul r0.x, r0.y, r0.x
    mov r1.w, c15.w
    add r1.xyz, -t0, c15
    dp4 r1.w, r1, r1
    nrm r2.xyz, r1
    mov r2.w, c15.w
    mad r2.w, r1.w, -r2.w, c3.x
    dp3_sat r0.z, t3, r2
    dp3 r0.w, r2, -c16
    mov r1.w, c16.w
    mad_sat r0.w, r0.w, r1.w, c17.w
    mul r0.z, r0.z, r2.w
    mul r0.y, r0.w, r0.z
    max r1.xy, r0, t2.z
    min r0.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r0.xzw, r0.x, r1.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    dp3 r0.w, -c18, t3
    mul r1.x, r0.w, c9.z
    max r2.x, r0.w, r1.x
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xyz, r2.x, c19, r0
    mad_pp r0.xyz, r2.y, c21, r0
    add r1.xyz, -t0, c8
    dp3 r0.w, r1, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r1
    nrm r3.xyz, r1
    dp3 r0.w, t3, r3
    mov r1, c10
    mad_pp r1, r0.w, r1, c11
    mov r3.xy, t1.zwzw
    texld_pp r2, r2, s3
    texld_pp r3, r3, s2
    mul_pp r1.xyz, r1, r2
    mul_pp r0.w, r1.w, r3.z
    mul_pp r1.xyz, r0.w, r1
    texld_pp r2, t2, s1
    texld_pp r4, t1, s0
    mad_pp r0.xyz, r4, r0, r1
    mul_pp r0.xyz, r2, r0
    mul_pp r0.xyz, r0, c0.w
    add r1.xyz, r0, r0
    mov r2.xyz, c0
    mad r0.xyz, r0, -c3.y, r2
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 70 instruction slots used (4 texture, 66 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyComplexFog
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, true, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c12.w
    mad r1.w, r0.w, -r1.w, c3.x
    dp3_sat r0.x, t3, r1
    dp3 r0.y, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.y, r0.y, r0.w, c14.w
    mul r0.x, r0.x, r1.w
    mul r0.x, r0.y, r0.x
    mov r1.w, c15.w
    add r1.xyz, -t0, c15
    dp4 r1.w, r1, r1
    nrm r2.xyz, r1
    mov r2.w, c15.w
    mad r2.w, r1.w, -r2.w, c3.x
    dp3_sat r0.z, t3, r2
    dp3 r0.w, r2, -c16
    mov r1.w, c16.w
    mad_sat r0.w, r0.w, r1.w, c17.w
    mul r0.z, r0.z, r2.w
    mul r0.y, r0.w, r0.z
    max r1.xy, r0, t2.z
    min r0.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r0.xzw, r0.x, r1.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    dp3 r0.w, -c18, t3
    mul r1.x, r0.w, c9.z
    max r2.x, r0.w, r1.x
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xyz, r2.x, c19, r0
    mad_pp r0.xyz, r2.y, c21, r0
    add r1.xyz, -t0, c8
    dp3 r0.w, r1, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r1
    nrm r3.xyz, r1
    dp3 r0.w, t3, r3
    mov r1, c10
    mad_pp r1, r0.w, r1, c11
    mov r3.xy, t1.zwzw
    texld_pp r2, r2, s3
    texld_pp r3, r3, s2
    mul_pp r1.xyz, r1, r2
    mul_pp r0.w, r1.w, r3.z
    mul_pp r1.xyz, r0.w, r1
    texld_pp r2, t2, s1
    texld_pp r4, t1, s0
    mad_pp r0.xyz, r4, r0, r1
    mul_pp r0.xyz, r2, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 68 instruction slots used (4 texture, 64 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddComplexFog
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, true, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 1, 2, -1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    dp3 r0.z, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.z, r0.z, r0.w, c14.w
    mul r0.x, r0.y, r0.x
    mul r0.x, r0.z, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r1.xyz, r2
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r0.z, t3, r1
    dp3 r0.w, r1, -c16
    mov r2.w, c16.w
    mad_sat r0.w, r0.w, r2.w, c17.w
    mul r0.z, r0.z, r1.w
    mul r0.y, r0.w, r0.z
    max r1.xy, r0, t2.z
    min r0.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r0.xzw, r0.x, r1.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    dp3 r0.w, -c18, t3
    mul r1.x, r0.w, c9.z
    max r2.x, r0.w, r1.x
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xyz, r2.x, c19, r0
    mad_pp r0.xyz, r2.y, c21, r0
    add r1.xyz, -t0, c8
    dp3 r0.w, r1, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r1
    nrm r3.xyz, r1
    dp3 r0.w, t3, r3
    mov r1, c10
    mad_pp r1, r0.w, r1, c11
    mov r3.xy, t1.zwzw
    texld_pp r2, r2, s3
    texld_pp r3, r3, s2
    mul_pp r1.xyz, r1, r2
    mul_pp r0.w, r1.w, r3.z
    mul_pp r1.xyz, r0.w, r1
    texld_pp r2, t2, s1
    texld_pp r4, t1, s0
    mad_pp r0.xyz, r4, r0, r1
    mad_pp r0.xyz, r2, c3.y, r0
    add_pp r0.xyz, r0, c3.z
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 68 instruction slots used (4 texture, 64 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyComplexFog
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, true, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c12.w
    mad r1.w, r0.w, -r1.w, c3.x
    dp3_sat r0.x, t3, r1
    dp3 r0.y, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.y, r0.y, r0.w, c14.w
    mul r0.x, r0.x, r1.w
    mul r0.x, r0.y, r0.x
    mov r1.w, c15.w
    add r1.xyz, -t0, c15
    dp4 r1.w, r1, r1
    nrm r2.xyz, r1
    mov r2.w, c15.w
    mad r2.w, r1.w, -r2.w, c3.x
    dp3_sat r0.z, t3, r2
    dp3 r0.w, r2, -c16
    mov r1.w, c16.w
    mad_sat r0.w, r0.w, r1.w, c17.w
    mul r0.z, r0.z, r2.w
    mul r0.y, r0.w, r0.z
    max r1.xy, r0, t2.z
    min r0.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r0.xzw, r0.x, r1.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    dp3 r0.w, -c18, t3
    mul r1.x, r0.w, c9.z
    max r2.x, r0.w, r1.x
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xyz, r2.x, c19, r0
    mad_pp r0.xyz, r2.y, c21, r0
    texld_pp r1, t2, s1
    texld_pp r2, t1, s0
    mul_pp r1.xyz, r1, r2
    mul_pp r0.xyz, r0, r1
    add_pp r0.xyz, r0, r0
    add r1.xyz, -t0, c8
    dp3 r0.w, r1, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r1
    nrm r3.xyz, r1
    dp3 r0.w, t3, r3
    mov r1, c10
    mad_pp r1, r0.w, r1, c11
    mov r3.xy, t1.zwzw
    texld_pp r2, r2, s3
    texld_pp r3, r3, s2
    mul_pp r1.xyz, r1, r2
    mul_pp r0.w, r1.w, r3.z
    mad_pp r0.xyz, r1, r0.w, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 69 instruction slots used (4 texture, 65 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyComplexFog
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, true, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c12.w
    mad r1.w, r0.w, -r1.w, c3.x
    dp3_sat r0.x, t3, r1
    dp3 r0.y, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.y, r0.y, r0.w, c14.w
    mul r0.x, r0.x, r1.w
    mul r0.x, r0.y, r0.x
    mov r1.w, c15.w
    add r1.xyz, -t0, c15
    dp4 r1.w, r1, r1
    nrm r2.xyz, r1
    mov r2.w, c15.w
    mad r2.w, r1.w, -r2.w, c3.x
    dp3_sat r0.z, t3, r2
    dp3 r0.w, r2, -c16
    mov r1.w, c16.w
    mad_sat r0.w, r0.w, r1.w, c17.w
    mul r0.z, r0.z, r2.w
    mul r0.y, r0.w, r0.z
    max r1.xy, r0, t2.z
    min r0.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r0.xzw, r0.x, r1.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    dp3 r0.w, -c18, t3
    mul r1.x, r0.w, c9.z
    max r2.x, r0.w, r1.x
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xyz, r2.x, c19, r0
    mad_pp r0.xyz, r2.y, c21, r0
    add r1.xyz, -t0, c8
    dp3 r0.w, r1, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r1
    nrm r3.xyz, r1
    dp3 r0.w, t3, r3
    mov r1, c10
    mad_pp r1, r0.w, r1, c11
    mov r3.xy, t1.zwzw
    texld_pp r2, r2, s3
    texld_pp r3, r3, s2
    mul_pp r1.xyz, r1, r2
    mul_pp r0.w, r1.w, r3.z
    mul_pp r1.xyz, r0.w, r1
    texld_pp r2, t2, s1
    texld_pp r4, t1, s0
    mul_pp r2.xyz, r2, r4
    mad_pp r0.xyz, r2, r0, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 68 instruction slots used (4 texture, 64 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddComplexFog
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, true, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 1, 2, -1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    dp3 r0.z, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.z, r0.z, r0.w, c14.w
    mul r0.x, r0.y, r0.x
    mul r0.x, r0.z, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r1.xyz, r2
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r0.z, t3, r1
    dp3 r0.w, r1, -c16
    mov r2.w, c16.w
    mad_sat r0.w, r0.w, r2.w, c17.w
    mul r0.z, r0.z, r1.w
    mul r0.y, r0.w, r0.z
    max r1.xy, r0, t2.z
    min r0.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r0.xzw, r0.x, r1.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    dp3 r0.w, -c18, t3
    mul r1.x, r0.w, c9.z
    max r2.x, r0.w, r1.x
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xyz, r2.x, c19, r0
    mad_pp r0.xyz, r2.y, c21, r0
    add r1.xyz, -t0, c8
    dp3 r0.w, r1, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r1
    nrm r3.xyz, r1
    dp3 r0.w, t3, r3
    mov r1, c10
    mad_pp r1, r0.w, r1, c11
    mov r3.xy, t1.zwzw
    texld_pp r2, r2, s3
    texld_pp r3, r3, s2
    mul_pp r1.xyz, r1, r2
    mul_pp r0.w, r1.w, r3.z
    mul_pp r1.xyz, r0.w, r1
    texld_pp r2, t2, s1
    texld_pp r4, t1, s0
    mad_pp r2.xyz, r2, c3.y, r4
    add_pp r2.xyz, r2, c3.z
    mad_pp r0.xyz, r2, r0, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 68 instruction slots used (4 texture, 64 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyBaseNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, false, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c0.x, c0.y
    mul r0.yzw, r0, c5.xxxy
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r0.xyz, r4, r1, r0
    mul_pp r0.xyz, r3, r0
    add_pp r2.xyz, r0, r0
    mov_pp oC0, r2

// approximately 68 instruction slots used (5 texture, 63 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyBaseNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, false, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c0.x, c0.y
    mul r0.yzw, r0, c5.xxxy
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r0.xyz, r4, r1, r0
    mul_pp r2.xyz, r3, r0
    mov_pp oC0, r2

// approximately 67 instruction slots used (5 texture, 62 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddBaseNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, false, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c0.x, c0.y
    mul r0.yzw, r0, c5.xxxy
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r0.xyz, r4, r1, r0
    mad_pp r0.xyz, r3, c0.x, r0
    add_pp r2.xyz, r0, c0.y
    mov_pp oC0, r2

// approximately 68 instruction slots used (5 texture, 63 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyBaseNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, false, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c0.x, c0.y
    mul r0.yzw, r0, c5.xxxy
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mul_pp r2.xyz, r2, r3
    mul_pp r1.xyz, r1, r2
    add_pp r1.xyz, r1, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mad_pp r2.xyz, r0, r0.w, r1
    mov_pp oC0, r2

// approximately 68 instruction slots used (5 texture, 63 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyBaseNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, false, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c0.x, c0.y
    mul r0.yzw, r0, c5.xxxy
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mul_pp r2.xyz, r2, r3
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r3.xy, t1.zwzw
    texld_pp r4, r4, s3
    texld_pp r3, r3, s2
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r3.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 67 instruction slots used (5 texture, 62 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddBaseNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, false, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c0.x, c0.y
    mul r0.yzw, r0, c5.xxxy
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r2.xyz, r2, c0.x, r3
    add_pp r2.xyz, r2, c0.y
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r3.xy, t1.zwzw
    texld_pp r4, r4, s3
    texld_pp r3, r3, s2
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r3.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 68 instruction slots used (5 texture, 63 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyComplexFogBaseNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, true, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r0.xz, c3
    mad r1.w, r0.w, -c12.w, r0.z
    texld r2, t1, s4
    mad r2.xyz, r2, c3.x, c3.y
    mul r2.xyz, r2, c5.xxyw
    mul r3.xyz, r2.y, t4
    mad r2.xyw, t5.xyzz, r2.x, r3.xyzz
    mad r2.xyz, t3, r2.z, r2.xyww
    dp3_sat r2.w, r2, r1
    dp3 r0.y, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.y, r0.y, r0.w, c14.w
    mul r0.w, r1.w, r2.w
    mul r1.x, r0.y, r0.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r4.w, r2, r4
    dp3 r0.y, r4, -c16
    mov r0.w, c16.w
    mad_sat r0.y, r0.y, r0.w, c17.w
    mul r2.w, r2.w, r4.w
    mul r1.y, r0.y, r2.w
    max r0.yz, r1.xxyw, t2.z
    min r1.xy, t2.w, r0.yzzw
    mov r3.xyz, c14
    mad_pp r0.yzw, r1.x, r3.xxyz, c22.xxyz
    mad_pp r0.yzw, r1.y, c17.xxyz, r0
    dp3 r2.w, -c18, r2
    mul r1.x, r2.w, c9.z
    max r3.x, r2.w, r1.x
    dp3 r3.y, -c20, r2
    max r1.xy, r3, t2.z
    min r3.xy, t2.w, r1
    mad_pp r0.yzw, r3.x, c19.xxyz, r0
    mad_pp r0.yzw, r3.y, c21.xxyz, r0
    add r1.xyz, -t0, c8
    dp3 r1.w, r1, r2
    mul r3.xyz, r2, r1.w
    mad_pp r3.xyz, r3, c8.w, -r1
    nrm r4.xyz, r1
    dp3 r3.w, r2, r4
    mov r1, c10
    mad_pp r1, r3.w, r1, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r1.xyz, r1, r3
    mul_pp r1.w, r1.w, r2.z
    mul_pp r1.xyz, r1.w, r1
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r0.yzw, r4.xxyz, r0, r1.xxyz
    mul_pp r0.yzw, r3.xxyz, r0
    mul_pp r0.yzw, r0, c0.w
    add r1.xyz, r0.yzww, r0.yzww
    mad r0.xyz, r0.yzww, -r0.x, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 74 instruction slots used (5 texture, 69 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyComplexFogBaseNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, true, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c3.x, c3.y
    mul r0.yzw, r0, c5.xxxy
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r0.xyz, r4, r1, r0
    mul_pp r0.xyz, r3, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 73 instruction slots used (5 texture, 68 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddComplexFogBaseNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, true, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c3.x, c3.y
    mul r0.yzw, r0, c5.xxxy
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r0.xyz, r4, r1, r0
    mad_pp r0.xyz, r3, c3.x, r0
    add_pp r0.xyz, r0, c3.y
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 74 instruction slots used (5 texture, 69 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyComplexFogBaseNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, true, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c3.x, c3.y
    mul r0.yzw, r0, c5.xxxy
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mul_pp r2.xyz, r2, r3
    mul_pp r1.xyz, r1, r2
    add_pp r1.xyz, r1, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 74 instruction slots used (5 texture, 69 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyComplexFogBaseNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, true, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c3.x, c3.y
    mul r0.yzw, r0, c5.xxxy
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mul_pp r2.xyz, r2, r3
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r3.xy, t1.zwzw
    texld_pp r4, r4, s3
    texld_pp r3, r3, s2
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r3.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r2, r1, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 73 instruction slots used (5 texture, 68 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddComplexFogBaseNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, true, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c3.x, c3.y
    mul r0.yzw, r0, c5.xxxy
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r2.xyz, r2, c3.x, r3
    add_pp r2.xyz, r2, c3.y
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r3.xy, t1.zwzw
    texld_pp r4, r4, s3
    texld_pp r3, r3, s2
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r3.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r2, r1, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 74 instruction slots used (5 texture, 69 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyBaseNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, false, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c0.x, c0.y
    mul r0.yzw, r0, c5.xxxy
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    add_pp r2.xyz, r0, r0
    mov_pp oC0, r2

// approximately 78 instruction slots used (6 texture, 72 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyBaseNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, false, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c0.x, c0.y
    mul r0.yzw, r0, c5.xxxy
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    texld_pp r1, t2, s1
    mul_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 77 instruction slots used (6 texture, 71 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddBaseNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, false, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c0.x, c0.y
    mul r0.yzw, r0, c5.xxxy
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c0.x, r0
    add_pp r2.xyz, r0, c0.y
    mov_pp oC0, r2

// approximately 78 instruction slots used (6 texture, 72 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyBaseNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, false, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c0.x, c0.y
    mul r0.yzw, r0, c5.xxxy
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mul_pp r2.xyz, r2, r3
    mul_pp r1.xyz, r1, r2
    add_pp r1.xyz, r1, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mad_pp r2.xyz, r0, r0.w, r1
    mov_pp oC0, r2

// approximately 78 instruction slots used (6 texture, 72 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyBaseNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, false, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c0.x, c0.y
    mul r0.yzw, r0, c5.xxxy
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mul_pp r3.xyz, r3, r4
    mad_pp r2.xyz, r3, r1, r0
    mov_pp oC0, r2

// approximately 77 instruction slots used (6 texture, 71 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddBaseNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, false, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c0.x, c0.y
    mul r0.yzw, r0, c5.xxxy
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r2.xyz, r2, c0.x, r3
    add_pp r2.xyz, r2, c0.y
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r3, t1, s7
    texld_pp r4, r4, s3
    log r5.x, r3.x
    log r5.y, r3.y
    log r5.z, r3.z
    mul r3.xyz, r5, c5.z
    exp r5.x, r3.x
    exp r5.y, r3.y
    exp r5.z, r3.z
    mul_sat_pp r3.xyz, r5, c5.w
    mul_pp r0.xyz, r0, r3
    mul_pp r0.xyz, r0, r4
    mov r3.xy, t1.zwzw
    texld_pp r3, r3, s2
    mul_pp r0.w, r0.w, r3.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 78 instruction slots used (6 texture, 72 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyComplexFogBaseNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, true, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r0.xz, c3
    mad r1.w, r0.w, -c12.w, r0.z
    texld r2, t1, s4
    mad r2.xyz, r2, c3.x, c3.y
    mul r2.xyz, r2, c5.xxyw
    mul r3.xyz, r2.y, t4
    mad r2.xyw, t5.xyzz, r2.x, r3.xyzz
    mad r2.xyz, t3, r2.z, r2.xyww
    dp3_sat r2.w, r2, r1
    dp3 r0.y, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.y, r0.y, r0.w, c14.w
    mul r0.w, r1.w, r2.w
    mul r1.x, r0.y, r0.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r4.w, r2, r4
    dp3 r0.y, r4, -c16
    mov r0.w, c16.w
    mad_sat r0.y, r0.y, r0.w, c17.w
    mul r2.w, r2.w, r4.w
    mul r1.y, r0.y, r2.w
    max r0.yz, r1.xxyw, t2.z
    min r1.xy, t2.w, r0.yzzw
    mov r3.xyz, c14
    mad_pp r0.yzw, r1.x, r3.xxyz, c22.xxyz
    mad_pp r0.yzw, r1.y, c17.xxyz, r0
    dp3 r2.w, -c18, r2
    mul r1.x, r2.w, c9.z
    max r3.x, r2.w, r1.x
    dp3 r3.y, -c20, r2
    max r1.xy, r3, t2.z
    min r3.xy, t2.w, r1
    mad_pp r0.yzw, r3.x, c19.xxyz, r0
    mad_pp r0.yzw, r3.y, c21.xxyz, r0
    add r1.xyz, -t0, c8
    dp3 r1.w, r1, r2
    mul r3.xyz, r2, r1.w
    mad_pp r3.xyz, r3, c8.w, -r1
    nrm r4.xyz, r1
    dp3 r3.w, r2, r4
    mov r1, c10
    mad_pp r1, r3.w, r1, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r1.xyz, r1, r2
    mul_pp r1.xyz, r1, r3
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mul_pp r1.w, r1.w, r2.z
    mul_pp r1.xyz, r1.w, r1
    mad_pp r0.yzw, r3.xxyz, r0, r1.xxyz
    texld_pp r1, t2, s1
    mul_pp r0.yzw, r0, r1.xxyz
    mul_pp r0.yzw, r0, c0.w
    add r1.xyz, r0.yzww, r0.yzww
    mad r0.xyz, r0.yzww, -r0.x, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 84 instruction slots used (6 texture, 78 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyComplexFogBaseNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, true, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c3.x, c3.y
    mul r0.yzw, r0, c5.xxxy
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 83 instruction slots used (6 texture, 77 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddComplexFogBaseNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, true, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c3.x, c3.y
    mul r0.yzw, r0, c5.xxxy
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c3.x, r0
    add_pp r0.xyz, r0, c3.y
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 84 instruction slots used (6 texture, 78 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyComplexFogBaseNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, true, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c3.x, c3.y
    mul r0.yzw, r0, c5.xxxy
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mul_pp r2.xyz, r2, r3
    mul_pp r1.xyz, r1, r2
    add_pp r1.xyz, r1, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 84 instruction slots used (6 texture, 78 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyComplexFogBaseNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, true, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c3.x, c3.y
    mul r0.yzw, r0, c5.xxxy
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mul_pp r3.xyz, r3, r4
    mad_pp r0.xyz, r3, r1, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 83 instruction slots used (6 texture, 77 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddComplexFogBaseNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, true, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c3.x, c3.y
    mul r0.yzw, r0, c5.xxxy
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r2.xyz, r2, c3.x, r3
    add_pp r2.xyz, r2, c3.y
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r3, t1, s7
    texld_pp r4, r4, s3
    log r5.x, r3.x
    log r5.y, r3.y
    log r5.z, r3.z
    mul r3.xyz, r5, c5.z
    exp r5.x, r3.x
    exp r5.y, r3.y
    exp r5.z, r3.z
    mul_sat_pp r3.xyz, r5, c5.w
    mul_pp r0.xyz, r0, r3
    mul_pp r0.xyz, r0, r4
    mov r3.xy, t1.zwzw
    texld_pp r3, r3, s2
    mul_pp r0.w, r0.w, r3.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r2, r1, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 84 instruction slots used (6 texture, 78 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyBaseNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, false, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c0.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c0.x, c0.y
    mul r0.xyw, r0, c5.xxzy
    mul r3.xyz, r0.y, t4
    mad r3.xyz, t5, r0.x, r3
    mad r0.xyw, t3.xyzz, r0.w, r3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c0.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s0
    texld_pp r6, r6, s2
    mul_pp r1.xyz, r1, r6.z
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r2, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul_pp r0.xyz, r2, r0
    add_pp r6.xyz, r0, r0
    mov_pp oC0, r6

// approximately 113 instruction slots used (5 texture, 108 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyBaseNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, false, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c0.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c0.x, c0.y
    mul r0.xyw, r0, c5.xxzy
    mul r3.xyz, r0.y, t4
    mad r3.xyz, t5, r0.x, r3
    mad r0.xyw, t3.xyzz, r0.w, r3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c0.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s0
    texld_pp r6, r6, s2
    mul_pp r1.xyz, r1, r6.z
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r2, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul_pp r6.xyz, r2, r0
    mov_pp oC0, r6

// approximately 112 instruction slots used (5 texture, 107 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddBaseNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, false, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c0.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c0.x, c0.y
    mul r0.xyw, r0, c5.xxzy
    mul r3.xyz, r0.y, t4
    mad r3.xyz, t5, r0.x, r3
    mad r0.xyw, t3.xyzz, r0.w, r3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c0.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s0
    texld_pp r6, r6, s2
    mul_pp r1.xyz, r1, r6.z
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r2, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    mad_pp r0.xyz, r2, c0.x, r0
    add_pp r6.xyz, r0, c0.y
    mov_pp oC0, r6

// approximately 113 instruction slots used (5 texture, 108 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyBaseNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, false, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c0.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c0.x, c0.y
    mul r0.xyw, r0, c5.xxzy
    mul r3.xyz, r0.y, t4
    mad r3.xyz, t5, r0.x, r3
    mad r0.xyw, t3.xyzz, r0.w, r3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c0.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    texld_pp r3, t2, s1
    texld_pp r6, t1, s0
    mul_pp r3.xyz, r3, r6
    mul_pp r2.xyz, r2, r3
    add_pp r2.xyz, r2, r2
    mov r3.xy, t1.zwzw
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, r3, s2
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r1, r3.z, r2
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r3.z
    mad_pp r3.xyz, r0, r0.w, r1
    mov_pp oC0, r3

// approximately 113 instruction slots used (5 texture, 108 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyBaseNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, false, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c0.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c0.x, c0.y
    mul r0.xyw, r0, c5.xxzy
    mul r3.xyz, r0.y, t4
    mad r3.xyz, t5, r0.x, r3
    mad r0.xyw, t3.xyzz, r0.w, r3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c0.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mov r6.xy, t1.zwzw
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r5, r6, s2
    texld_pp r0, r0, s3
    mul_pp r1.xyz, r1, r5.z
    texld_pp r6, t2, s1
    texld_pp r8, t1, s0
    mul_pp r6.xyz, r6, r8
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r6, r2, r1
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r5.z
    mad_pp r5.xyz, r0, r0.w, r1
    mov_pp oC0, r5

// approximately 112 instruction slots used (5 texture, 107 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddBaseNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, false, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c0.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c0.x, c0.y
    mul r0.xyw, r0, c5.xxzy
    mul r3.xyz, r0.y, t4
    mad r3.xyz, t5, r0.x, r3
    mad r0.xyw, t3.xyzz, r0.w, r3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c0.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mov r6.xy, t1.zwzw
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r5, r6, s2
    texld_pp r0, r0, s3
    mul_pp r1.xyz, r1, r5.z
    texld_pp r6, t2, s1
    texld_pp r8, t1, s0
    mad_pp r6.xyz, r6, c0.x, r8
    add_pp r6.xyz, r6, c0.y
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r6, r2, r1
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r5.z
    mad_pp r5.xyz, r0, r0.w, r1
    mov_pp oC0, r5

// approximately 113 instruction slots used (5 texture, 108 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyComplexFogBaseNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, true, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.xz, c3
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r3.xyz, r3, c3.x, c3.y
    mul r3.xyz, r3, c5.xxyw
    mul r4.xyz, r3.y, t4
    mad r3.xyw, t5.xyzz, r3.x, r4.xyzz
    mad r3.xyz, t3, r3.z, r3.xyww
    dp3_sat r3.w, r3, r1
    mul r2.w, r2.w, r3.w
    mul r4.x, r1.w, r2.w
    add r5.xyz, -t0, c15
    nrm r6.xyz, r5
    dp3 r1.w, r6, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r5.w, c15.w
    dp4 r2.w, r5, r5
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r3.w, r3, r6
    mul r2.w, r2.w, r3.w
    mul r4.y, r1.w, r2.w
    max r0.yz, r4.xxyw, t2.z
    min r4.xy, t2.w, r0.yzzw
    dp3 r1.w, -r6, r3
    add r1.w, r1.w, r1.w
    mad r0.yzw, r3.xxyz, -r1.w, -r6.xxyz
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r0.yzww, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r0.yzw, r1.w, c17.xxyz
    mul r0.yzw, r4.y, r0
    dp3 r1.w, -r1, r3
    add r1.w, r1.w, r1.w
    mad r1.xyz, r3, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r3.w, r2.w, c3.w
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r0.yzw, r1.xxyz, r4.x, r0
    dp3 r2.w, c18, r3
    add r2.w, r2.w, r2.w
    mad r1.xyz, r3, -r2.w, c18
    dp3 r2.w, r1, r6
    max r3.w, r2.w, c3.w
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c19
    dp3 r1.w, -c18, r3
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r3
    max r4.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r4.zwzw
    mad_pp r0.yzw, r1.xxyz, r7.x, r0
    dp3 r2.w, c20, r3
    add r2.w, r2.w, r2.w
    mad r1.xyz, r3, -r2.w, c20
    dp3 r2.w, r1, r6
    dp3 r3.w, r3, r6
    mov r1, c10
    mad_pp r1, r3.w, r1, c11
    max r3.w, r2.w, c3.w
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r6.xyz, r2.w, c21
    mad_pp r0.yzw, r6.xxyz, r7.y, r0
    mul_sat_pp r0.yzw, r1.xxyz, r0
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s0
    texld_pp r6, r6, s2
    mul_pp r0.yzw, r0, r6.z
    mad_pp r2.xyz, r4.x, r2, c22
    mad_pp r2.xyz, r4.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r0.yzw, r8.xxyz, r2.xxyz, r0
    dp3 r3.w, r5, r3
    mul r2.xyz, r3, r3.w
    mad_pp r2.xyz, r2, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r2, r2, s3
    mul_pp r1.xyz, r1, r2
    mul_pp r1.w, r1.w, r6.z
    mad_pp r0.yzw, r1.xxyz, r1.w, r0
    mul_pp r0.yzw, r3.xxyz, r0
    mul_pp r0.yzw, r0, c0.w
    add r1.xyz, r0.yzww, r0.yzww
    mad r0.xyz, r0.yzww, -r0.x, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 119 instruction slots used (5 texture, 114 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyComplexFogBaseNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, true, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c3.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c3.x, c3.y
    mul r0.xyw, r0, c5.xxzy
    mul r3.xyz, r0.y, t4
    mad r3.xyz, t5, r0.x, r3
    mad r0.xyw, t3.xyzz, r0.w, r3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c3.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s0
    texld_pp r6, r6, s2
    mul_pp r1.xyz, r1, r6.z
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r2, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul_pp r0.xyz, r2, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 118 instruction slots used (5 texture, 113 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddComplexFogBaseNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, true, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c3.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c3.x, c3.y
    mul r0.xyw, r0, c5.xxzy
    mul r3.xyz, r0.y, t4
    mad r3.xyz, t5, r0.x, r3
    mad r0.xyw, t3.xyzz, r0.w, r3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c3.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s0
    texld_pp r6, r6, s2
    mul_pp r1.xyz, r1, r6.z
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r2, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    mad_pp r0.xyz, r2, c3.x, r0
    add_pp r0.xyz, r0, c3.y
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 119 instruction slots used (5 texture, 114 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyComplexFogBaseNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, true, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c3.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c3.x, c3.y
    mul r0.xyw, r0, c5.xxzy
    mul r3.xyz, r0.y, t4
    mad r3.xyz, t5, r0.x, r3
    mad r0.xyw, t3.xyzz, r0.w, r3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c3.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    texld_pp r3, t2, s1
    texld_pp r6, t1, s0
    mul_pp r3.xyz, r3, r6
    mul_pp r2.xyz, r2, r3
    add_pp r2.xyz, r2, r2
    mov r3.xy, t1.zwzw
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, r3, s2
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r1, r3.z, r2
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r3.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 119 instruction slots used (5 texture, 114 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyComplexFogBaseNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, true, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c3.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c3.x, c3.y
    mul r0.xyw, r0, c5.xxzy
    mul r3.xyz, r0.y, t4
    mad r3.xyz, t5, r0.x, r3
    mad r0.xyw, t3.xyzz, r0.w, r3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c3.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mov r6.xy, t1.zwzw
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r5, r6, s2
    texld_pp r0, r0, s3
    mul_pp r1.xyz, r1, r5.z
    texld_pp r6, t2, s1
    texld_pp r8, t1, s0
    mul_pp r6.xyz, r6, r8
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r6, r2, r1
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r5.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r5.xyz, r0, r1
    mov_pp oC0, r5

// approximately 118 instruction slots used (5 texture, 113 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddComplexFogBaseNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, true, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c3.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c3.x, c3.y
    mul r0.xyw, r0, c5.xxzy
    mul r3.xyz, r0.y, t4
    mad r3.xyz, t5, r0.x, r3
    mad r0.xyw, t3.xyzz, r0.w, r3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c3.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mov r6.xy, t1.zwzw
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r5, r6, s2
    texld_pp r0, r0, s3
    mul_pp r1.xyz, r1, r5.z
    texld_pp r6, t2, s1
    texld_pp r8, t1, s0
    mad_pp r6.xyz, r6, c3.x, r8
    add_pp r6.xyz, r6, c3.y
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r6, r2, r1
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r5.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r5.xyz, r0, r1
    mov_pp oC0, r5

// approximately 119 instruction slots used (5 texture, 114 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyBaseNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, false, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s7
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c0.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c0.x, c0.y
    mul r0.xyw, r0, c5.xxzy
    mul r3.xyz, r0.y, t4
    mad r3.xyz, t5, r0.x, r3
    mad r0.xyw, t3.xyzz, r0.w, r3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c0.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r6, r6, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r4.xyz, r4, r8
    mul_sat_pp r1.xyz, r1, r4
    mul_pp r1.xyz, r6.z, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r5, t1, s0
    texld_pp r0, r0, s3
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r5, r2, r1
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    add_pp r6.xyz, r0, r0
    mov_pp oC0, r6

// approximately 123 instruction slots used (6 texture, 117 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyBaseNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, false, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s7
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c0.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c0.x, c0.y
    mul r0.xyw, r0, c5.xxzy
    mul r3.xyz, r0.y, t4
    mad r3.xyz, t5, r0.x, r3
    mad r0.xyw, t3.xyzz, r0.w, r3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c0.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r6, r6, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r4.xyz, r4, r8
    mul_sat_pp r1.xyz, r1, r4
    mul_pp r1.xyz, r6.z, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r5, t1, s0
    texld_pp r0, r0, s3
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r5, r2, r1
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    texld_pp r1, t2, s1
    mul_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 122 instruction slots used (6 texture, 116 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddBaseNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, false, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s7
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c0.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c0.x, c0.y
    mul r0.xyw, r0, c5.xxzy
    mul r3.xyz, r0.y, t4
    mad r3.xyz, t5, r0.x, r3
    mad r0.xyw, t3.xyzz, r0.w, r3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c0.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r6, r6, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r4.xyz, r4, r8
    mul_sat_pp r1.xyz, r1, r4
    mul_pp r1.xyz, r6.z, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r5, t1, s0
    texld_pp r0, r0, s3
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r5, r2, r1
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c0.x, r0
    add_pp r6.xyz, r0, c0.y
    mov_pp oC0, r6

// approximately 123 instruction slots used (6 texture, 117 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyBaseNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, false, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s7
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c0.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c0.x, c0.y
    mul r0.xyw, r0, c5.xxzy
    mul r3.xyz, r0.y, t4
    mad r3.xyz, t5, r0.x, r3
    mad r0.xyw, t3.xyzz, r0.w, r3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c0.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r6, r6, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r4.xyz, r4, r8
    mul_sat_pp r1.xyz, r1, r4
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    texld_pp r3, t2, s1
    texld_pp r7, t1, s0
    mul_pp r3.xyz, r3, r7
    mul_pp r2.xyz, r2, r3
    add_pp r2.xyz, r2, r2
    mad_pp r1.xyz, r1, r6.z, r2
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r6.xyz, r0, r0.w, r1
    mov_pp oC0, r6

// approximately 123 instruction slots used (6 texture, 117 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyBaseNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, false, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s7
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c0.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c0.x, c0.y
    mul r0.xyw, r0, c5.xxzy
    mul r3.xyz, r0.y, t4
    mad r3.xyz, t5, r0.x, r3
    mad r0.xyw, t3.xyzz, r0.w, r3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c0.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r6, r6, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r4.xyz, r4, r8
    mul_sat_pp r1.xyz, r1, r4
    mul_pp r1.xyz, r6.z, r1
    texld_pp r8, t2, s1
    texld_pp r9, t1, s0
    mul_pp r8.xyz, r8, r9
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r6.xyz, r0, r0.w, r1
    mov_pp oC0, r6

// approximately 122 instruction slots used (6 texture, 116 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddBaseNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, false, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s7
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c0.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c0.x, c0.y
    mul r0.xyw, r0, c5.xxzy
    mul r3.xyz, r0.y, t4
    mad r3.xyz, t5, r0.x, r3
    mad r0.xyw, t3.xyzz, r0.w, r3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c0.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r6, r6, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r4.xyz, r4, r8
    mul_sat_pp r1.xyz, r1, r4
    mul_pp r1.xyz, r6.z, r1
    texld_pp r8, t2, s1
    texld_pp r9, t1, s0
    mad_pp r8.xyz, r8, c0.x, r9
    add_pp r8.xyz, r8, c0.y
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r6.xyz, r0, r0.w, r1
    mov_pp oC0, r6

// approximately 123 instruction slots used (6 texture, 117 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyComplexFogBaseNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, true, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s7
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.xz, c3
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r3.xyz, r3, c3.x, c3.y
    mul r3.xyz, r3, c5.xxyw
    mul r4.xyz, r3.y, t4
    mad r3.xyw, t5.xyzz, r3.x, r4.xyzz
    mad r3.xyz, t3, r3.z, r3.xyww
    dp3_sat r3.w, r3, r1
    mul r2.w, r2.w, r3.w
    mul r4.x, r1.w, r2.w
    add r5.xyz, -t0, c15
    nrm r6.xyz, r5
    dp3 r1.w, r6, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r5.w, c15.w
    dp4 r2.w, r5, r5
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r3.w, r3, r6
    mul r2.w, r2.w, r3.w
    mul r4.y, r1.w, r2.w
    max r0.yz, r4.xxyw, t2.z
    min r4.xy, t2.w, r0.yzzw
    dp3 r1.w, -r6, r3
    add r1.w, r1.w, r1.w
    mad r0.yzw, r3.xxyz, -r1.w, -r6.xxyz
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r0.yzww, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r0.yzw, r1.w, c17.xxyz
    mul r0.yzw, r4.y, r0
    dp3 r1.w, -r1, r3
    add r1.w, r1.w, r1.w
    mad r1.xyz, r3, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r3.w, r2.w, c3.w
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r0.yzw, r1.xxyz, r4.x, r0
    dp3 r2.w, c18, r3
    add r2.w, r2.w, r2.w
    mad r1.xyz, r3, -r2.w, c18
    dp3 r2.w, r1, r6
    max r3.w, r2.w, c3.w
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c19
    dp3 r1.w, -c18, r3
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r3
    max r4.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r4.zwzw
    mad_pp r0.yzw, r1.xxyz, r7.x, r0
    dp3 r2.w, c20, r3
    add r2.w, r2.w, r2.w
    mad r1.xyz, r3, -r2.w, c20
    dp3 r2.w, r1, r6
    dp3 r3.w, r3, r6
    mov r1, c10
    mad_pp r1, r3.w, r1, c11
    max r3.w, r2.w, c3.w
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r6.xyz, r2.w, c21
    mad_pp r0.yzw, r6.xxyz, r7.y, r0
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r6, r6, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r1.xyz, r1, r8
    mul_sat_pp r0.yzw, r0, r1.xxyz
    mul_pp r0.yzw, r6.z, r0
    dp3 r2.w, r5, r3
    mul r3.xyz, r3, r2.w
    mad_pp r3.xyz, r3, c8.w, -r5
    texld_pp r5, t1, s0
    texld_pp r3, r3, s3
    mad_pp r2.xyz, r4.x, r2, c22
    mad_pp r2.xyz, r4.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r0.yzw, r5.xxyz, r2.xxyz, r0
    mul_pp r1.xyz, r1, r3
    mul_pp r1.w, r1.w, r6.z
    mad_pp r0.yzw, r1.xxyz, r1.w, r0
    texld_pp r1, t2, s1
    mul_pp r0.yzw, r0, r1.xxyz
    mul_pp r0.yzw, r0, c0.w
    add r1.xyz, r0.yzww, r0.yzww
    mad r0.xyz, r0.yzww, -r0.x, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 129 instruction slots used (6 texture, 123 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyComplexFogBaseNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, true, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s7
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c3.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c3.x, c3.y
    mul r0.xyw, r0, c5.xxzy
    mul r3.xyz, r0.y, t4
    mad r3.xyz, t5, r0.x, r3
    mad r0.xyw, t3.xyzz, r0.w, r3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c3.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r6, r6, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r4.xyz, r4, r8
    mul_sat_pp r1.xyz, r1, r4
    mul_pp r1.xyz, r6.z, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r5, t1, s0
    texld_pp r0, r0, s3
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r5, r2, r1
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 128 instruction slots used (6 texture, 122 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddComplexFogBaseNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, true, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s7
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c3.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c3.x, c3.y
    mul r0.xyw, r0, c5.xxzy
    mul r3.xyz, r0.y, t4
    mad r3.xyz, t5, r0.x, r3
    mad r0.xyw, t3.xyzz, r0.w, r3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c3.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r6, r6, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r4.xyz, r4, r8
    mul_sat_pp r1.xyz, r1, r4
    mul_pp r1.xyz, r6.z, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r5, t1, s0
    texld_pp r0, r0, s3
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r5, r2, r1
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c3.x, r0
    add_pp r0.xyz, r0, c3.y
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 129 instruction slots used (6 texture, 123 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyComplexFogBaseNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, true, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s7
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c3.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c3.x, c3.y
    mul r0.xyw, r0, c5.xxzy
    mul r3.xyz, r0.y, t4
    mad r3.xyz, t5, r0.x, r3
    mad r0.xyw, t3.xyzz, r0.w, r3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c3.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r6, r6, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r4.xyz, r4, r8
    mul_sat_pp r1.xyz, r1, r4
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    texld_pp r3, t2, s1
    texld_pp r7, t1, s0
    mul_pp r3.xyz, r3, r7
    mul_pp r2.xyz, r2, r3
    add_pp r2.xyz, r2, r2
    mad_pp r1.xyz, r1, r6.z, r2
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 129 instruction slots used (6 texture, 123 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyComplexFogBaseNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, true, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s7
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c3.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c3.x, c3.y
    mul r0.xyw, r0, c5.xxzy
    mul r3.xyz, r0.y, t4
    mad r3.xyz, t5, r0.x, r3
    mad r0.xyw, t3.xyzz, r0.w, r3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c3.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r6, r6, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r4.xyz, r4, r8
    mul_sat_pp r1.xyz, r1, r4
    mul_pp r1.xyz, r6.z, r1
    texld_pp r8, t2, s1
    texld_pp r9, t1, s0
    mul_pp r8.xyz, r8, r9
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 128 instruction slots used (6 texture, 122 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddComplexFogBaseNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, true, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s7
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c3.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c3.x, c3.y
    mul r0.xyw, r0, c5.xxzy
    mul r3.xyz, r0.y, t4
    mad r3.xyz, t5, r0.x, r3
    mad r0.xyw, t3.xyzz, r0.w, r3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c3.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r6, r6, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r4.xyz, r4, r8
    mul_sat_pp r1.xyz, r1, r4
    mul_pp r1.xyz, r6.z, r1
    texld_pp r8, t2, s1
    texld_pp r9, t1, s0
    mad_pp r8.xyz, r8, c3.x, r9
    add_pp r8.xyz, r8, c3.y
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 129 instruction slots used (6 texture, 123 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyBaseNormDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, false, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 3, -2
    def c1, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    mov r0.xy, t6.zwzw
    texld r1, t6, s5
    texld r0, r0, s6
    mad r0.xy, r0, c0.x, c0.y
    mad r0.zw, r1.xyxy, c0.x, c0.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c0.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c0.z, c0.ywzw
    mad r1.xyz, r1, c0.x, c0.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov_pp r0.z, c1.x
    mad r0.xyz, r1, c5.xxyw, r0
    mul r1.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r1.xyzz
    mad r0.xyz, t3, r0.z, r0.xyww
    add r1.xyz, -t0, c12
    nrm r2.xyz, r1
    dp3_sat r0.w, r0, r2
    dp3 r2.x, r2, -c13
    mov r2.w, c13.w
    mad_sat r2.x, r2.x, r2.w, c14.w
    mov r1.w, c12.w
    dp4 r1.x, r1, r1
    mov r1.y, c0.y
    mad r1.x, r1.x, -c12.w, -r1.y
    mul r0.w, r0.w, r1.x
    mul r2.x, r2.x, r0.w
    add r3.xyz, -t0, c15
    nrm r4.xyz, r3
    dp3_sat r0.w, r0, r4
    dp3 r1.x, r4, -c16
    mov r1.w, c16.w
    mad_sat r1.x, r1.x, r1.w, c17.w
    mov r3.w, c15.w
    dp4 r1.z, r3, r3
    mad r1.y, r1.z, -c15.w, -r1.y
    mul r0.w, r0.w, r1.y
    mul r2.y, r1.x, r0.w
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r1.xyz, r2.x, r1, c22
    mad_pp r1.xyz, r2.y, c17, r1
    dp3 r0.w, -c18, r0
    mul r1.w, r0.w, c9.z
    max r2.x, r0.w, r1.w
    dp3 r2.y, -c20, r0
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r0.xyz, r4, r1, r0
    mul_pp r0.xyz, r3, r0
    add_pp r2.xyz, r0, r0
    mov_pp oC0, r2

// approximately 83 instruction slots used (7 texture, 76 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyBaseNormDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, false, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 3, -2
    def c1, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    mov r0.xy, t6.zwzw
    texld r1, t6, s5
    texld r0, r0, s6
    mad r0.xy, r0, c0.x, c0.y
    mad r0.zw, r1.xyxy, c0.x, c0.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c0.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c0.z, c0.ywzw
    mad r1.xyz, r1, c0.x, c0.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c1.x
    mad r0.xyz, r1, c5.xxyw, r0
    mul r1.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r1.xyzz
    mad r0.xyz, t3, r0.z, r0.xyww
    add r1.xyz, -t0, c12
    nrm r2.xyz, r1
    dp3_sat r0.w, r0, r2
    dp3 r2.x, r2, -c13
    mov r2.w, c13.w
    mad_sat r2.x, r2.x, r2.w, c14.w
    mov r1.w, c12.w
    dp4 r1.x, r1, r1
    mov r1.y, c0.y
    mad r1.x, r1.x, -c12.w, -r1.y
    mul r0.w, r0.w, r1.x
    mul r2.x, r2.x, r0.w
    add r3.xyz, -t0, c15
    nrm r4.xyz, r3
    dp3_sat r0.w, r0, r4
    dp3 r1.x, r4, -c16
    mov r1.w, c16.w
    mad_sat r1.x, r1.x, r1.w, c17.w
    mov r3.w, c15.w
    dp4 r1.z, r3, r3
    mad r1.y, r1.z, -c15.w, -r1.y
    mul r0.w, r0.w, r1.y
    mul r2.y, r1.x, r0.w
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r1.xyz, r2.x, r1, c22
    mad_pp r1.xyz, r2.y, c17, r1
    dp3 r0.w, -c18, r0
    mul r1.w, r0.w, c9.z
    max r2.x, r0.w, r1.w
    dp3 r2.y, -c20, r0
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r0.xyz, r4, r1, r0
    mul_pp r2.xyz, r3, r0
    mov_pp oC0, r2

// approximately 82 instruction slots used (7 texture, 75 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddBaseNormDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, false, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 3, -2
    def c1, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    mov r0.xy, t6.zwzw
    texld r1, t6, s5
    texld r0, r0, s6
    mad r0.xy, r0, c0.x, c0.y
    mad r0.zw, r1.xyxy, c0.x, c0.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c0.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c0.z, c0.ywzw
    mad r1.xyz, r1, c0.x, c0.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c1.x
    mad r0.xyz, r1, c5.xxyw, r0
    mul r1.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r1.xyzz
    mad r0.xyz, t3, r0.z, r0.xyww
    add r1.xyz, -t0, c12
    nrm r2.xyz, r1
    dp3_sat r0.w, r0, r2
    dp3 r2.x, r2, -c13
    mov r2.w, c13.w
    mad_sat r2.x, r2.x, r2.w, c14.w
    mov r1.w, c12.w
    dp4 r1.x, r1, r1
    mov r1.y, c0.y
    mad r1.x, r1.x, -c12.w, -r1.y
    mul r0.w, r0.w, r1.x
    mul r2.x, r2.x, r0.w
    add r3.xyz, -t0, c15
    nrm r4.xyz, r3
    dp3_sat r0.w, r0, r4
    dp3 r1.x, r4, -c16
    mov r1.w, c16.w
    mad_sat r1.x, r1.x, r1.w, c17.w
    mov r3.w, c15.w
    dp4 r1.z, r3, r3
    mad r1.y, r1.z, -c15.w, -r1.y
    mul r0.w, r0.w, r1.y
    mul r2.y, r1.x, r0.w
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r1.xyz, r2.x, r1, c22
    mad_pp r1.xyz, r2.y, c17, r1
    dp3 r0.w, -c18, r0
    mul r1.w, r0.w, c9.z
    max r2.x, r0.w, r1.w
    dp3 r2.y, -c20, r0
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r0.xyz, r4, r1, r0
    mad_pp r0.xyz, r3, c0.x, r0
    add_pp r2.xyz, r0, c0.y
    mov_pp oC0, r2

// approximately 83 instruction slots used (7 texture, 76 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyBaseNormDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, false, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 3, -2
    def c1, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    mov r0.xy, t6.zwzw
    texld r1, t6, s5
    texld r0, r0, s6
    mad r0.xy, r0, c0.x, c0.y
    mad r0.zw, r1.xyxy, c0.x, c0.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c0.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c0.z, c0.ywzw
    mad r1.xyz, r1, c0.x, c0.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov_pp r0.z, c1.x
    mad r0.xyz, r1, c5.xxyw, r0
    mul r1.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r1.xyzz
    mad r0.xyz, t3, r0.z, r0.xyww
    add r1.xyz, -t0, c12
    nrm r2.xyz, r1
    dp3_sat r0.w, r0, r2
    dp3 r2.x, r2, -c13
    mov r2.w, c13.w
    mad_sat r2.x, r2.x, r2.w, c14.w
    mov r1.w, c12.w
    dp4 r1.x, r1, r1
    mov r1.y, c0.y
    mad r1.x, r1.x, -c12.w, -r1.y
    mul r0.w, r0.w, r1.x
    mul r2.x, r2.x, r0.w
    add r3.xyz, -t0, c15
    nrm r4.xyz, r3
    dp3_sat r0.w, r0, r4
    dp3 r1.x, r4, -c16
    mov r1.w, c16.w
    mad_sat r1.x, r1.x, r1.w, c17.w
    mov r3.w, c15.w
    dp4 r1.z, r3, r3
    mad r1.y, r1.z, -c15.w, -r1.y
    mul r0.w, r0.w, r1.y
    mul r2.y, r1.x, r0.w
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r1.xyz, r2.x, r1, c22
    mad_pp r1.xyz, r2.y, c17, r1
    dp3 r0.w, -c18, r0
    mul r1.w, r0.w, c9.z
    max r2.x, r0.w, r1.w
    dp3 r2.y, -c20, r0
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mul_pp r2.xyz, r2, r3
    mul_pp r1.xyz, r1, r2
    add_pp r1.xyz, r1, r1
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mad_pp r2.xyz, r0, r0.w, r1
    mov_pp oC0, r2

// approximately 83 instruction slots used (7 texture, 76 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyBaseNormDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, false, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 3, -2
    def c1, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    mov r0.xy, t6.zwzw
    texld r1, t6, s5
    texld r0, r0, s6
    mad r0.xy, r0, c0.x, c0.y
    mad r0.zw, r1.xyxy, c0.x, c0.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c0.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c0.z, c0.ywzw
    mad r1.xyz, r1, c0.x, c0.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c1.x
    mad r0.xyz, r1, c5.xxyw, r0
    mul r1.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r1.xyzz
    mad r0.xyz, t3, r0.z, r0.xyww
    add r1.xyz, -t0, c12
    nrm r2.xyz, r1
    dp3_sat r0.w, r0, r2
    dp3 r2.x, r2, -c13
    mov r2.w, c13.w
    mad_sat r2.x, r2.x, r2.w, c14.w
    mov r1.w, c12.w
    dp4 r1.x, r1, r1
    mov r1.y, c0.y
    mad r1.x, r1.x, -c12.w, -r1.y
    mul r0.w, r0.w, r1.x
    mul r2.x, r2.x, r0.w
    add r3.xyz, -t0, c15
    nrm r4.xyz, r3
    dp3_sat r0.w, r0, r4
    dp3 r1.x, r4, -c16
    mov r1.w, c16.w
    mad_sat r1.x, r1.x, r1.w, c17.w
    mov r3.w, c15.w
    dp4 r1.z, r3, r3
    mad r1.y, r1.z, -c15.w, -r1.y
    mul r0.w, r0.w, r1.y
    mul r2.y, r1.x, r0.w
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r1.xyz, r2.x, r1, c22
    mad_pp r1.xyz, r2.y, c17, r1
    dp3 r0.w, -c18, r0
    mul r1.w, r0.w, c9.z
    max r2.x, r0.w, r1.w
    dp3 r2.y, -c20, r0
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mul_pp r3.xyz, r3, r4
    mad_pp r2.xyz, r3, r1, r0
    mov_pp oC0, r2

// approximately 82 instruction slots used (7 texture, 75 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddBaseNormDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, false, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 3, -2
    def c1, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    mov r0.xy, t6.zwzw
    texld r1, t6, s5
    texld r0, r0, s6
    mad r0.xy, r0, c0.x, c0.y
    mad r0.zw, r1.xyxy, c0.x, c0.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c0.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c0.z, c0.ywzw
    mad r1.xyz, r1, c0.x, c0.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c1.x
    mad r0.xyz, r1, c5.xxyw, r0
    mul r1.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r1.xyzz
    mad r0.xyz, t3, r0.z, r0.xyww
    add r1.xyz, -t0, c12
    nrm r2.xyz, r1
    dp3_sat r0.w, r0, r2
    dp3 r2.x, r2, -c13
    mov r2.w, c13.w
    mad_sat r2.x, r2.x, r2.w, c14.w
    mov r1.w, c12.w
    dp4 r1.x, r1, r1
    mov r1.y, c0.y
    mad r1.x, r1.x, -c12.w, -r1.y
    mul r0.w, r0.w, r1.x
    mul r2.x, r2.x, r0.w
    add r3.xyz, -t0, c15
    nrm r4.xyz, r3
    dp3_sat r0.w, r0, r4
    dp3 r1.x, r4, -c16
    mov r1.w, c16.w
    mad_sat r1.x, r1.x, r1.w, c17.w
    mov r3.w, c15.w
    dp4 r1.z, r3, r3
    mad r1.y, r1.z, -c15.w, -r1.y
    mul r0.w, r0.w, r1.y
    mul r2.y, r1.x, r0.w
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r1.xyz, r2.x, r1, c22
    mad_pp r1.xyz, r2.y, c17, r1
    dp3 r0.w, -c18, r0
    mul r1.w, r0.w, c9.z
    max r2.x, r0.w, r1.w
    dp3 r2.y, -c20, r0
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r3.xyz, r3, c0.x, r4
    add_pp r3.xyz, r3, c0.y
    mad_pp r2.xyz, r3, r1, r0
    mov_pp oC0, r2

// approximately 83 instruction slots used (7 texture, 76 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyComplexFogBaseNormDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, true, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 3, -2
    def c4, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    mov r0.xy, t6.zwzw
    texld r1, t6, s5
    texld r0, r0, s6
    mad r0.xy, r0, c3.x, c3.y
    mad r0.zw, r1.xyxy, c3.x, c3.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c3.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c3.z, c3.ywzw
    mad r1.xyz, r1, c3.x, c3.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov_pp r0.z, c4.x
    mad r0.xyz, r1, c5.xxyw, r0
    mul r1.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r1.xyzz
    mad r0.xyz, t3, r0.z, r0.xyww
    add r1.xyz, -t0, c12
    nrm r2.xyz, r1
    dp3_sat r0.w, r0, r2
    dp3 r2.x, r2, -c13
    mov r2.w, c13.w
    mad_sat r2.x, r2.x, r2.w, c14.w
    mov r1.w, c12.w
    dp4 r1.x, r1, r1
    mov r3.xy, c3
    mad r1.x, r1.x, -c12.w, -r3.y
    mul r0.w, r0.w, r1.x
    mul r1.x, r2.x, r0.w
    add r2.xyz, -t0, c15
    nrm r4.xyz, r2
    dp3_sat r0.w, r0, r4
    dp3 r1.z, r4, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mov r2.w, c15.w
    dp4 r1.w, r2, r2
    mad r1.w, r1.w, -c15.w, -r3.y
    mul r0.w, r0.w, r1.w
    mul r1.y, r1.z, r0.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r0.w, -c18, r0
    mul r1.w, r0.w, c9.z
    max r2.x, r0.w, r1.w
    dp3 r2.y, -c20, r0
    max r3.yz, r2.xxyw, t2.z
    min r2.xy, t2.w, r3.yzzw
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, r0
    mul r3.yzw, r0.xxyz, r0.w
    mad_pp r4.xyz, r3.yzww, c8.w, -r2
    nrm r5.xyz, r2
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r4, r4, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r4, t2, s1
    texld_pp r5, t1, s0
    mad_pp r0.xyz, r5, r1, r0
    mul_pp r0.xyz, r4, r0
    mul_pp r0.xyz, r0, c0.w
    add r1.xyz, r0, r0
    mad r0.xyz, r0, -r3.x, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 89 instruction slots used (7 texture, 82 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyComplexFogBaseNormDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, true, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 3, -2
    def c4, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    mov r0.xy, t6.zwzw
    texld r1, t6, s5
    texld r0, r0, s6
    mad r0.xy, r0, c3.x, c3.y
    mad r0.zw, r1.xyxy, c3.x, c3.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c3.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c3.z, c3.ywzw
    mad r1.xyz, r1, c3.x, c3.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c4.x
    mad r0.xyz, r1, c5.xxyw, r0
    mul r1.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r1.xyzz
    mad r0.xyz, t3, r0.z, r0.xyww
    add r1.xyz, -t0, c12
    nrm r2.xyz, r1
    dp3_sat r0.w, r0, r2
    dp3 r2.x, r2, -c13
    mov r2.w, c13.w
    mad_sat r2.x, r2.x, r2.w, c14.w
    mov r1.w, c12.w
    dp4 r1.x, r1, r1
    mov r1.y, c3.y
    mad r1.x, r1.x, -c12.w, -r1.y
    mul r0.w, r0.w, r1.x
    mul r2.x, r2.x, r0.w
    add r3.xyz, -t0, c15
    nrm r4.xyz, r3
    dp3_sat r0.w, r0, r4
    dp3 r1.x, r4, -c16
    mov r1.w, c16.w
    mad_sat r1.x, r1.x, r1.w, c17.w
    mov r3.w, c15.w
    dp4 r1.z, r3, r3
    mad r1.y, r1.z, -c15.w, -r1.y
    mul r0.w, r0.w, r1.y
    mul r2.y, r1.x, r0.w
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r1.xyz, r2.x, r1, c22
    mad_pp r1.xyz, r2.y, c17, r1
    dp3 r0.w, -c18, r0
    mul r1.w, r0.w, c9.z
    max r2.x, r0.w, r1.w
    dp3 r2.y, -c20, r0
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r0.xyz, r4, r1, r0
    mul_pp r0.xyz, r3, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 88 instruction slots used (7 texture, 81 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddComplexFogBaseNormDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, true, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 3, -2
    def c4, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    mov r0.xy, t6.zwzw
    texld r1, t6, s5
    texld r0, r0, s6
    mad r0.xy, r0, c3.x, c3.y
    mad r0.zw, r1.xyxy, c3.x, c3.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c3.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c3.z, c3.ywzw
    mad r1.xyz, r1, c3.x, c3.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c4.x
    mad r0.xyz, r1, c5.xxyw, r0
    mul r1.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r1.xyzz
    mad r0.xyz, t3, r0.z, r0.xyww
    add r1.xyz, -t0, c12
    nrm r2.xyz, r1
    dp3_sat r0.w, r0, r2
    dp3 r2.x, r2, -c13
    mov r2.w, c13.w
    mad_sat r2.x, r2.x, r2.w, c14.w
    mov r1.w, c12.w
    dp4 r1.x, r1, r1
    mov r1.y, c3.y
    mad r1.x, r1.x, -c12.w, -r1.y
    mul r0.w, r0.w, r1.x
    mul r2.x, r2.x, r0.w
    add r3.xyz, -t0, c15
    nrm r4.xyz, r3
    dp3_sat r0.w, r0, r4
    dp3 r1.x, r4, -c16
    mov r1.w, c16.w
    mad_sat r1.x, r1.x, r1.w, c17.w
    mov r3.w, c15.w
    dp4 r1.z, r3, r3
    mad r1.y, r1.z, -c15.w, -r1.y
    mul r0.w, r0.w, r1.y
    mul r2.y, r1.x, r0.w
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r1.xyz, r2.x, r1, c22
    mad_pp r1.xyz, r2.y, c17, r1
    dp3 r0.w, -c18, r0
    mul r1.w, r0.w, c9.z
    max r2.x, r0.w, r1.w
    dp3 r2.y, -c20, r0
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r0.xyz, r4, r1, r0
    mad_pp r0.xyz, r3, c3.x, r0
    add_pp r0.xyz, r0, c3.y
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 89 instruction slots used (7 texture, 82 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyComplexFogBaseNormDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, true, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 3, -2
    def c4, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    mov r0.xy, t6.zwzw
    texld r1, t6, s5
    texld r0, r0, s6
    mad r0.xy, r0, c3.x, c3.y
    mad r0.zw, r1.xyxy, c3.x, c3.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c3.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c3.z, c3.ywzw
    mad r1.xyz, r1, c3.x, c3.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov_pp r0.z, c4.x
    mad r0.xyz, r1, c5.xxyw, r0
    mul r1.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r1.xyzz
    mad r0.xyz, t3, r0.z, r0.xyww
    add r1.xyz, -t0, c12
    nrm r2.xyz, r1
    dp3_sat r0.w, r0, r2
    dp3 r2.x, r2, -c13
    mov r2.w, c13.w
    mad_sat r2.x, r2.x, r2.w, c14.w
    mov r1.w, c12.w
    dp4 r1.x, r1, r1
    mov r1.y, c3.y
    mad r1.x, r1.x, -c12.w, -r1.y
    mul r0.w, r0.w, r1.x
    mul r2.x, r2.x, r0.w
    add r3.xyz, -t0, c15
    nrm r4.xyz, r3
    dp3_sat r0.w, r0, r4
    dp3 r1.x, r4, -c16
    mov r1.w, c16.w
    mad_sat r1.x, r1.x, r1.w, c17.w
    mov r3.w, c15.w
    dp4 r1.z, r3, r3
    mad r1.y, r1.z, -c15.w, -r1.y
    mul r0.w, r0.w, r1.y
    mul r2.y, r1.x, r0.w
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r1.xyz, r2.x, r1, c22
    mad_pp r1.xyz, r2.y, c17, r1
    dp3 r0.w, -c18, r0
    mul r1.w, r0.w, c9.z
    max r2.x, r0.w, r1.w
    dp3 r2.y, -c20, r0
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mul_pp r2.xyz, r2, r3
    mul_pp r1.xyz, r1, r2
    add_pp r1.xyz, r1, r1
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 89 instruction slots used (7 texture, 82 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyComplexFogBaseNormDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, true, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 3, -2
    def c4, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    mov r0.xy, t6.zwzw
    texld r1, t6, s5
    texld r0, r0, s6
    mad r0.xy, r0, c3.x, c3.y
    mad r0.zw, r1.xyxy, c3.x, c3.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c3.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c3.z, c3.ywzw
    mad r1.xyz, r1, c3.x, c3.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c4.x
    mad r0.xyz, r1, c5.xxyw, r0
    mul r1.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r1.xyzz
    mad r0.xyz, t3, r0.z, r0.xyww
    add r1.xyz, -t0, c12
    nrm r2.xyz, r1
    dp3_sat r0.w, r0, r2
    dp3 r2.x, r2, -c13
    mov r2.w, c13.w
    mad_sat r2.x, r2.x, r2.w, c14.w
    mov r1.w, c12.w
    dp4 r1.x, r1, r1
    mov r1.y, c3.y
    mad r1.x, r1.x, -c12.w, -r1.y
    mul r0.w, r0.w, r1.x
    mul r2.x, r2.x, r0.w
    add r3.xyz, -t0, c15
    nrm r4.xyz, r3
    dp3_sat r0.w, r0, r4
    dp3 r1.x, r4, -c16
    mov r1.w, c16.w
    mad_sat r1.x, r1.x, r1.w, c17.w
    mov r3.w, c15.w
    dp4 r1.z, r3, r3
    mad r1.y, r1.z, -c15.w, -r1.y
    mul r0.w, r0.w, r1.y
    mul r2.y, r1.x, r0.w
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r1.xyz, r2.x, r1, c22
    mad_pp r1.xyz, r2.y, c17, r1
    dp3 r0.w, -c18, r0
    mul r1.w, r0.w, c9.z
    max r2.x, r0.w, r1.w
    dp3 r2.y, -c20, r0
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mul_pp r3.xyz, r3, r4
    mad_pp r0.xyz, r3, r1, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 88 instruction slots used (7 texture, 81 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddComplexFogBaseNormDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, true, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 3, -2
    def c4, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    mov r0.xy, t6.zwzw
    texld r1, t6, s5
    texld r0, r0, s6
    mad r0.xy, r0, c3.x, c3.y
    mad r0.zw, r1.xyxy, c3.x, c3.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c3.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c3.z, c3.ywzw
    mad r1.xyz, r1, c3.x, c3.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c4.x
    mad r0.xyz, r1, c5.xxyw, r0
    mul r1.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r1.xyzz
    mad r0.xyz, t3, r0.z, r0.xyww
    add r1.xyz, -t0, c12
    nrm r2.xyz, r1
    dp3_sat r0.w, r0, r2
    dp3 r2.x, r2, -c13
    mov r2.w, c13.w
    mad_sat r2.x, r2.x, r2.w, c14.w
    mov r1.w, c12.w
    dp4 r1.x, r1, r1
    mov r1.y, c3.y
    mad r1.x, r1.x, -c12.w, -r1.y
    mul r0.w, r0.w, r1.x
    mul r2.x, r2.x, r0.w
    add r3.xyz, -t0, c15
    nrm r4.xyz, r3
    dp3_sat r0.w, r0, r4
    dp3 r1.x, r4, -c16
    mov r1.w, c16.w
    mad_sat r1.x, r1.x, r1.w, c17.w
    mov r3.w, c15.w
    dp4 r1.z, r3, r3
    mad r1.y, r1.z, -c15.w, -r1.y
    mul r0.w, r0.w, r1.y
    mul r2.y, r1.x, r0.w
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r1.xyz, r2.x, r1, c22
    mad_pp r1.xyz, r2.y, c17, r1
    dp3 r0.w, -c18, r0
    mul r1.w, r0.w, c9.z
    max r2.x, r0.w, r1.w
    dp3 r2.y, -c20, r0
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r3.xyz, r3, c3.x, r4
    add_pp r3.xyz, r3, c3.y
    mad_pp r0.xyz, r3, r1, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 89 instruction slots used (7 texture, 82 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyBaseNormDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, false, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 3, -2
    def c1, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    dcl_2d s7
    mov r0.xy, t6.zwzw
    texld r1, t6, s5
    texld r0, r0, s6
    mad r0.xy, r0, c0.x, c0.y
    mad r0.zw, r1.xyxy, c0.x, c0.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c0.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c0.z, c0.ywzw
    mad r1.xyz, r1, c0.x, c0.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov_pp r0.z, c1.x
    mad r0.xyz, r1, c5.xxyw, r0
    mul r1.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r1.xyzz
    mad r0.xyz, t3, r0.z, r0.xyww
    add r1.xyz, -t0, c12
    nrm r2.xyz, r1
    dp3_sat r0.w, r0, r2
    dp3 r2.x, r2, -c13
    mov r2.w, c13.w
    mad_sat r2.x, r2.x, r2.w, c14.w
    mov r1.w, c12.w
    dp4 r1.x, r1, r1
    mov r1.y, c0.y
    mad r1.x, r1.x, -c12.w, -r1.y
    mul r0.w, r0.w, r1.x
    mul r2.x, r2.x, r0.w
    add r3.xyz, -t0, c15
    nrm r4.xyz, r3
    dp3_sat r0.w, r0, r4
    dp3 r1.x, r4, -c16
    mov r1.w, c16.w
    mad_sat r1.x, r1.x, r1.w, c17.w
    mov r3.w, c15.w
    dp4 r1.z, r3, r3
    mad r1.y, r1.z, -c15.w, -r1.y
    mul r0.w, r0.w, r1.y
    mul r2.y, r1.x, r0.w
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r1.xyz, r2.x, r1, c22
    mad_pp r1.xyz, r2.y, c17, r1
    dp3 r0.w, -c18, r0
    mul r1.w, r0.w, c9.z
    max r2.x, r0.w, r1.w
    dp3 r2.y, -c20, r0
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    add_pp r2.xyz, r0, r0
    mov_pp oC0, r2

// approximately 93 instruction slots used (8 texture, 85 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyBaseNormDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, false, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 3, -2
    def c1, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    dcl_2d s7
    mov r0.xy, t6.zwzw
    texld r1, t6, s5
    texld r0, r0, s6
    mad r0.xy, r0, c0.x, c0.y
    mad r0.zw, r1.xyxy, c0.x, c0.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c0.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c0.z, c0.ywzw
    mad r1.xyz, r1, c0.x, c0.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c1.x
    mad r0.xyz, r1, c5.xxyw, r0
    mul r1.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r1.xyzz
    mad r0.xyz, t3, r0.z, r0.xyww
    add r1.xyz, -t0, c12
    nrm r2.xyz, r1
    dp3_sat r0.w, r0, r2
    dp3 r2.x, r2, -c13
    mov r2.w, c13.w
    mad_sat r2.x, r2.x, r2.w, c14.w
    mov r1.w, c12.w
    dp4 r1.x, r1, r1
    mov r1.y, c0.y
    mad r1.x, r1.x, -c12.w, -r1.y
    mul r0.w, r0.w, r1.x
    mul r2.x, r2.x, r0.w
    add r3.xyz, -t0, c15
    nrm r4.xyz, r3
    dp3_sat r0.w, r0, r4
    dp3 r1.x, r4, -c16
    mov r1.w, c16.w
    mad_sat r1.x, r1.x, r1.w, c17.w
    mov r3.w, c15.w
    dp4 r1.z, r3, r3
    mad r1.y, r1.z, -c15.w, -r1.y
    mul r0.w, r0.w, r1.y
    mul r2.y, r1.x, r0.w
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r1.xyz, r2.x, r1, c22
    mad_pp r1.xyz, r2.y, c17, r1
    dp3 r0.w, -c18, r0
    mul r1.w, r0.w, c9.z
    max r2.x, r0.w, r1.w
    dp3 r2.y, -c20, r0
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    texld_pp r1, t2, s1
    mul_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 92 instruction slots used (8 texture, 84 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddBaseNormDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, false, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 3, -2
    def c1, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    dcl_2d s7
    mov r0.xy, t6.zwzw
    texld r1, t6, s5
    texld r0, r0, s6
    mad r0.xy, r0, c0.x, c0.y
    mad r0.zw, r1.xyxy, c0.x, c0.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c0.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c0.z, c0.ywzw
    mad r1.xyz, r1, c0.x, c0.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c1.x
    mad r0.xyz, r1, c5.xxyw, r0
    mul r1.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r1.xyzz
    mad r0.xyz, t3, r0.z, r0.xyww
    add r1.xyz, -t0, c12
    nrm r2.xyz, r1
    dp3_sat r0.w, r0, r2
    dp3 r2.x, r2, -c13
    mov r2.w, c13.w
    mad_sat r2.x, r2.x, r2.w, c14.w
    mov r1.w, c12.w
    dp4 r1.x, r1, r1
    mov r1.y, c0.y
    mad r1.x, r1.x, -c12.w, -r1.y
    mul r0.w, r0.w, r1.x
    mul r2.x, r2.x, r0.w
    add r3.xyz, -t0, c15
    nrm r4.xyz, r3
    dp3_sat r0.w, r0, r4
    dp3 r1.x, r4, -c16
    mov r1.w, c16.w
    mad_sat r1.x, r1.x, r1.w, c17.w
    mov r3.w, c15.w
    dp4 r1.z, r3, r3
    mad r1.y, r1.z, -c15.w, -r1.y
    mul r0.w, r0.w, r1.y
    mul r2.y, r1.x, r0.w
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r1.xyz, r2.x, r1, c22
    mad_pp r1.xyz, r2.y, c17, r1
    dp3 r0.w, -c18, r0
    mul r1.w, r0.w, c9.z
    max r2.x, r0.w, r1.w
    dp3 r2.y, -c20, r0
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c0.x, r0
    add_pp r2.xyz, r0, c0.y
    mov_pp oC0, r2

// approximately 93 instruction slots used (8 texture, 85 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyBaseNormDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, false, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 3, -2
    def c1, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    dcl_2d s7
    mov r0.xy, t6.zwzw
    texld r1, t6, s5
    texld r0, r0, s6
    mad r0.xy, r0, c0.x, c0.y
    mad r0.zw, r1.xyxy, c0.x, c0.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c0.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c0.z, c0.ywzw
    mad r1.xyz, r1, c0.x, c0.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov_pp r0.z, c1.x
    mad r0.xyz, r1, c5.xxyw, r0
    mul r1.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r1.xyzz
    mad r0.xyz, t3, r0.z, r0.xyww
    add r1.xyz, -t0, c12
    nrm r2.xyz, r1
    dp3_sat r0.w, r0, r2
    dp3 r2.x, r2, -c13
    mov r2.w, c13.w
    mad_sat r2.x, r2.x, r2.w, c14.w
    mov r1.w, c12.w
    dp4 r1.x, r1, r1
    mov r1.y, c0.y
    mad r1.x, r1.x, -c12.w, -r1.y
    mul r0.w, r0.w, r1.x
    mul r2.x, r2.x, r0.w
    add r3.xyz, -t0, c15
    nrm r4.xyz, r3
    dp3_sat r0.w, r0, r4
    dp3 r1.x, r4, -c16
    mov r1.w, c16.w
    mad_sat r1.x, r1.x, r1.w, c17.w
    mov r3.w, c15.w
    dp4 r1.z, r3, r3
    mad r1.y, r1.z, -c15.w, -r1.y
    mul r0.w, r0.w, r1.y
    mul r2.y, r1.x, r0.w
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r1.xyz, r2.x, r1, c22
    mad_pp r1.xyz, r2.y, c17, r1
    dp3 r0.w, -c18, r0
    mul r1.w, r0.w, c9.z
    max r2.x, r0.w, r1.w
    dp3 r2.y, -c20, r0
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mul_pp r2.xyz, r2, r3
    mul_pp r1.xyz, r1, r2
    add_pp r1.xyz, r1, r1
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mad_pp r2.xyz, r0, r0.w, r1
    mov_pp oC0, r2

// approximately 93 instruction slots used (8 texture, 85 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyBaseNormDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, false, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 3, -2
    def c1, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    dcl_2d s7
    mov r0.xy, t6.zwzw
    texld r1, t6, s5
    texld r0, r0, s6
    mad r0.xy, r0, c0.x, c0.y
    mad r0.zw, r1.xyxy, c0.x, c0.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c0.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c0.z, c0.ywzw
    mad r1.xyz, r1, c0.x, c0.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c1.x
    mad r0.xyz, r1, c5.xxyw, r0
    mul r1.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r1.xyzz
    mad r0.xyz, t3, r0.z, r0.xyww
    add r1.xyz, -t0, c12
    nrm r2.xyz, r1
    dp3_sat r0.w, r0, r2
    dp3 r2.x, r2, -c13
    mov r2.w, c13.w
    mad_sat r2.x, r2.x, r2.w, c14.w
    mov r1.w, c12.w
    dp4 r1.x, r1, r1
    mov r1.y, c0.y
    mad r1.x, r1.x, -c12.w, -r1.y
    mul r0.w, r0.w, r1.x
    mul r2.x, r2.x, r0.w
    add r3.xyz, -t0, c15
    nrm r4.xyz, r3
    dp3_sat r0.w, r0, r4
    dp3 r1.x, r4, -c16
    mov r1.w, c16.w
    mad_sat r1.x, r1.x, r1.w, c17.w
    mov r3.w, c15.w
    dp4 r1.z, r3, r3
    mad r1.y, r1.z, -c15.w, -r1.y
    mul r0.w, r0.w, r1.y
    mul r2.y, r1.x, r0.w
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r1.xyz, r2.x, r1, c22
    mad_pp r1.xyz, r2.y, c17, r1
    dp3 r0.w, -c18, r0
    mul r1.w, r0.w, c9.z
    max r2.x, r0.w, r1.w
    dp3 r2.y, -c20, r0
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mul_pp r3.xyz, r3, r4
    mad_pp r2.xyz, r3, r1, r0
    mov_pp oC0, r2

// approximately 92 instruction slots used (8 texture, 84 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddBaseNormDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, false, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 3, -2
    def c1, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    dcl_2d s7
    mov r0.xy, t6.zwzw
    texld r1, t6, s5
    texld r0, r0, s6
    mad r0.xy, r0, c0.x, c0.y
    mad r0.zw, r1.xyxy, c0.x, c0.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c0.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c0.z, c0.ywzw
    mad r1.xyz, r1, c0.x, c0.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c1.x
    mad r0.xyz, r1, c5.xxyw, r0
    mul r1.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r1.xyzz
    mad r0.xyz, t3, r0.z, r0.xyww
    add r1.xyz, -t0, c12
    nrm r2.xyz, r1
    dp3_sat r0.w, r0, r2
    dp3 r2.x, r2, -c13
    mov r2.w, c13.w
    mad_sat r2.x, r2.x, r2.w, c14.w
    mov r1.w, c12.w
    dp4 r1.x, r1, r1
    mov r1.y, c0.y
    mad r1.x, r1.x, -c12.w, -r1.y
    mul r0.w, r0.w, r1.x
    mul r2.x, r2.x, r0.w
    add r3.xyz, -t0, c15
    nrm r4.xyz, r3
    dp3_sat r0.w, r0, r4
    dp3 r1.x, r4, -c16
    mov r1.w, c16.w
    mad_sat r1.x, r1.x, r1.w, c17.w
    mov r3.w, c15.w
    dp4 r1.z, r3, r3
    mad r1.y, r1.z, -c15.w, -r1.y
    mul r0.w, r0.w, r1.y
    mul r2.y, r1.x, r0.w
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r1.xyz, r2.x, r1, c22
    mad_pp r1.xyz, r2.y, c17, r1
    dp3 r0.w, -c18, r0
    mul r1.w, r0.w, c9.z
    max r2.x, r0.w, r1.w
    dp3 r2.y, -c20, r0
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r3.xyz, r3, c0.x, r4
    add_pp r3.xyz, r3, c0.y
    mad_pp r2.xyz, r3, r1, r0
    mov_pp oC0, r2

// approximately 93 instruction slots used (8 texture, 85 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyComplexFogBaseNormDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, true, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 3, -2
    def c4, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    dcl_2d s7
    mov r0.xy, t6.zwzw
    texld r1, t6, s5
    texld r0, r0, s6
    mad r0.xy, r0, c3.x, c3.y
    mad r0.zw, r1.xyxy, c3.x, c3.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c3.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c3.z, c3.ywzw
    mad r1.xyz, r1, c3.x, c3.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov_pp r0.z, c4.x
    mad r0.xyz, r1, c5.xxyw, r0
    mul r1.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r1.xyzz
    mad r0.xyz, t3, r0.z, r0.xyww
    add r1.xyz, -t0, c12
    nrm r2.xyz, r1
    dp3_sat r0.w, r0, r2
    dp3 r2.x, r2, -c13
    mov r2.w, c13.w
    mad_sat r2.x, r2.x, r2.w, c14.w
    mov r1.w, c12.w
    dp4 r1.x, r1, r1
    mov r3.xy, c3
    mad r1.x, r1.x, -c12.w, -r3.y
    mul r0.w, r0.w, r1.x
    mul r1.x, r2.x, r0.w
    add r2.xyz, -t0, c15
    nrm r4.xyz, r2
    dp3_sat r0.w, r0, r4
    dp3 r1.z, r4, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mov r2.w, c15.w
    dp4 r1.w, r2, r2
    mad r1.w, r1.w, -c15.w, -r3.y
    mul r0.w, r0.w, r1.w
    mul r1.y, r1.z, r0.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r0.w, -c18, r0
    mul r1.w, r0.w, c9.z
    max r2.x, r0.w, r1.w
    dp3 r2.y, -c20, r0
    max r3.yz, r2.xxyw, t2.z
    min r2.xy, t2.w, r3.yzzw
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, r0
    mul r3.yzw, r0.xxyz, r0.w
    mad_pp r4.xyz, r3.yzww, c8.w, -r2
    nrm r5.xyz, r2
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r4, r4, s3
    log r5.x, r2.x
    log r5.y, r2.y
    log r5.z, r2.z
    mul r2.xyz, r5, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.z
    mul_sat_pp r2.xyz, r5, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r4
    mov r2.xy, t1.zwzw
    texld_pp r4, t1, s0
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r4, r1, r0
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    mul_pp r0.xyz, r0, c0.w
    add r1.xyz, r0, r0
    mad r0.xyz, r0, -r3.x, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 99 instruction slots used (8 texture, 91 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyComplexFogBaseNormDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, true, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 3, -2
    def c4, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    dcl_2d s7
    mov r0.xy, t6.zwzw
    texld r1, t6, s5
    texld r0, r0, s6
    mad r0.xy, r0, c3.x, c3.y
    mad r0.zw, r1.xyxy, c3.x, c3.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c3.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c3.z, c3.ywzw
    mad r1.xyz, r1, c3.x, c3.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c4.x
    mad r0.xyz, r1, c5.xxyw, r0
    mul r1.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r1.xyzz
    mad r0.xyz, t3, r0.z, r0.xyww
    add r1.xyz, -t0, c12
    nrm r2.xyz, r1
    dp3_sat r0.w, r0, r2
    dp3 r2.x, r2, -c13
    mov r2.w, c13.w
    mad_sat r2.x, r2.x, r2.w, c14.w
    mov r1.w, c12.w
    dp4 r1.x, r1, r1
    mov r1.y, c3.y
    mad r1.x, r1.x, -c12.w, -r1.y
    mul r0.w, r0.w, r1.x
    mul r2.x, r2.x, r0.w
    add r3.xyz, -t0, c15
    nrm r4.xyz, r3
    dp3_sat r0.w, r0, r4
    dp3 r1.x, r4, -c16
    mov r1.w, c16.w
    mad_sat r1.x, r1.x, r1.w, c17.w
    mov r3.w, c15.w
    dp4 r1.z, r3, r3
    mad r1.y, r1.z, -c15.w, -r1.y
    mul r0.w, r0.w, r1.y
    mul r2.y, r1.x, r0.w
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r1.xyz, r2.x, r1, c22
    mad_pp r1.xyz, r2.y, c17, r1
    dp3 r0.w, -c18, r0
    mul r1.w, r0.w, c9.z
    max r2.x, r0.w, r1.w
    dp3 r2.y, -c20, r0
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 98 instruction slots used (8 texture, 90 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddComplexFogBaseNormDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, true, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 3, -2
    def c4, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    dcl_2d s7
    mov r0.xy, t6.zwzw
    texld r1, t6, s5
    texld r0, r0, s6
    mad r0.xy, r0, c3.x, c3.y
    mad r0.zw, r1.xyxy, c3.x, c3.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c3.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c3.z, c3.ywzw
    mad r1.xyz, r1, c3.x, c3.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c4.x
    mad r0.xyz, r1, c5.xxyw, r0
    mul r1.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r1.xyzz
    mad r0.xyz, t3, r0.z, r0.xyww
    add r1.xyz, -t0, c12
    nrm r2.xyz, r1
    dp3_sat r0.w, r0, r2
    dp3 r2.x, r2, -c13
    mov r2.w, c13.w
    mad_sat r2.x, r2.x, r2.w, c14.w
    mov r1.w, c12.w
    dp4 r1.x, r1, r1
    mov r1.y, c3.y
    mad r1.x, r1.x, -c12.w, -r1.y
    mul r0.w, r0.w, r1.x
    mul r2.x, r2.x, r0.w
    add r3.xyz, -t0, c15
    nrm r4.xyz, r3
    dp3_sat r0.w, r0, r4
    dp3 r1.x, r4, -c16
    mov r1.w, c16.w
    mad_sat r1.x, r1.x, r1.w, c17.w
    mov r3.w, c15.w
    dp4 r1.z, r3, r3
    mad r1.y, r1.z, -c15.w, -r1.y
    mul r0.w, r0.w, r1.y
    mul r2.y, r1.x, r0.w
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r1.xyz, r2.x, r1, c22
    mad_pp r1.xyz, r2.y, c17, r1
    dp3 r0.w, -c18, r0
    mul r1.w, r0.w, c9.z
    max r2.x, r0.w, r1.w
    dp3 r2.y, -c20, r0
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c3.x, r0
    add_pp r0.xyz, r0, c3.y
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 99 instruction slots used (8 texture, 91 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyComplexFogBaseNormDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, true, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 3, -2
    def c4, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    dcl_2d s7
    mov r0.xy, t6.zwzw
    texld r1, t6, s5
    texld r0, r0, s6
    mad r0.xy, r0, c3.x, c3.y
    mad r0.zw, r1.xyxy, c3.x, c3.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c3.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c3.z, c3.ywzw
    mad r1.xyz, r1, c3.x, c3.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov_pp r0.z, c4.x
    mad r0.xyz, r1, c5.xxyw, r0
    mul r1.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r1.xyzz
    mad r0.xyz, t3, r0.z, r0.xyww
    add r1.xyz, -t0, c12
    nrm r2.xyz, r1
    dp3_sat r0.w, r0, r2
    dp3 r2.x, r2, -c13
    mov r2.w, c13.w
    mad_sat r2.x, r2.x, r2.w, c14.w
    mov r1.w, c12.w
    dp4 r1.x, r1, r1
    mov r1.y, c3.y
    mad r1.x, r1.x, -c12.w, -r1.y
    mul r0.w, r0.w, r1.x
    mul r2.x, r2.x, r0.w
    add r3.xyz, -t0, c15
    nrm r4.xyz, r3
    dp3_sat r0.w, r0, r4
    dp3 r1.x, r4, -c16
    mov r1.w, c16.w
    mad_sat r1.x, r1.x, r1.w, c17.w
    mov r3.w, c15.w
    dp4 r1.z, r3, r3
    mad r1.y, r1.z, -c15.w, -r1.y
    mul r0.w, r0.w, r1.y
    mul r2.y, r1.x, r0.w
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r1.xyz, r2.x, r1, c22
    mad_pp r1.xyz, r2.y, c17, r1
    dp3 r0.w, -c18, r0
    mul r1.w, r0.w, c9.z
    max r2.x, r0.w, r1.w
    dp3 r2.y, -c20, r0
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mul_pp r2.xyz, r2, r3
    mul_pp r1.xyz, r1, r2
    add_pp r1.xyz, r1, r1
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 99 instruction slots used (8 texture, 91 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyComplexFogBaseNormDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, true, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 3, -2
    def c4, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    dcl_2d s7
    mov r0.xy, t6.zwzw
    texld r1, t6, s5
    texld r0, r0, s6
    mad r0.xy, r0, c3.x, c3.y
    mad r0.zw, r1.xyxy, c3.x, c3.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c3.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c3.z, c3.ywzw
    mad r1.xyz, r1, c3.x, c3.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c4.x
    mad r0.xyz, r1, c5.xxyw, r0
    mul r1.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r1.xyzz
    mad r0.xyz, t3, r0.z, r0.xyww
    add r1.xyz, -t0, c12
    nrm r2.xyz, r1
    dp3_sat r0.w, r0, r2
    dp3 r2.x, r2, -c13
    mov r2.w, c13.w
    mad_sat r2.x, r2.x, r2.w, c14.w
    mov r1.w, c12.w
    dp4 r1.x, r1, r1
    mov r1.y, c3.y
    mad r1.x, r1.x, -c12.w, -r1.y
    mul r0.w, r0.w, r1.x
    mul r2.x, r2.x, r0.w
    add r3.xyz, -t0, c15
    nrm r4.xyz, r3
    dp3_sat r0.w, r0, r4
    dp3 r1.x, r4, -c16
    mov r1.w, c16.w
    mad_sat r1.x, r1.x, r1.w, c17.w
    mov r3.w, c15.w
    dp4 r1.z, r3, r3
    mad r1.y, r1.z, -c15.w, -r1.y
    mul r0.w, r0.w, r1.y
    mul r2.y, r1.x, r0.w
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r1.xyz, r2.x, r1, c22
    mad_pp r1.xyz, r2.y, c17, r1
    dp3 r0.w, -c18, r0
    mul r1.w, r0.w, c9.z
    max r2.x, r0.w, r1.w
    dp3 r2.y, -c20, r0
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mul_pp r3.xyz, r3, r4
    mad_pp r0.xyz, r3, r1, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 98 instruction slots used (8 texture, 90 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddComplexFogBaseNormDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, true, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 3, -2
    def c4, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    dcl_2d s7
    mov r0.xy, t6.zwzw
    texld r1, t6, s5
    texld r0, r0, s6
    mad r0.xy, r0, c3.x, c3.y
    mad r0.zw, r1.xyxy, c3.x, c3.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c3.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c3.z, c3.ywzw
    mad r1.xyz, r1, c3.x, c3.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c4.x
    mad r0.xyz, r1, c5.xxyw, r0
    mul r1.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r1.xyzz
    mad r0.xyz, t3, r0.z, r0.xyww
    add r1.xyz, -t0, c12
    nrm r2.xyz, r1
    dp3_sat r0.w, r0, r2
    dp3 r2.x, r2, -c13
    mov r2.w, c13.w
    mad_sat r2.x, r2.x, r2.w, c14.w
    mov r1.w, c12.w
    dp4 r1.x, r1, r1
    mov r1.y, c3.y
    mad r1.x, r1.x, -c12.w, -r1.y
    mul r0.w, r0.w, r1.x
    mul r2.x, r2.x, r0.w
    add r3.xyz, -t0, c15
    nrm r4.xyz, r3
    dp3_sat r0.w, r0, r4
    dp3 r1.x, r4, -c16
    mov r1.w, c16.w
    mad_sat r1.x, r1.x, r1.w, c17.w
    mov r3.w, c15.w
    dp4 r1.z, r3, r3
    mad r1.y, r1.z, -c15.w, -r1.y
    mul r0.w, r0.w, r1.y
    mul r2.y, r1.x, r0.w
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r1.xyz, r2.x, r1, c22
    mad_pp r1.xyz, r2.y, c17, r1
    dp3 r0.w, -c18, r0
    mul r1.w, r0.w, c9.z
    max r2.x, r0.w, r1.w
    dp3 r2.y, -c20, r0
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r3.xyz, r3, c3.x, r4
    add_pp r3.xyz, r3, c3.y
    mad_pp r0.xyz, r3, r1, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 99 instruction slots used (8 texture, 91 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyBaseNormDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, false, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 3, -2
    def c1, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c0.x, c0.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c0.x, c0.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c0.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c0.z, c0.xyyw
    mad r3.xyz, r3, c0.x, c0.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c1.x
    mad r0.yzw, r3.xxyz, c5.xxxy, r2.xxyz
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    mul r0.x, r0.x, r2.x
    dp3 r2.x, r1, -c13
    mov r3, c14
    mad_sat r3.w, r2.x, c13.w, r3.w
    mul r2.x, r0.x, r3.w
    mov r4.w, c15.w
    add r4.xyz, -t0, c15
    dp4 r3.w, r4, r4
    nrm r5.xyz, r4
    mad r1.w, r3.w, -c15.w, -r1.w
    dp3_sat r3.w, r0.yzww, r5
    mul r1.w, r1.w, r3.w
    dp3 r3.w, r5, -c16
    mov r5.w, c17.w
    mad_sat r3.w, r3.w, c16.w, r5.w
    mul r2.y, r1.w, r3.w
    max r4.xy, r2, t2.z
    min r2.xy, t2.w, r4
    dp3 r1.w, -r5, r0.yzww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.yzww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c1.x
    pow r3.w, r4.w, c7.z
    mul r3.w, r3.w, c7.w
    mul r1.xyz, r3.w, c14
    mad_pp r1.xyz, r1, r2.x, r4
    dp3 r1.w, -c18, r0.yzww
    mul r3.w, r1.w, c9.z
    max r4.x, r1.w, r3.w
    dp3 r4.y, -c20, r0.yzww
    max r2.zw, r4.xyxy, t2.z
    min r4.xy, t2.w, r2.zwzw
    dp3 r1.w, c18, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c18
    dp3 r1.w, r7, r6
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c19
    mad_pp r1.xyz, r7, r4.x, r1
    dp3 r1.w, c20, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c20
    dp3 r1.w, r7, r6
    dp3 r3.w, r0.yzww, r6
    mov r6, c10
    mad_pp r6, r3.w, r6, c11
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mul_sat_pp r1.xyz, r6, r1
    mov r7.xy, t1.zwzw
    texld_pp r8, t1, s0
    texld_pp r7, r7, s2
    mul_pp r1.xyz, r1, r7.z
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r2, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mul_pp r0.w, r6.w, r7.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul_pp r0.xyz, r2, r0
    add_pp r7.xyz, r0, r0
    mov_pp oC0, r7

// approximately 129 instruction slots used (7 texture, 122 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyBaseNormDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, false, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 3, -2
    def c1, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c0.x, c0.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c0.x, c0.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c0.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c0.z, c0.xyyw
    mad r3.xyz, r3, c0.x, c0.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov_pp r2.z, c1.x
    mad r0.yzw, r3.xxyz, c5.xxxy, r2.xxyz
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    mul r0.x, r0.x, r2.x
    dp3 r2.x, r1, -c13
    mov r3, c14
    mad_sat r3.w, r2.x, c13.w, r3.w
    mul r2.x, r0.x, r3.w
    mov r4.w, c15.w
    add r4.xyz, -t0, c15
    dp4 r3.w, r4, r4
    nrm r5.xyz, r4
    mad r1.w, r3.w, -c15.w, -r1.w
    dp3_sat r3.w, r0.yzww, r5
    mul r1.w, r1.w, r3.w
    dp3 r3.w, r5, -c16
    mov r5.w, c17.w
    mad_sat r3.w, r3.w, c16.w, r5.w
    mul r2.y, r1.w, r3.w
    max r4.xy, r2, t2.z
    min r2.xy, t2.w, r4
    dp3 r1.w, -r5, r0.yzww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.yzww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c1.x
    pow r3.w, r4.w, c7.z
    mul r3.w, r3.w, c7.w
    mul r1.xyz, r3.w, c14
    mad_pp r1.xyz, r1, r2.x, r4
    dp3 r1.w, -c18, r0.yzww
    mul r3.w, r1.w, c9.z
    max r4.x, r1.w, r3.w
    dp3 r4.y, -c20, r0.yzww
    max r2.zw, r4.xyxy, t2.z
    min r4.xy, t2.w, r2.zwzw
    dp3 r1.w, c18, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c18
    dp3 r1.w, r7, r6
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c19
    mad_pp r1.xyz, r7, r4.x, r1
    dp3 r1.w, c20, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c20
    dp3 r1.w, r7, r6
    dp3 r3.w, r0.yzww, r6
    mov r6, c10
    mad_pp r6, r3.w, r6, c11
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mul_sat_pp r1.xyz, r6, r1
    mov r7.xy, t1.zwzw
    texld_pp r8, t1, s0
    texld_pp r7, r7, s2
    mul_pp r1.xyz, r1, r7.z
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r2, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mul_pp r0.w, r6.w, r7.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul_pp r7.xyz, r2, r0
    mov_pp oC0, r7

// approximately 128 instruction slots used (7 texture, 121 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddBaseNormDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, false, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 3, -2
    def c1, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c0.x, c0.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c0.x, c0.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c0.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c0.z, c0.xyyw
    mad r3.xyz, r3, c0.x, c0.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c1.x
    mad r0.yzw, r3.xxyz, c5.xxxy, r2.xxyz
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    mul r0.x, r0.x, r2.x
    dp3 r2.x, r1, -c13
    mov r3, c14
    mad_sat r3.w, r2.x, c13.w, r3.w
    mul r2.x, r0.x, r3.w
    mov r4.w, c15.w
    add r4.xyz, -t0, c15
    dp4 r3.w, r4, r4
    nrm r5.xyz, r4
    mad r1.w, r3.w, -c15.w, -r1.w
    dp3_sat r3.w, r0.yzww, r5
    mul r1.w, r1.w, r3.w
    dp3 r3.w, r5, -c16
    mov r5.w, c17.w
    mad_sat r3.w, r3.w, c16.w, r5.w
    mul r2.y, r1.w, r3.w
    max r4.xy, r2, t2.z
    min r2.xy, t2.w, r4
    dp3 r1.w, -r5, r0.yzww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.yzww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c1.x
    pow r3.w, r4.w, c7.z
    mul r3.w, r3.w, c7.w
    mul r1.xyz, r3.w, c14
    mad_pp r1.xyz, r1, r2.x, r4
    dp3 r1.w, -c18, r0.yzww
    mul r3.w, r1.w, c9.z
    max r4.x, r1.w, r3.w
    dp3 r4.y, -c20, r0.yzww
    max r2.zw, r4.xyxy, t2.z
    min r4.xy, t2.w, r2.zwzw
    dp3 r1.w, c18, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c18
    dp3 r1.w, r7, r6
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c19
    mad_pp r1.xyz, r7, r4.x, r1
    dp3 r1.w, c20, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c20
    dp3 r1.w, r7, r6
    dp3 r3.w, r0.yzww, r6
    mov r6, c10
    mad_pp r6, r3.w, r6, c11
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mul_sat_pp r1.xyz, r6, r1
    mov r7.xy, t1.zwzw
    texld_pp r8, t1, s0
    texld_pp r7, r7, s2
    mul_pp r1.xyz, r1, r7.z
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r2, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mul_pp r0.w, r6.w, r7.z
    mad_pp r0.xyz, r0, r0.w, r1
    mad_pp r0.xyz, r2, c0.x, r0
    add_pp r7.xyz, r0, c0.y
    mov_pp oC0, r7

// approximately 129 instruction slots used (7 texture, 122 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyBaseNormDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, false, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 3, -2
    def c1, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c0.x, c0.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c0.x, c0.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c0.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c0.z, c0.xyyw
    mad r3.xyz, r3, c0.x, c0.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c1.x
    mad r0.yzw, r3.xxyz, c5.xxxy, r2.xxyz
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    mul r0.x, r0.x, r2.x
    dp3 r2.x, r1, -c13
    mov r3, c14
    mad_sat r3.w, r2.x, c13.w, r3.w
    mul r2.x, r0.x, r3.w
    mov r4.w, c15.w
    add r4.xyz, -t0, c15
    dp4 r3.w, r4, r4
    nrm r5.xyz, r4
    mad r1.w, r3.w, -c15.w, -r1.w
    dp3_sat r3.w, r0.yzww, r5
    mul r1.w, r1.w, r3.w
    dp3 r3.w, r5, -c16
    mov r5.w, c17.w
    mad_sat r3.w, r3.w, c16.w, r5.w
    mul r2.y, r1.w, r3.w
    max r4.xy, r2, t2.z
    min r2.xy, t2.w, r4
    dp3 r1.w, -r5, r0.yzww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.yzww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c1.x
    pow r3.w, r4.w, c7.z
    mul r3.w, r3.w, c7.w
    mul r1.xyz, r3.w, c14
    mad_pp r1.xyz, r1, r2.x, r4
    dp3 r1.w, -c18, r0.yzww
    mul r3.w, r1.w, c9.z
    max r4.x, r1.w, r3.w
    dp3 r4.y, -c20, r0.yzww
    max r2.zw, r4.xyxy, t2.z
    min r4.xy, t2.w, r2.zwzw
    dp3 r1.w, c18, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c18
    dp3 r1.w, r7, r6
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c19
    mad_pp r1.xyz, r7, r4.x, r1
    dp3 r1.w, c20, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c20
    dp3 r1.w, r7, r6
    dp3 r3.w, r0.yzww, r6
    mov r6, c10
    mad_pp r6, r3.w, r6, c11
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mul_sat_pp r1.xyz, r6, r1
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mul_pp r3.xyz, r3, r4
    mul_pp r2.xyz, r2, r3
    add_pp r2.xyz, r2, r2
    mov r3.xy, t1.zwzw
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, r3, s2
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r1, r3.z, r2
    mul_pp r0.xyz, r6, r0
    mul_pp r0.w, r6.w, r3.z
    mad_pp r3.xyz, r0, r0.w, r1
    mov_pp oC0, r3

// approximately 129 instruction slots used (7 texture, 122 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyBaseNormDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, false, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 3, -2
    def c1, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c0.x, c0.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c0.x, c0.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c0.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c0.z, c0.xyyw
    mad r3.xyz, r3, c0.x, c0.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov_pp r2.z, c1.x
    mad r0.yzw, r3.xxyz, c5.xxxy, r2.xxyz
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    mul r0.x, r0.x, r2.x
    dp3 r2.x, r1, -c13
    mov r3, c14
    mad_sat r3.w, r2.x, c13.w, r3.w
    mul r2.x, r0.x, r3.w
    mov r4.w, c15.w
    add r4.xyz, -t0, c15
    dp4 r3.w, r4, r4
    nrm r5.xyz, r4
    mad r1.w, r3.w, -c15.w, -r1.w
    dp3_sat r3.w, r0.yzww, r5
    mul r1.w, r1.w, r3.w
    dp3 r3.w, r5, -c16
    mov r5.w, c17.w
    mad_sat r3.w, r3.w, c16.w, r5.w
    mul r2.y, r1.w, r3.w
    max r4.xy, r2, t2.z
    min r2.xy, t2.w, r4
    dp3 r1.w, -r5, r0.yzww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.yzww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c1.x
    pow r3.w, r4.w, c7.z
    mul r3.w, r3.w, c7.w
    mul r1.xyz, r3.w, c14
    mad_pp r1.xyz, r1, r2.x, r4
    dp3 r1.w, -c18, r0.yzww
    mul r3.w, r1.w, c9.z
    max r4.x, r1.w, r3.w
    dp3 r4.y, -c20, r0.yzww
    max r2.zw, r4.xyxy, t2.z
    min r4.xy, t2.w, r2.zwzw
    dp3 r1.w, c18, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c18
    dp3 r1.w, r7, r6
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c19
    mad_pp r1.xyz, r7, r4.x, r1
    dp3 r1.w, c20, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c20
    dp3 r1.w, r7, r6
    dp3 r3.w, r0.yzww, r6
    mov r6, c10
    mad_pp r6, r3.w, r6, c11
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mul_sat_pp r1.xyz, r6, r1
    mov r7.xy, t1.zwzw
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r5, r7, s2
    texld_pp r0, r0, s3
    mul_pp r1.xyz, r1, r5.z
    texld_pp r7, t2, s1
    texld_pp r8, t1, s0
    mul_pp r7.xyz, r7, r8
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    mad_pp r1.xyz, r7, r2, r1
    mul_pp r0.xyz, r6, r0
    mul_pp r0.w, r6.w, r5.z
    mad_pp r5.xyz, r0, r0.w, r1
    mov_pp oC0, r5

// approximately 128 instruction slots used (7 texture, 121 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddBaseNormDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, false, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 3, -2
    def c1, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c0.x, c0.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c0.x, c0.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c0.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c0.z, c0.xyyw
    mad r3.xyz, r3, c0.x, c0.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c1.x
    mad r0.yzw, r3.xxyz, c5.xxxy, r2.xxyz
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    mul r0.x, r0.x, r2.x
    dp3 r2.x, r1, -c13
    mov r3, c14
    mad_sat r3.w, r2.x, c13.w, r3.w
    mul r2.x, r0.x, r3.w
    mov r4.w, c15.w
    add r4.xyz, -t0, c15
    dp4 r3.w, r4, r4
    nrm r5.xyz, r4
    mad r1.w, r3.w, -c15.w, -r1.w
    dp3_sat r3.w, r0.yzww, r5
    mul r1.w, r1.w, r3.w
    dp3 r3.w, r5, -c16
    mov r5.w, c17.w
    mad_sat r3.w, r3.w, c16.w, r5.w
    mul r2.y, r1.w, r3.w
    max r4.xy, r2, t2.z
    min r2.xy, t2.w, r4
    dp3 r1.w, -r5, r0.yzww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.yzww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c1.x
    pow r3.w, r4.w, c7.z
    mul r3.w, r3.w, c7.w
    mul r1.xyz, r3.w, c14
    mad_pp r1.xyz, r1, r2.x, r4
    dp3 r1.w, -c18, r0.yzww
    mul r3.w, r1.w, c9.z
    max r4.x, r1.w, r3.w
    dp3 r4.y, -c20, r0.yzww
    max r2.zw, r4.xyxy, t2.z
    min r4.xy, t2.w, r2.zwzw
    dp3 r1.w, c18, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c18
    dp3 r1.w, r7, r6
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c19
    mad_pp r1.xyz, r7, r4.x, r1
    dp3 r1.w, c20, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c20
    dp3 r1.w, r7, r6
    dp3 r3.w, r0.yzww, r6
    mov r6, c10
    mad_pp r6, r3.w, r6, c11
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mul_sat_pp r1.xyz, r6, r1
    mov r7.xy, t1.zwzw
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r5, r7, s2
    texld_pp r0, r0, s3
    mul_pp r1.xyz, r1, r5.z
    texld_pp r7, t2, s1
    texld_pp r8, t1, s0
    mad_pp r7.xyz, r7, c0.x, r8
    add_pp r7.xyz, r7, c0.y
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    mad_pp r1.xyz, r7, r2, r1
    mul_pp r0.xyz, r6, r0
    mul_pp r0.w, r6.w, r5.z
    mad_pp r5.xyz, r0, r0.w, r1
    mov_pp oC0, r5

// approximately 129 instruction slots used (7 texture, 122 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyComplexFogBaseNormDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, true, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 3, -2
    def c4, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r0.xy, c3
    mad r1.w, r0.w, -c12.w, -r0.y
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.zw, r2.xyxy, c3.x, c3.y
    mul r0.zw, r0, c6.y
    mad r2.xy, r3, c3.x, c3.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r2.z, r3.w, c3.z
    mov_sat r2.z, r2.z
    mul r2.xy, r2, r2.z
    mad r0.zw, r0, c6.w, -r2.xyxy
    mad_sat r2.zw, r3.w, c3.z, c3.xyyw
    mad r3.xyz, r3, c3.x, c3.y
    mad r0.zw, r2.z, r0, r2.xyxy
    mad r2.xy, r2.w, -r0.zwzw, r0.zwzw
    mov r2.z, c4.x
    mad r2.xyz, r3, c5.xxyw, r2
    mul r3.xyz, r2.y, t4
    mad r2.xyw, t5.xyzz, r2.x, r3.xyzz
    mad r2.xyz, t3, r2.z, r2.xyww
    dp3_sat r2.w, r2, r1
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r1, -c13
    mov r3, c14
    mad_sat r2.w, r2.w, c13.w, r3.w
    mul r4.x, r1.w, r2.w
    mov r5.w, c15.w
    add r5.xyz, -t0, c15
    dp4 r1.w, r5, r5
    nrm r6.xyz, r5
    mad r1.w, r1.w, -c15.w, -r0.y
    dp3_sat r2.w, r2, r6
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r6, -c16
    mov r3.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r3.w
    mul r4.y, r1.w, r2.w
    max r0.yz, r4.xxyw, t2.z
    min r4.xy, t2.w, r0.yzzw
    dp3 r1.w, -r6, r2
    add r1.w, r1.w, r1.w
    mad r0.yzw, r2.xxyz, -r1.w, -r6.xxyz
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r0.yzww, r6
    max r2.w, r1.w, c4.x
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r0.yzw, r1.w, c17.xxyz
    mul r0.yzw, r4.y, r0
    dp3 r1.w, -r1, r2
    add r1.w, r1.w, r1.w
    mad r1.xyz, r2, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r3.w, r2.w, c4.x
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r0.yzw, r1.xxyz, r4.x, r0
    dp3 r2.w, -c18, r2
    mul r3.w, r2.w, c9.z
    max r1.x, r2.w, r3.w
    dp3 r1.y, -c20, r2
    max r4.zw, r1.xyxy, t2.z
    min r1.xy, t2.w, r4.zwzw
    dp3 r2.w, c18, r2
    add r2.w, r2.w, r2.w
    mad r7.xyz, r2, -r2.w, c18
    dp3 r2.w, r7, r6
    max r3.w, r2.w, c4.x
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r7.xyz, r2.w, c19
    mad_pp r0.yzw, r7.xxyz, r1.x, r0
    dp3 r2.w, c20, r2
    add r2.w, r2.w, r2.w
    mad r7.xyz, r2, -r2.w, c20
    dp3 r2.w, r7, r6
    dp3 r3.w, r2, r6
    mov r6, c10
    mad_pp r6, r3.w, r6, c11
    max r3.w, r2.w, c4.x
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r7.xyz, r2.w, c21
    mad_pp r0.yzw, r7.xxyz, r1.y, r0
    mul_sat_pp r0.yzw, r6.xxyz, r0
    mov r7.xy, t1.zwzw
    texld_pp r8, t1, s0
    texld_pp r7, r7, s2
    mul_pp r0.yzw, r0, r7.z
    mad_pp r3.xyz, r4.x, r3, c22
    mad_pp r3.xyz, r4.y, c17, r3
    mad_pp r1.xzw, r1.x, c19.xyyz, r3.xyyz
    mad_pp r1.xyz, r1.y, c21, r1.xzww
    mad_pp r0.yzw, r8.xxyz, r1.xxyz, r0
    dp3 r2.w, r5, r2
    mul r1.xyz, r2, r2.w
    mad_pp r1.xyz, r1, c8.w, -r5
    texld_pp r2, t2, s1
    texld_pp r1, r1, s3
    mul_pp r1.xyz, r6, r1
    mul_pp r1.w, r6.w, r7.z
    mad_pp r0.yzw, r1.xxyz, r1.w, r0
    mul_pp r0.yzw, r2.xxyz, r0
    mul_pp r0.yzw, r0, c0.w
    add r1.xyz, r0.yzww, r0.yzww
    mad r0.xyz, r0.yzww, -r0.x, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r7.xyz, r0, r1
    mov_pp oC0, r7

// approximately 135 instruction slots used (7 texture, 128 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyComplexFogBaseNormDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, true, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 3, -2
    def c4, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c3.x, c3.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c3.x, c3.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c3.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c3.z, c3.xyyw
    mad r3.xyz, r3, c3.x, c3.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov_pp r2.z, c4.x
    mad r0.yzw, r3.xxyz, c5.xxxy, r2.xxyz
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    mul r0.x, r0.x, r2.x
    dp3 r2.x, r1, -c13
    mov r3, c14
    mad_sat r3.w, r2.x, c13.w, r3.w
    mul r2.x, r0.x, r3.w
    mov r4.w, c15.w
    add r4.xyz, -t0, c15
    dp4 r3.w, r4, r4
    nrm r5.xyz, r4
    mad r1.w, r3.w, -c15.w, -r1.w
    dp3_sat r3.w, r0.yzww, r5
    mul r1.w, r1.w, r3.w
    dp3 r3.w, r5, -c16
    mov r5.w, c17.w
    mad_sat r3.w, r3.w, c16.w, r5.w
    mul r2.y, r1.w, r3.w
    max r4.xy, r2, t2.z
    min r2.xy, t2.w, r4
    dp3 r1.w, -r5, r0.yzww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.yzww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c4.x
    pow r3.w, r4.w, c7.z
    mul r3.w, r3.w, c7.w
    mul r1.xyz, r3.w, c14
    mad_pp r1.xyz, r1, r2.x, r4
    dp3 r1.w, -c18, r0.yzww
    mul r3.w, r1.w, c9.z
    max r4.x, r1.w, r3.w
    dp3 r4.y, -c20, r0.yzww
    max r2.zw, r4.xyxy, t2.z
    min r4.xy, t2.w, r2.zwzw
    dp3 r1.w, c18, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c18
    dp3 r1.w, r7, r6
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c19
    mad_pp r1.xyz, r7, r4.x, r1
    dp3 r1.w, c20, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c20
    dp3 r1.w, r7, r6
    dp3 r3.w, r0.yzww, r6
    mov r6, c10
    mad_pp r6, r3.w, r6, c11
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mul_sat_pp r1.xyz, r6, r1
    mov r7.xy, t1.zwzw
    texld_pp r8, t1, s0
    texld_pp r7, r7, s2
    mul_pp r1.xyz, r1, r7.z
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r2, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mul_pp r0.w, r6.w, r7.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul_pp r0.xyz, r2, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r7.xyz, r0, r1
    mov_pp oC0, r7

// approximately 134 instruction slots used (7 texture, 127 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddComplexFogBaseNormDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, true, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 3, -2
    def c4, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c3.x, c3.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c3.x, c3.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c3.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c3.z, c3.xyyw
    mad r3.xyz, r3, c3.x, c3.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c4.x
    mad r0.yzw, r3.xxyz, c5.xxxy, r2.xxyz
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    mul r0.x, r0.x, r2.x
    dp3 r2.x, r1, -c13
    mov r3, c14
    mad_sat r3.w, r2.x, c13.w, r3.w
    mul r2.x, r0.x, r3.w
    mov r4.w, c15.w
    add r4.xyz, -t0, c15
    dp4 r3.w, r4, r4
    nrm r5.xyz, r4
    mad r1.w, r3.w, -c15.w, -r1.w
    dp3_sat r3.w, r0.yzww, r5
    mul r1.w, r1.w, r3.w
    dp3 r3.w, r5, -c16
    mov r5.w, c17.w
    mad_sat r3.w, r3.w, c16.w, r5.w
    mul r2.y, r1.w, r3.w
    max r4.xy, r2, t2.z
    min r2.xy, t2.w, r4
    dp3 r1.w, -r5, r0.yzww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.yzww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c4.x
    pow r3.w, r4.w, c7.z
    mul r3.w, r3.w, c7.w
    mul r1.xyz, r3.w, c14
    mad_pp r1.xyz, r1, r2.x, r4
    dp3 r1.w, -c18, r0.yzww
    mul r3.w, r1.w, c9.z
    max r4.x, r1.w, r3.w
    dp3 r4.y, -c20, r0.yzww
    max r2.zw, r4.xyxy, t2.z
    min r4.xy, t2.w, r2.zwzw
    dp3 r1.w, c18, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c18
    dp3 r1.w, r7, r6
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c19
    mad_pp r1.xyz, r7, r4.x, r1
    dp3 r1.w, c20, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c20
    dp3 r1.w, r7, r6
    dp3 r3.w, r0.yzww, r6
    mov r6, c10
    mad_pp r6, r3.w, r6, c11
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mul_sat_pp r1.xyz, r6, r1
    mov r7.xy, t1.zwzw
    texld_pp r8, t1, s0
    texld_pp r7, r7, s2
    mul_pp r1.xyz, r1, r7.z
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r2, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mul_pp r0.w, r6.w, r7.z
    mad_pp r0.xyz, r0, r0.w, r1
    mad_pp r0.xyz, r2, c3.x, r0
    add_pp r0.xyz, r0, c3.y
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r7.xyz, r0, r1
    mov_pp oC0, r7

// approximately 135 instruction slots used (7 texture, 128 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyComplexFogBaseNormDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, true, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 3, -2
    def c4, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c3.x, c3.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c3.x, c3.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c3.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c3.z, c3.xyyw
    mad r3.xyz, r3, c3.x, c3.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c4.x
    mad r0.yzw, r3.xxyz, c5.xxxy, r2.xxyz
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    mul r0.x, r0.x, r2.x
    dp3 r2.x, r1, -c13
    mov r3, c14
    mad_sat r3.w, r2.x, c13.w, r3.w
    mul r2.x, r0.x, r3.w
    mov r4.w, c15.w
    add r4.xyz, -t0, c15
    dp4 r3.w, r4, r4
    nrm r5.xyz, r4
    mad r1.w, r3.w, -c15.w, -r1.w
    dp3_sat r3.w, r0.yzww, r5
    mul r1.w, r1.w, r3.w
    dp3 r3.w, r5, -c16
    mov r5.w, c17.w
    mad_sat r3.w, r3.w, c16.w, r5.w
    mul r2.y, r1.w, r3.w
    max r4.xy, r2, t2.z
    min r2.xy, t2.w, r4
    dp3 r1.w, -r5, r0.yzww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.yzww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c4.x
    pow r3.w, r4.w, c7.z
    mul r3.w, r3.w, c7.w
    mul r1.xyz, r3.w, c14
    mad_pp r1.xyz, r1, r2.x, r4
    dp3 r1.w, -c18, r0.yzww
    mul r3.w, r1.w, c9.z
    max r4.x, r1.w, r3.w
    dp3 r4.y, -c20, r0.yzww
    max r2.zw, r4.xyxy, t2.z
    min r4.xy, t2.w, r2.zwzw
    dp3 r1.w, c18, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c18
    dp3 r1.w, r7, r6
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c19
    mad_pp r1.xyz, r7, r4.x, r1
    dp3 r1.w, c20, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c20
    dp3 r1.w, r7, r6
    dp3 r3.w, r0.yzww, r6
    mov r6, c10
    mad_pp r6, r3.w, r6, c11
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mul_sat_pp r1.xyz, r6, r1
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mul_pp r3.xyz, r3, r4
    mul_pp r2.xyz, r2, r3
    add_pp r2.xyz, r2, r2
    mov r3.xy, t1.zwzw
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, r3, s2
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r1, r3.z, r2
    mul_pp r0.xyz, r6, r0
    mul_pp r0.w, r6.w, r3.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 135 instruction slots used (7 texture, 128 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyComplexFogBaseNormDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, true, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 3, -2
    def c4, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c3.x, c3.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c3.x, c3.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c3.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c3.z, c3.xyyw
    mad r3.xyz, r3, c3.x, c3.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov_pp r2.z, c4.x
    mad r0.yzw, r3.xxyz, c5.xxxy, r2.xxyz
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    mul r0.x, r0.x, r2.x
    dp3 r2.x, r1, -c13
    mov r3, c14
    mad_sat r3.w, r2.x, c13.w, r3.w
    mul r2.x, r0.x, r3.w
    mov r4.w, c15.w
    add r4.xyz, -t0, c15
    dp4 r3.w, r4, r4
    nrm r5.xyz, r4
    mad r1.w, r3.w, -c15.w, -r1.w
    dp3_sat r3.w, r0.yzww, r5
    mul r1.w, r1.w, r3.w
    dp3 r3.w, r5, -c16
    mov r5.w, c17.w
    mad_sat r3.w, r3.w, c16.w, r5.w
    mul r2.y, r1.w, r3.w
    max r4.xy, r2, t2.z
    min r2.xy, t2.w, r4
    dp3 r1.w, -r5, r0.yzww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.yzww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c4.x
    pow r3.w, r4.w, c7.z
    mul r3.w, r3.w, c7.w
    mul r1.xyz, r3.w, c14
    mad_pp r1.xyz, r1, r2.x, r4
    dp3 r1.w, -c18, r0.yzww
    mul r3.w, r1.w, c9.z
    max r4.x, r1.w, r3.w
    dp3 r4.y, -c20, r0.yzww
    max r2.zw, r4.xyxy, t2.z
    min r4.xy, t2.w, r2.zwzw
    dp3 r1.w, c18, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c18
    dp3 r1.w, r7, r6
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c19
    mad_pp r1.xyz, r7, r4.x, r1
    dp3 r1.w, c20, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c20
    dp3 r1.w, r7, r6
    dp3 r3.w, r0.yzww, r6
    mov r6, c10
    mad_pp r6, r3.w, r6, c11
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mul_sat_pp r1.xyz, r6, r1
    mov r7.xy, t1.zwzw
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r5, r7, s2
    texld_pp r0, r0, s3
    mul_pp r1.xyz, r1, r5.z
    texld_pp r7, t2, s1
    texld_pp r8, t1, s0
    mul_pp r7.xyz, r7, r8
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    mad_pp r1.xyz, r7, r2, r1
    mul_pp r0.xyz, r6, r0
    mul_pp r0.w, r6.w, r5.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r5.xyz, r0, r1
    mov_pp oC0, r5

// approximately 134 instruction slots used (7 texture, 127 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddComplexFogBaseNormDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, true, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 3, -2
    def c4, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c3.x, c3.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c3.x, c3.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c3.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c3.z, c3.xyyw
    mad r3.xyz, r3, c3.x, c3.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c4.x
    mad r0.yzw, r3.xxyz, c5.xxxy, r2.xxyz
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    mul r0.x, r0.x, r2.x
    dp3 r2.x, r1, -c13
    mov r3, c14
    mad_sat r3.w, r2.x, c13.w, r3.w
    mul r2.x, r0.x, r3.w
    mov r4.w, c15.w
    add r4.xyz, -t0, c15
    dp4 r3.w, r4, r4
    nrm r5.xyz, r4
    mad r1.w, r3.w, -c15.w, -r1.w
    dp3_sat r3.w, r0.yzww, r5
    mul r1.w, r1.w, r3.w
    dp3 r3.w, r5, -c16
    mov r5.w, c17.w
    mad_sat r3.w, r3.w, c16.w, r5.w
    mul r2.y, r1.w, r3.w
    max r4.xy, r2, t2.z
    min r2.xy, t2.w, r4
    dp3 r1.w, -r5, r0.yzww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.yzww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c4.x
    pow r3.w, r4.w, c7.z
    mul r3.w, r3.w, c7.w
    mul r1.xyz, r3.w, c14
    mad_pp r1.xyz, r1, r2.x, r4
    dp3 r1.w, -c18, r0.yzww
    mul r3.w, r1.w, c9.z
    max r4.x, r1.w, r3.w
    dp3 r4.y, -c20, r0.yzww
    max r2.zw, r4.xyxy, t2.z
    min r4.xy, t2.w, r2.zwzw
    dp3 r1.w, c18, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c18
    dp3 r1.w, r7, r6
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c19
    mad_pp r1.xyz, r7, r4.x, r1
    dp3 r1.w, c20, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c20
    dp3 r1.w, r7, r6
    dp3 r3.w, r0.yzww, r6
    mov r6, c10
    mad_pp r6, r3.w, r6, c11
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mul_sat_pp r1.xyz, r6, r1
    mov r7.xy, t1.zwzw
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r5, r7, s2
    texld_pp r0, r0, s3
    mul_pp r1.xyz, r1, r5.z
    texld_pp r7, t2, s1
    texld_pp r8, t1, s0
    mad_pp r7.xyz, r7, c3.x, r8
    add_pp r7.xyz, r7, c3.y
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    mad_pp r1.xyz, r7, r2, r1
    mul_pp r0.xyz, r6, r0
    mul_pp r0.w, r6.w, r5.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r5.xyz, r0, r1
    mov_pp oC0, r5

// approximately 135 instruction slots used (7 texture, 128 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyBaseNormDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, false, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 3, -2
    def c1, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c0.x, c0.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c0.x, c0.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c0.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c0.z, c0.xyyw
    mad r3.xyz, r3, c0.x, c0.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c1.x
    mad r0.yzw, r3.xxyz, c5.xxxy, r2.xxyz
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    mul r0.x, r0.x, r2.x
    dp3 r2.x, r1, -c13
    mov r3, c14
    mad_sat r3.w, r2.x, c13.w, r3.w
    mul r2.x, r0.x, r3.w
    mov r4.w, c15.w
    add r4.xyz, -t0, c15
    dp4 r3.w, r4, r4
    nrm r5.xyz, r4
    mad r1.w, r3.w, -c15.w, -r1.w
    dp3_sat r3.w, r0.yzww, r5
    mul r1.w, r1.w, r3.w
    dp3 r3.w, r5, -c16
    mov r5.w, c17.w
    mad_sat r3.w, r3.w, c16.w, r5.w
    mul r2.y, r1.w, r3.w
    max r4.xy, r2, t2.z
    min r2.xy, t2.w, r4
    dp3 r1.w, -r5, r0.yzww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.yzww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c1.x
    pow r3.w, r4.w, c7.z
    mul r3.w, r3.w, c7.w
    mul r1.xyz, r3.w, c14
    mad_pp r1.xyz, r1, r2.x, r4
    dp3 r1.w, -c18, r0.yzww
    mul r3.w, r1.w, c9.z
    max r4.x, r1.w, r3.w
    dp3 r4.y, -c20, r0.yzww
    max r2.zw, r4.xyxy, t2.z
    min r4.xy, t2.w, r2.zwzw
    dp3 r1.w, c18, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c18
    dp3 r1.w, r7, r6
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c19
    mad_pp r1.xyz, r7, r4.x, r1
    dp3 r1.w, c20, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c20
    dp3 r1.w, r7, r6
    dp3 r3.w, r0.yzww, r6
    mov r6, c10
    mad_pp r6, r3.w, r6, c11
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mov r7.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r7, r7, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r6.xyz, r6, r8
    mul_sat_pp r1.xyz, r1, r6
    mul_pp r1.xyz, r7.z, r1
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r0.xyz, r6, r0
    mul_pp r0.w, r6.w, r7.z
    mad_pp r0.xyz, r0, r0.w, r1
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    add_pp r7.xyz, r0, r0
    mov_pp oC0, r7

// approximately 139 instruction slots used (8 texture, 131 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyBaseNormDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, false, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 3, -2
    def c1, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c0.x, c0.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c0.x, c0.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c0.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c0.z, c0.xyyw
    mad r3.xyz, r3, c0.x, c0.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov_pp r2.z, c1.x
    mad r0.yzw, r3.xxyz, c5.xxxy, r2.xxyz
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    mul r0.x, r0.x, r2.x
    dp3 r2.x, r1, -c13
    mov r3, c14
    mad_sat r3.w, r2.x, c13.w, r3.w
    mul r2.x, r0.x, r3.w
    mov r4.w, c15.w
    add r4.xyz, -t0, c15
    dp4 r3.w, r4, r4
    nrm r5.xyz, r4
    mad r1.w, r3.w, -c15.w, -r1.w
    dp3_sat r3.w, r0.yzww, r5
    mul r1.w, r1.w, r3.w
    dp3 r3.w, r5, -c16
    mov r5.w, c17.w
    mad_sat r3.w, r3.w, c16.w, r5.w
    mul r2.y, r1.w, r3.w
    max r4.xy, r2, t2.z
    min r2.xy, t2.w, r4
    dp3 r1.w, -r5, r0.yzww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.yzww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c1.x
    pow r3.w, r4.w, c7.z
    mul r3.w, r3.w, c7.w
    mul r1.xyz, r3.w, c14
    mad_pp r1.xyz, r1, r2.x, r4
    dp3 r1.w, -c18, r0.yzww
    mul r3.w, r1.w, c9.z
    max r4.x, r1.w, r3.w
    dp3 r4.y, -c20, r0.yzww
    max r2.zw, r4.xyxy, t2.z
    min r4.xy, t2.w, r2.zwzw
    dp3 r1.w, c18, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c18
    dp3 r1.w, r7, r6
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c19
    mad_pp r1.xyz, r7, r4.x, r1
    dp3 r1.w, c20, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c20
    dp3 r1.w, r7, r6
    dp3 r3.w, r0.yzww, r6
    mov r6, c10
    mad_pp r6, r3.w, r6, c11
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mov r7.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r7, r7, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r6.xyz, r6, r8
    mul_sat_pp r1.xyz, r1, r6
    mul_pp r1.xyz, r7.z, r1
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r0.xyz, r6, r0
    mul_pp r0.w, r6.w, r7.z
    mad_pp r0.xyz, r0, r0.w, r1
    texld_pp r1, t2, s1
    mul_pp r7.xyz, r0, r1
    mov_pp oC0, r7

// approximately 138 instruction slots used (8 texture, 130 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddBaseNormDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, false, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 3, -2
    def c1, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c0.x, c0.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c0.x, c0.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c0.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c0.z, c0.xyyw
    mad r3.xyz, r3, c0.x, c0.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c1.x
    mad r0.yzw, r3.xxyz, c5.xxxy, r2.xxyz
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    mul r0.x, r0.x, r2.x
    dp3 r2.x, r1, -c13
    mov r3, c14
    mad_sat r3.w, r2.x, c13.w, r3.w
    mul r2.x, r0.x, r3.w
    mov r4.w, c15.w
    add r4.xyz, -t0, c15
    dp4 r3.w, r4, r4
    nrm r5.xyz, r4
    mad r1.w, r3.w, -c15.w, -r1.w
    dp3_sat r3.w, r0.yzww, r5
    mul r1.w, r1.w, r3.w
    dp3 r3.w, r5, -c16
    mov r5.w, c17.w
    mad_sat r3.w, r3.w, c16.w, r5.w
    mul r2.y, r1.w, r3.w
    max r4.xy, r2, t2.z
    min r2.xy, t2.w, r4
    dp3 r1.w, -r5, r0.yzww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.yzww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c1.x
    pow r3.w, r4.w, c7.z
    mul r3.w, r3.w, c7.w
    mul r1.xyz, r3.w, c14
    mad_pp r1.xyz, r1, r2.x, r4
    dp3 r1.w, -c18, r0.yzww
    mul r3.w, r1.w, c9.z
    max r4.x, r1.w, r3.w
    dp3 r4.y, -c20, r0.yzww
    max r2.zw, r4.xyxy, t2.z
    min r4.xy, t2.w, r2.zwzw
    dp3 r1.w, c18, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c18
    dp3 r1.w, r7, r6
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c19
    mad_pp r1.xyz, r7, r4.x, r1
    dp3 r1.w, c20, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c20
    dp3 r1.w, r7, r6
    dp3 r3.w, r0.yzww, r6
    mov r6, c10
    mad_pp r6, r3.w, r6, c11
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mov r7.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r7, r7, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r6.xyz, r6, r8
    mul_sat_pp r1.xyz, r1, r6
    mul_pp r1.xyz, r7.z, r1
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r0.xyz, r6, r0
    mul_pp r0.w, r6.w, r7.z
    mad_pp r0.xyz, r0, r0.w, r1
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c0.x, r0
    add_pp r7.xyz, r0, c0.y
    mov_pp oC0, r7

// approximately 139 instruction slots used (8 texture, 131 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyBaseNormDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, false, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 3, -2
    def c1, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c0.x, c0.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c0.x, c0.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c0.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c0.z, c0.xyyw
    mad r3.xyz, r3, c0.x, c0.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c1.x
    mad r0.yzw, r3.xxyz, c5.xxxy, r2.xxyz
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    mul r0.x, r0.x, r2.x
    dp3 r2.x, r1, -c13
    mov r3, c14
    mad_sat r3.w, r2.x, c13.w, r3.w
    mul r2.x, r0.x, r3.w
    mov r4.w, c15.w
    add r4.xyz, -t0, c15
    dp4 r3.w, r4, r4
    nrm r5.xyz, r4
    mad r1.w, r3.w, -c15.w, -r1.w
    dp3_sat r3.w, r0.yzww, r5
    mul r1.w, r1.w, r3.w
    dp3 r3.w, r5, -c16
    mov r5.w, c17.w
    mad_sat r3.w, r3.w, c16.w, r5.w
    mul r2.y, r1.w, r3.w
    max r4.xy, r2, t2.z
    min r2.xy, t2.w, r4
    dp3 r1.w, -r5, r0.yzww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.yzww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c1.x
    pow r3.w, r4.w, c7.z
    mul r3.w, r3.w, c7.w
    mul r1.xyz, r3.w, c14
    mad_pp r1.xyz, r1, r2.x, r4
    dp3 r1.w, -c18, r0.yzww
    mul r3.w, r1.w, c9.z
    max r4.x, r1.w, r3.w
    dp3 r4.y, -c20, r0.yzww
    max r2.zw, r4.xyxy, t2.z
    min r4.xy, t2.w, r2.zwzw
    dp3 r1.w, c18, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c18
    dp3 r1.w, r7, r6
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c19
    mad_pp r1.xyz, r7, r4.x, r1
    dp3 r1.w, c20, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c20
    dp3 r1.w, r7, r6
    dp3 r3.w, r0.yzww, r6
    mov r6, c10
    mad_pp r6, r3.w, r6, c11
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mov r7.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r7, r7, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r6.xyz, r6, r8
    mul_sat_pp r1.xyz, r1, r6
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mul_pp r3.xyz, r3, r4
    mul_pp r2.xyz, r2, r3
    add_pp r2.xyz, r2, r2
    mad_pp r1.xyz, r1, r7.z, r2
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mul_pp r0.w, r6.w, r7.z
    mad_pp r7.xyz, r0, r0.w, r1
    mov_pp oC0, r7

// approximately 139 instruction slots used (8 texture, 131 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyBaseNormDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, false, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 3, -2
    def c1, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c0.x, c0.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c0.x, c0.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c0.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c0.z, c0.xyyw
    mad r3.xyz, r3, c0.x, c0.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov_pp r2.z, c1.x
    mad r0.yzw, r3.xxyz, c5.xxxy, r2.xxyz
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    mul r0.x, r0.x, r2.x
    dp3 r2.x, r1, -c13
    mov r3, c14
    mad_sat r3.w, r2.x, c13.w, r3.w
    mul r2.x, r0.x, r3.w
    mov r4.w, c15.w
    add r4.xyz, -t0, c15
    dp4 r3.w, r4, r4
    nrm r5.xyz, r4
    mad r1.w, r3.w, -c15.w, -r1.w
    dp3_sat r3.w, r0.yzww, r5
    mul r1.w, r1.w, r3.w
    dp3 r3.w, r5, -c16
    mov r5.w, c17.w
    mad_sat r3.w, r3.w, c16.w, r5.w
    mul r2.y, r1.w, r3.w
    max r4.xy, r2, t2.z
    min r2.xy, t2.w, r4
    dp3 r1.w, -r5, r0.yzww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.yzww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c1.x
    pow r3.w, r4.w, c7.z
    mul r3.w, r3.w, c7.w
    mul r1.xyz, r3.w, c14
    mad_pp r1.xyz, r1, r2.x, r4
    dp3 r1.w, -c18, r0.yzww
    mul r3.w, r1.w, c9.z
    max r4.x, r1.w, r3.w
    dp3 r4.y, -c20, r0.yzww
    max r2.zw, r4.xyxy, t2.z
    min r4.xy, t2.w, r2.zwzw
    dp3 r1.w, c18, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c18
    dp3 r1.w, r7, r6
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c19
    mad_pp r1.xyz, r7, r4.x, r1
    dp3 r1.w, c20, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c20
    dp3 r1.w, r7, r6
    dp3 r3.w, r0.yzww, r6
    mov r6, c10
    mad_pp r6, r3.w, r6, c11
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mov r7.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r7, r7, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r6.xyz, r6, r8
    mul_sat_pp r1.xyz, r1, r6
    mul_pp r1.xyz, r7.z, r1
    texld_pp r8, t2, s1
    texld_pp r9, t1, s0
    mul_pp r8.xyz, r8, r9
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mul_pp r0.w, r6.w, r7.z
    mad_pp r7.xyz, r0, r0.w, r1
    mov_pp oC0, r7

// approximately 138 instruction slots used (8 texture, 130 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddBaseNormDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, false, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 3, -2
    def c1, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c0.x, c0.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c0.x, c0.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c0.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c0.z, c0.xyyw
    mad r3.xyz, r3, c0.x, c0.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c1.x
    mad r0.yzw, r3.xxyz, c5.xxxy, r2.xxyz
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    mul r0.x, r0.x, r2.x
    dp3 r2.x, r1, -c13
    mov r3, c14
    mad_sat r3.w, r2.x, c13.w, r3.w
    mul r2.x, r0.x, r3.w
    mov r4.w, c15.w
    add r4.xyz, -t0, c15
    dp4 r3.w, r4, r4
    nrm r5.xyz, r4
    mad r1.w, r3.w, -c15.w, -r1.w
    dp3_sat r3.w, r0.yzww, r5
    mul r1.w, r1.w, r3.w
    dp3 r3.w, r5, -c16
    mov r5.w, c17.w
    mad_sat r3.w, r3.w, c16.w, r5.w
    mul r2.y, r1.w, r3.w
    max r4.xy, r2, t2.z
    min r2.xy, t2.w, r4
    dp3 r1.w, -r5, r0.yzww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.yzww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c1.x
    pow r3.w, r4.w, c7.z
    mul r3.w, r3.w, c7.w
    mul r1.xyz, r3.w, c14
    mad_pp r1.xyz, r1, r2.x, r4
    dp3 r1.w, -c18, r0.yzww
    mul r3.w, r1.w, c9.z
    max r4.x, r1.w, r3.w
    dp3 r4.y, -c20, r0.yzww
    max r2.zw, r4.xyxy, t2.z
    min r4.xy, t2.w, r2.zwzw
    dp3 r1.w, c18, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c18
    dp3 r1.w, r7, r6
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c19
    mad_pp r1.xyz, r7, r4.x, r1
    dp3 r1.w, c20, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c20
    dp3 r1.w, r7, r6
    dp3 r3.w, r0.yzww, r6
    mov r6, c10
    mad_pp r6, r3.w, r6, c11
    max r3.w, r1.w, c1.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mov r7.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r7, r7, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r6.xyz, r6, r8
    mul_sat_pp r1.xyz, r1, r6
    mul_pp r1.xyz, r7.z, r1
    texld_pp r8, t2, s1
    texld_pp r9, t1, s0
    mad_pp r8.xyz, r8, c0.x, r9
    add_pp r8.xyz, r8, c0.y
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mul_pp r0.w, r6.w, r7.z
    mad_pp r7.xyz, r0, r0.w, r1
    mov_pp oC0, r7

// approximately 139 instruction slots used (8 texture, 131 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyComplexFogBaseNormDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, true, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 3, -2
    def c4, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r0.xy, c3
    mad r1.w, r0.w, -c12.w, -r0.y
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.zw, r2.xyxy, c3.x, c3.y
    mul r0.zw, r0, c6.y
    mad r2.xy, r3, c3.x, c3.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r2.z, r3.w, c3.z
    mov_sat r2.z, r2.z
    mul r2.xy, r2, r2.z
    mad r0.zw, r0, c6.w, -r2.xyxy
    mad_sat r2.zw, r3.w, c3.z, c3.xyyw
    mad r3.xyz, r3, c3.x, c3.y
    mad r0.zw, r2.z, r0, r2.xyxy
    mad r2.xy, r2.w, -r0.zwzw, r0.zwzw
    mov r2.z, c4.x
    mad r2.xyz, r3, c5.xxyw, r2
    mul r3.xyz, r2.y, t4
    mad r2.xyw, t5.xyzz, r2.x, r3.xyzz
    mad r2.xyz, t3, r2.z, r2.xyww
    dp3_sat r2.w, r2, r1
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r1, -c13
    mov r3, c14
    mad_sat r2.w, r2.w, c13.w, r3.w
    mul r4.x, r1.w, r2.w
    mov r5.w, c15.w
    add r5.xyz, -t0, c15
    dp4 r1.w, r5, r5
    nrm r6.xyz, r5
    mad r1.w, r1.w, -c15.w, -r0.y
    dp3_sat r2.w, r2, r6
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r6, -c16
    mov r3.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r3.w
    mul r4.y, r1.w, r2.w
    max r0.yz, r4.xxyw, t2.z
    min r4.xy, t2.w, r0.yzzw
    dp3 r1.w, -r6, r2
    add r1.w, r1.w, r1.w
    mad r0.yzw, r2.xxyz, -r1.w, -r6.xxyz
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r0.yzww, r6
    max r2.w, r1.w, c4.x
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r0.yzw, r1.w, c17.xxyz
    mul r0.yzw, r4.y, r0
    dp3 r1.w, -r1, r2
    add r1.w, r1.w, r1.w
    mad r1.xyz, r2, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r3.w, r2.w, c4.x
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r0.yzw, r1.xxyz, r4.x, r0
    dp3 r2.w, -c18, r2
    mul r3.w, r2.w, c9.z
    max r1.x, r2.w, r3.w
    dp3 r1.y, -c20, r2
    max r4.zw, r1.xyxy, t2.z
    min r1.xy, t2.w, r4.zwzw
    dp3 r2.w, c18, r2
    add r2.w, r2.w, r2.w
    mad r7.xyz, r2, -r2.w, c18
    dp3 r2.w, r7, r6
    max r3.w, r2.w, c4.x
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r7.xyz, r2.w, c19
    mad_pp r0.yzw, r7.xxyz, r1.x, r0
    dp3 r2.w, c20, r2
    add r2.w, r2.w, r2.w
    mad r7.xyz, r2, -r2.w, c20
    dp3 r2.w, r7, r6
    dp3 r3.w, r2, r6
    mov r6, c10
    mad_pp r6, r3.w, r6, c11
    max r3.w, r2.w, c4.x
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r7.xyz, r2.w, c21
    mad_pp r0.yzw, r7.xxyz, r1.y, r0
    mov r7.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r7, r7, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r6.xyz, r6, r8
    mul_sat_pp r0.yzw, r0, r6.xxyz
    mul_pp r0.yzw, r7.z, r0
    mad_pp r3.xyz, r4.x, r3, c22
    mad_pp r3.xyz, r4.y, c17, r3
    mad_pp r1.xzw, r1.x, c19.xyyz, r3.xyyz
    mad_pp r1.xyz, r1.y, c21, r1.xzww
    dp3 r1.w, r5, r2
    mul r2.xyz, r2, r1.w
    mad_pp r2.xyz, r2, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mad_pp r0.yzw, r3.xxyz, r1.xxyz, r0
    mul_pp r1.xyz, r6, r2
    mul_pp r1.w, r6.w, r7.z
    mad_pp r0.yzw, r1.xxyz, r1.w, r0
    texld_pp r1, t2, s1
    mul_pp r0.yzw, r0, r1.xxyz
    mul_pp r0.yzw, r0, c0.w
    add r1.xyz, r0.yzww, r0.yzww
    mad r0.xyz, r0.yzww, -r0.x, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r7.xyz, r0, r1
    mov_pp oC0, r7

// approximately 145 instruction slots used (8 texture, 137 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyComplexFogBaseNormDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, true, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 3, -2
    def c4, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c3.x, c3.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c3.x, c3.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c3.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c3.z, c3.xyyw
    mad r3.xyz, r3, c3.x, c3.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov_pp r2.z, c4.x
    mad r0.yzw, r3.xxyz, c5.xxxy, r2.xxyz
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    mul r0.x, r0.x, r2.x
    dp3 r2.x, r1, -c13
    mov r3, c14
    mad_sat r3.w, r2.x, c13.w, r3.w
    mul r2.x, r0.x, r3.w
    mov r4.w, c15.w
    add r4.xyz, -t0, c15
    dp4 r3.w, r4, r4
    nrm r5.xyz, r4
    mad r1.w, r3.w, -c15.w, -r1.w
    dp3_sat r3.w, r0.yzww, r5
    mul r1.w, r1.w, r3.w
    dp3 r3.w, r5, -c16
    mov r5.w, c17.w
    mad_sat r3.w, r3.w, c16.w, r5.w
    mul r2.y, r1.w, r3.w
    max r4.xy, r2, t2.z
    min r2.xy, t2.w, r4
    dp3 r1.w, -r5, r0.yzww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.yzww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c4.x
    pow r3.w, r4.w, c7.z
    mul r3.w, r3.w, c7.w
    mul r1.xyz, r3.w, c14
    mad_pp r1.xyz, r1, r2.x, r4
    dp3 r1.w, -c18, r0.yzww
    mul r3.w, r1.w, c9.z
    max r4.x, r1.w, r3.w
    dp3 r4.y, -c20, r0.yzww
    max r2.zw, r4.xyxy, t2.z
    min r4.xy, t2.w, r2.zwzw
    dp3 r1.w, c18, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c18
    dp3 r1.w, r7, r6
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c19
    mad_pp r1.xyz, r7, r4.x, r1
    dp3 r1.w, c20, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c20
    dp3 r1.w, r7, r6
    dp3 r3.w, r0.yzww, r6
    mov r6, c10
    mad_pp r6, r3.w, r6, c11
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mov r7.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r7, r7, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r6.xyz, r6, r8
    mul_sat_pp r1.xyz, r1, r6
    mul_pp r1.xyz, r7.z, r1
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r0.xyz, r6, r0
    mul_pp r0.w, r6.w, r7.z
    mad_pp r0.xyz, r0, r0.w, r1
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r7.xyz, r0, r1
    mov_pp oC0, r7

// approximately 144 instruction slots used (8 texture, 136 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddComplexFogBaseNormDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, true, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 3, -2
    def c4, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c3.x, c3.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c3.x, c3.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c3.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c3.z, c3.xyyw
    mad r3.xyz, r3, c3.x, c3.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c4.x
    mad r0.yzw, r3.xxyz, c5.xxxy, r2.xxyz
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    mul r0.x, r0.x, r2.x
    dp3 r2.x, r1, -c13
    mov r3, c14
    mad_sat r3.w, r2.x, c13.w, r3.w
    mul r2.x, r0.x, r3.w
    mov r4.w, c15.w
    add r4.xyz, -t0, c15
    dp4 r3.w, r4, r4
    nrm r5.xyz, r4
    mad r1.w, r3.w, -c15.w, -r1.w
    dp3_sat r3.w, r0.yzww, r5
    mul r1.w, r1.w, r3.w
    dp3 r3.w, r5, -c16
    mov r5.w, c17.w
    mad_sat r3.w, r3.w, c16.w, r5.w
    mul r2.y, r1.w, r3.w
    max r4.xy, r2, t2.z
    min r2.xy, t2.w, r4
    dp3 r1.w, -r5, r0.yzww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.yzww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c4.x
    pow r3.w, r4.w, c7.z
    mul r3.w, r3.w, c7.w
    mul r1.xyz, r3.w, c14
    mad_pp r1.xyz, r1, r2.x, r4
    dp3 r1.w, -c18, r0.yzww
    mul r3.w, r1.w, c9.z
    max r4.x, r1.w, r3.w
    dp3 r4.y, -c20, r0.yzww
    max r2.zw, r4.xyxy, t2.z
    min r4.xy, t2.w, r2.zwzw
    dp3 r1.w, c18, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c18
    dp3 r1.w, r7, r6
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c19
    mad_pp r1.xyz, r7, r4.x, r1
    dp3 r1.w, c20, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c20
    dp3 r1.w, r7, r6
    dp3 r3.w, r0.yzww, r6
    mov r6, c10
    mad_pp r6, r3.w, r6, c11
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mov r7.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r7, r7, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r6.xyz, r6, r8
    mul_sat_pp r1.xyz, r1, r6
    mul_pp r1.xyz, r7.z, r1
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r0.xyz, r6, r0
    mul_pp r0.w, r6.w, r7.z
    mad_pp r0.xyz, r0, r0.w, r1
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c3.x, r0
    add_pp r0.xyz, r0, c3.y
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r7.xyz, r0, r1
    mov_pp oC0, r7

// approximately 145 instruction slots used (8 texture, 137 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyComplexFogBaseNormDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, true, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 3, -2
    def c4, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c3.x, c3.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c3.x, c3.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c3.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c3.z, c3.xyyw
    mad r3.xyz, r3, c3.x, c3.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c4.x
    mad r0.yzw, r3.xxyz, c5.xxxy, r2.xxyz
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    mul r0.x, r0.x, r2.x
    dp3 r2.x, r1, -c13
    mov r3, c14
    mad_sat r3.w, r2.x, c13.w, r3.w
    mul r2.x, r0.x, r3.w
    mov r4.w, c15.w
    add r4.xyz, -t0, c15
    dp4 r3.w, r4, r4
    nrm r5.xyz, r4
    mad r1.w, r3.w, -c15.w, -r1.w
    dp3_sat r3.w, r0.yzww, r5
    mul r1.w, r1.w, r3.w
    dp3 r3.w, r5, -c16
    mov r5.w, c17.w
    mad_sat r3.w, r3.w, c16.w, r5.w
    mul r2.y, r1.w, r3.w
    max r4.xy, r2, t2.z
    min r2.xy, t2.w, r4
    dp3 r1.w, -r5, r0.yzww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.yzww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c4.x
    pow r3.w, r4.w, c7.z
    mul r3.w, r3.w, c7.w
    mul r1.xyz, r3.w, c14
    mad_pp r1.xyz, r1, r2.x, r4
    dp3 r1.w, -c18, r0.yzww
    mul r3.w, r1.w, c9.z
    max r4.x, r1.w, r3.w
    dp3 r4.y, -c20, r0.yzww
    max r2.zw, r4.xyxy, t2.z
    min r4.xy, t2.w, r2.zwzw
    dp3 r1.w, c18, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c18
    dp3 r1.w, r7, r6
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c19
    mad_pp r1.xyz, r7, r4.x, r1
    dp3 r1.w, c20, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c20
    dp3 r1.w, r7, r6
    dp3 r3.w, r0.yzww, r6
    mov r6, c10
    mad_pp r6, r3.w, r6, c11
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mov r7.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r7, r7, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r6.xyz, r6, r8
    mul_sat_pp r1.xyz, r1, r6
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mul_pp r3.xyz, r3, r4
    mul_pp r2.xyz, r2, r3
    add_pp r2.xyz, r2, r2
    mad_pp r1.xyz, r1, r7.z, r2
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mul_pp r0.w, r6.w, r7.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r7.xyz, r0, r1
    mov_pp oC0, r7

// approximately 145 instruction slots used (8 texture, 137 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyComplexFogBaseNormDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, true, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 3, -2
    def c4, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c3.x, c3.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c3.x, c3.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c3.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c3.z, c3.xyyw
    mad r3.xyz, r3, c3.x, c3.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov_pp r2.z, c4.x
    mad r0.yzw, r3.xxyz, c5.xxxy, r2.xxyz
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    mul r0.x, r0.x, r2.x
    dp3 r2.x, r1, -c13
    mov r3, c14
    mad_sat r3.w, r2.x, c13.w, r3.w
    mul r2.x, r0.x, r3.w
    mov r4.w, c15.w
    add r4.xyz, -t0, c15
    dp4 r3.w, r4, r4
    nrm r5.xyz, r4
    mad r1.w, r3.w, -c15.w, -r1.w
    dp3_sat r3.w, r0.yzww, r5
    mul r1.w, r1.w, r3.w
    dp3 r3.w, r5, -c16
    mov r5.w, c17.w
    mad_sat r3.w, r3.w, c16.w, r5.w
    mul r2.y, r1.w, r3.w
    max r4.xy, r2, t2.z
    min r2.xy, t2.w, r4
    dp3 r1.w, -r5, r0.yzww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.yzww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c4.x
    pow r3.w, r4.w, c7.z
    mul r3.w, r3.w, c7.w
    mul r1.xyz, r3.w, c14
    mad_pp r1.xyz, r1, r2.x, r4
    dp3 r1.w, -c18, r0.yzww
    mul r3.w, r1.w, c9.z
    max r4.x, r1.w, r3.w
    dp3 r4.y, -c20, r0.yzww
    max r2.zw, r4.xyxy, t2.z
    min r4.xy, t2.w, r2.zwzw
    dp3 r1.w, c18, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c18
    dp3 r1.w, r7, r6
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c19
    mad_pp r1.xyz, r7, r4.x, r1
    dp3 r1.w, c20, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c20
    dp3 r1.w, r7, r6
    dp3 r3.w, r0.yzww, r6
    mov r6, c10
    mad_pp r6, r3.w, r6, c11
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mov r7.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r7, r7, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r6.xyz, r6, r8
    mul_sat_pp r1.xyz, r1, r6
    mul_pp r1.xyz, r7.z, r1
    texld_pp r8, t2, s1
    texld_pp r9, t1, s0
    mul_pp r8.xyz, r8, r9
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mul_pp r0.w, r6.w, r7.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r7.xyz, r0, r1
    mov_pp oC0, r7

// approximately 144 instruction slots used (8 texture, 136 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddComplexFogBaseNormDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, true, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 3, -2
    def c4, 0, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c3.x, c3.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c3.x, c3.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c3.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c3.z, c3.xyyw
    mad r3.xyz, r3, c3.x, c3.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c4.x
    mad r0.yzw, r3.xxyz, c5.xxxy, r2.xxyz
    mul r2.xyz, r0.z, t4
    mad r2.xyz, t5, r0.y, r2
    mad r0.yzw, t3.xxyz, r0.w, r2.xxyz
    dp3_sat r2.x, r0.yzww, r1
    mul r0.x, r0.x, r2.x
    dp3 r2.x, r1, -c13
    mov r3, c14
    mad_sat r3.w, r2.x, c13.w, r3.w
    mul r2.x, r0.x, r3.w
    mov r4.w, c15.w
    add r4.xyz, -t0, c15
    dp4 r3.w, r4, r4
    nrm r5.xyz, r4
    mad r1.w, r3.w, -c15.w, -r1.w
    dp3_sat r3.w, r0.yzww, r5
    mul r1.w, r1.w, r3.w
    dp3 r3.w, r5, -c16
    mov r5.w, c17.w
    mad_sat r3.w, r3.w, c16.w, r5.w
    mul r2.y, r1.w, r3.w
    max r4.xy, r2, t2.z
    min r2.xy, t2.w, r4
    dp3 r1.w, -r5, r0.yzww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.yzww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c4.x
    pow r3.w, r4.w, c7.z
    mul r3.w, r3.w, c7.w
    mul r1.xyz, r3.w, c14
    mad_pp r1.xyz, r1, r2.x, r4
    dp3 r1.w, -c18, r0.yzww
    mul r3.w, r1.w, c9.z
    max r4.x, r1.w, r3.w
    dp3 r4.y, -c20, r0.yzww
    max r2.zw, r4.xyxy, t2.z
    min r4.xy, t2.w, r2.zwzw
    dp3 r1.w, c18, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c18
    dp3 r1.w, r7, r6
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c19
    mad_pp r1.xyz, r7, r4.x, r1
    dp3 r1.w, c20, r0.yzww
    add r1.w, r1.w, r1.w
    mad r7.xyz, r0.yzww, -r1.w, c20
    dp3 r1.w, r7, r6
    dp3 r3.w, r0.yzww, r6
    mov r6, c10
    mad_pp r6, r3.w, r6, c11
    max r3.w, r1.w, c4.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mov r7.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r7, r7, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r6.xyz, r6, r8
    mul_sat_pp r1.xyz, r1, r6
    mul_pp r1.xyz, r7.z, r1
    texld_pp r8, t2, s1
    texld_pp r9, t1, s0
    mad_pp r8.xyz, r8, c3.x, r9
    add_pp r8.xyz, r8, c3.y
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mul_pp r0.w, r6.w, r7.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r7.xyz, r0, r1
    mov_pp oC0, r7

// approximately 145 instruction slots used (8 texture, 137 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, false, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c0.x, c0.y
    mul r0.yz, r0, c6.x
    mul r0.yz, r0, c6.z
    mul r2.xyz, r0.z, t4
    mad r0.yzw, t5.xxyz, r0.y, r2.xxyz
    add r0.yzw, r0, t3.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r0.xyz, r4, r1, r0
    mul_pp r0.xyz, r3, r0
    add_pp r2.xyz, r0, r0
    mov_pp oC0, r2

// approximately 69 instruction slots used (5 texture, 64 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, false, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c0.x, c0.y
    mul r0.yz, r0, c6.x
    mul r0.yz, r0, c6.z
    mul r2.xyz, r0.z, t4
    mad r0.yzw, t5.xxyz, r0.y, r2.xxyz
    add r0.yzw, r0, t3.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r0.xyz, r4, r1, r0
    mul_pp r2.xyz, r3, r0
    mov_pp oC0, r2

// approximately 68 instruction slots used (5 texture, 63 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, false, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c0.x, c0.y
    mul r0.yz, r0, c6.x
    mul r0.yz, r0, c6.z
    mul r2.xyz, r0.z, t4
    mad r0.yzw, t5.xxyz, r0.y, r2.xxyz
    add r0.yzw, r0, t3.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r0.xyz, r4, r1, r0
    mad_pp r0.xyz, r3, c0.x, r0
    add_pp r2.xyz, r0, c0.y
    mov_pp oC0, r2

// approximately 69 instruction slots used (5 texture, 64 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, false, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c0.x, c0.y
    mul r0.yz, r0, c6.x
    mul r0.yz, r0, c6.z
    mul r2.xyz, r0.z, t4
    mad r0.yzw, t5.xxyz, r0.y, r2.xxyz
    add r0.yzw, r0, t3.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mul_pp r2.xyz, r2, r3
    mul_pp r1.xyz, r1, r2
    add_pp r1.xyz, r1, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mad_pp r2.xyz, r0, r0.w, r1
    mov_pp oC0, r2

// approximately 69 instruction slots used (5 texture, 64 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, false, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c0.x, c0.y
    mul r0.yz, r0, c6.x
    mul r0.yz, r0, c6.z
    mul r2.xyz, r0.z, t4
    mad r0.yzw, t5.xxyz, r0.y, r2.xxyz
    add r0.yzw, r0, t3.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mul_pp r2.xyz, r2, r3
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r3.xy, t1.zwzw
    texld_pp r4, r4, s3
    texld_pp r3, r3, s2
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r3.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 68 instruction slots used (5 texture, 63 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, false, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c0.x, c0.y
    mul r0.yz, r0, c6.x
    mul r0.yz, r0, c6.z
    mul r2.xyz, r0.z, t4
    mad r0.yzw, t5.xxyz, r0.y, r2.xxyz
    add r0.yzw, r0, t3.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r2.xyz, r2, c0.x, r3
    add_pp r2.xyz, r2, c0.y
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r3.xy, t1.zwzw
    texld_pp r4, r4, s3
    texld_pp r3, r3, s2
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r3.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 69 instruction slots used (5 texture, 64 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyComplexFogDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, true, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r0.xz, c3
    mad r1.w, r0.w, -c12.w, r0.z
    texld r2, t6, s5
    mad r0.yw, r2.xxzy, c3.x, c3.y
    mul r0.yw, r0, c6.x
    mul r0.yw, r0, c6.z
    mul r2.xyz, r0.w, t4
    mad r2.xyz, t5, r0.y, r2
    add r2.xyz, r2, t3
    dp3_sat r2.w, r2, r1
    dp3 r0.y, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.y, r0.y, r0.w, c14.w
    mul r0.w, r1.w, r2.w
    mul r1.x, r0.y, r0.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r4.w, r2, r4
    dp3 r0.y, r4, -c16
    mov r0.w, c16.w
    mad_sat r0.y, r0.y, r0.w, c17.w
    mul r2.w, r2.w, r4.w
    mul r1.y, r0.y, r2.w
    max r0.yz, r1.xxyw, t2.z
    min r1.xy, t2.w, r0.yzzw
    mov r3.xyz, c14
    mad_pp r0.yzw, r1.x, r3.xxyz, c22.xxyz
    mad_pp r0.yzw, r1.y, c17.xxyz, r0
    dp3 r2.w, -c18, r2
    mul r1.x, r2.w, c9.z
    max r3.x, r2.w, r1.x
    dp3 r3.y, -c20, r2
    max r1.xy, r3, t2.z
    min r3.xy, t2.w, r1
    mad_pp r0.yzw, r3.x, c19.xxyz, r0
    mad_pp r0.yzw, r3.y, c21.xxyz, r0
    add r1.xyz, -t0, c8
    dp3 r1.w, r1, r2
    mul r3.xyz, r2, r1.w
    mad_pp r3.xyz, r3, c8.w, -r1
    nrm r4.xyz, r1
    dp3 r3.w, r2, r4
    mov r1, c10
    mad_pp r1, r3.w, r1, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r1.xyz, r1, r3
    mul_pp r1.w, r1.w, r2.z
    mul_pp r1.xyz, r1.w, r1
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r0.yzw, r4.xxyz, r0, r1.xxyz
    mul_pp r0.yzw, r3.xxyz, r0
    mul_pp r0.yzw, r0, c0.w
    add r1.xyz, r0.yzww, r0.yzww
    mad r0.xyz, r0.yzww, -r0.x, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 75 instruction slots used (5 texture, 70 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyComplexFogDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, true, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c3.x, c3.y
    mul r0.yz, r0, c6.x
    mul r0.yz, r0, c6.z
    mul r2.xyz, r0.z, t4
    mad r0.yzw, t5.xxyz, r0.y, r2.xxyz
    add r0.yzw, r0, t3.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r0.xyz, r4, r1, r0
    mul_pp r0.xyz, r3, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 74 instruction slots used (5 texture, 69 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddComplexFogDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, true, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c3.x, c3.y
    mul r0.yz, r0, c6.x
    mul r0.yz, r0, c6.z
    mul r2.xyz, r0.z, t4
    mad r0.yzw, t5.xxyz, r0.y, r2.xxyz
    add r0.yzw, r0, t3.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r0.xyz, r4, r1, r0
    mad_pp r0.xyz, r3, c3.x, r0
    add_pp r0.xyz, r0, c3.y
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 75 instruction slots used (5 texture, 70 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyComplexFogDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, true, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c3.x, c3.y
    mul r0.yz, r0, c6.x
    mul r0.yz, r0, c6.z
    mul r2.xyz, r0.z, t4
    mad r0.yzw, t5.xxyz, r0.y, r2.xxyz
    add r0.yzw, r0, t3.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mul_pp r2.xyz, r2, r3
    mul_pp r1.xyz, r1, r2
    add_pp r1.xyz, r1, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r2.xy, t1.zwzw
    texld_pp r3, r3, s3
    texld_pp r2, r2, s2
    mul_pp r0.xyz, r0, r3
    mul_pp r0.w, r0.w, r2.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 75 instruction slots used (5 texture, 70 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyComplexFogDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, true, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c3.x, c3.y
    mul r0.yz, r0, c6.x
    mul r0.yz, r0, c6.z
    mul r2.xyz, r0.z, t4
    mad r0.yzw, t5.xxyz, r0.y, r2.xxyz
    add r0.yzw, r0, t3.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mul_pp r2.xyz, r2, r3
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r3.xy, t1.zwzw
    texld_pp r4, r4, s3
    texld_pp r3, r3, s2
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r3.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r2, r1, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 74 instruction slots used (5 texture, 69 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddComplexFogDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, true, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c3.x, c3.y
    mul r0.yz, r0, c6.x
    mul r0.yz, r0, c6.z
    mul r2.xyz, r0.z, t4
    mad r0.yzw, t5.xxyz, r0.y, r2.xxyz
    add r0.yzw, r0, t3.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r2.xyz, r2, c3.x, r3
    add_pp r2.xyz, r2, c3.y
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mov r3.xy, t1.zwzw
    texld_pp r4, r4, s3
    texld_pp r3, r3, s2
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r3.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r2, r1, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 75 instruction slots used (5 texture, 70 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, false, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c0.x, c0.y
    mul r0.yz, r0, c6.x
    mul r0.yz, r0, c6.z
    mul r2.xyz, r0.z, t4
    mad r0.yzw, t5.xxyz, r0.y, r2.xxyz
    add r0.yzw, r0, t3.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    add_pp r2.xyz, r0, r0
    mov_pp oC0, r2

// approximately 79 instruction slots used (6 texture, 73 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, false, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c0.x, c0.y
    mul r0.yz, r0, c6.x
    mul r0.yz, r0, c6.z
    mul r2.xyz, r0.z, t4
    mad r0.yzw, t5.xxyz, r0.y, r2.xxyz
    add r0.yzw, r0, t3.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    texld_pp r1, t2, s1
    mul_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 78 instruction slots used (6 texture, 72 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, false, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c0.x, c0.y
    mul r0.yz, r0, c6.x
    mul r0.yz, r0, c6.z
    mul r2.xyz, r0.z, t4
    mad r0.yzw, t5.xxyz, r0.y, r2.xxyz
    add r0.yzw, r0, t3.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c0.x, r0
    add_pp r2.xyz, r0, c0.y
    mov_pp oC0, r2

// approximately 79 instruction slots used (6 texture, 73 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, false, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c0.x, c0.y
    mul r0.yz, r0, c6.x
    mul r0.yz, r0, c6.z
    mul r2.xyz, r0.z, t4
    mad r0.yzw, t5.xxyz, r0.y, r2.xxyz
    add r0.yzw, r0, t3.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mul_pp r2.xyz, r2, r3
    mul_pp r1.xyz, r1, r2
    add_pp r1.xyz, r1, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mad_pp r2.xyz, r0, r0.w, r1
    mov_pp oC0, r2

// approximately 79 instruction slots used (6 texture, 73 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, false, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c0.x, c0.y
    mul r0.yz, r0, c6.x
    mul r0.yz, r0, c6.z
    mul r2.xyz, r0.z, t4
    mad r0.yzw, t5.xxyz, r0.y, r2.xxyz
    add r0.yzw, r0, t3.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mul_pp r3.xyz, r3, r4
    mad_pp r2.xyz, r3, r1, r0
    mov_pp oC0, r2

// approximately 78 instruction slots used (6 texture, 72 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, false, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c0.x, c0.y
    mul r0.yz, r0, c6.x
    mul r0.yz, r0, c6.z
    mul r2.xyz, r0.z, t4
    mad r0.yzw, t5.xxyz, r0.y, r2.xxyz
    add r0.yzw, r0, t3.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r3.xyz, r3, c0.x, r4
    add_pp r3.xyz, r3, c0.y
    mad_pp r2.xyz, r3, r1, r0
    mov_pp oC0, r2

// approximately 79 instruction slots used (6 texture, 73 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyComplexFogDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, true, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r0.xz, c3
    mad r1.w, r0.w, -c12.w, r0.z
    texld r2, t6, s5
    mad r0.yw, r2.xxzy, c3.x, c3.y
    mul r0.yw, r0, c6.x
    mul r0.yw, r0, c6.z
    mul r2.xyz, r0.w, t4
    mad r2.xyz, t5, r0.y, r2
    add r2.xyz, r2, t3
    dp3_sat r2.w, r2, r1
    dp3 r0.y, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.y, r0.y, r0.w, c14.w
    mul r0.w, r1.w, r2.w
    mul r1.x, r0.y, r0.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r4.w, r2, r4
    dp3 r0.y, r4, -c16
    mov r0.w, c16.w
    mad_sat r0.y, r0.y, r0.w, c17.w
    mul r2.w, r2.w, r4.w
    mul r1.y, r0.y, r2.w
    max r0.yz, r1.xxyw, t2.z
    min r1.xy, t2.w, r0.yzzw
    mov r3.xyz, c14
    mad_pp r0.yzw, r1.x, r3.xxyz, c22.xxyz
    mad_pp r0.yzw, r1.y, c17.xxyz, r0
    dp3 r2.w, -c18, r2
    mul r1.x, r2.w, c9.z
    max r3.x, r2.w, r1.x
    dp3 r3.y, -c20, r2
    max r1.xy, r3, t2.z
    min r3.xy, t2.w, r1
    mad_pp r0.yzw, r3.x, c19.xxyz, r0
    mad_pp r0.yzw, r3.y, c21.xxyz, r0
    add r1.xyz, -t0, c8
    dp3 r1.w, r1, r2
    mul r3.xyz, r2, r1.w
    mad_pp r3.xyz, r3, c8.w, -r1
    nrm r4.xyz, r1
    dp3 r3.w, r2, r4
    mov r1, c10
    mad_pp r1, r3.w, r1, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r1.xyz, r1, r2
    mul_pp r1.xyz, r1, r3
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mul_pp r1.w, r1.w, r2.z
    mul_pp r1.xyz, r1.w, r1
    mad_pp r0.yzw, r3.xxyz, r0, r1.xxyz
    texld_pp r1, t2, s1
    mul_pp r0.yzw, r0, r1.xxyz
    mul_pp r0.yzw, r0, c0.w
    add r1.xyz, r0.yzww, r0.yzww
    mad r0.xyz, r0.yzww, -r0.x, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 85 instruction slots used (6 texture, 79 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyComplexFogDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, true, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c3.x, c3.y
    mul r0.yz, r0, c6.x
    mul r0.yz, r0, c6.z
    mul r2.xyz, r0.z, t4
    mad r0.yzw, t5.xxyz, r0.y, r2.xxyz
    add r0.yzw, r0, t3.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 84 instruction slots used (6 texture, 78 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddComplexFogDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, true, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c3.x, c3.y
    mul r0.yz, r0, c6.x
    mul r0.yz, r0, c6.z
    mul r2.xyz, r0.z, t4
    mad r0.yzw, t5.xxyz, r0.y, r2.xxyz
    add r0.yzw, r0, t3.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c3.x, r0
    add_pp r0.xyz, r0, c3.y
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 85 instruction slots used (6 texture, 79 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyComplexFogDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, true, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c3.x, c3.y
    mul r0.yz, r0, c6.x
    mul r0.yz, r0, c6.z
    mul r2.xyz, r0.z, t4
    mad r0.yzw, t5.xxyz, r0.y, r2.xxyz
    add r0.yzw, r0, t3.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mul_pp r2.xyz, r2, r3
    mul_pp r1.xyz, r1, r2
    add_pp r1.xyz, r1, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 85 instruction slots used (6 texture, 79 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyComplexFogDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, true, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c3.x, c3.y
    mul r0.yz, r0, c6.x
    mul r0.yz, r0, c6.z
    mul r2.xyz, r0.z, t4
    mad r0.yzw, t5.xxyz, r0.y, r2.xxyz
    add r0.yzw, r0, t3.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mul_pp r3.xyz, r3, r4
    mad_pp r0.xyz, r3, r1, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 84 instruction slots used (6 texture, 78 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddComplexFogDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, true, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c3.x, c3.y
    mul r0.yz, r0, c6.x
    mul r0.yz, r0, c6.z
    mul r2.xyz, r0.z, t4
    mad r0.yzw, t5.xxyz, r0.y, r2.xxyz
    add r0.yzw, r0, t3.xxyz
    dp3_sat r2.x, r0.yzww, r1
    dp3 r1.x, r1, -c13
    mov r2.w, c13.w
    mad_sat r1.x, r1.x, r2.w, c14.w
    mul r0.x, r0.x, r2.x
    mul r1.x, r1.x, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r1.w
    dp3_sat r0.x, r0.yzww, r3
    dp3 r1.z, r3, -c16
    mov r1.w, c16.w
    mad_sat r1.z, r1.z, r1.w, c17.w
    mul r1.w, r0.x, r3.w
    mul r1.y, r1.z, r1.w
    max r2.xy, r1, t2.z
    min r1.xy, t2.w, r2
    mov r2.xyz, c14
    mad_pp r1.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r1.xyz, r1.y, c17, r1.xzww
    dp3 r1.w, -c18, r0.yzww
    mul r0.x, r1.w, c9.z
    max r2.x, r1.w, r0.x
    dp3 r2.y, -c20, r0.yzww
    max r3.xy, r2, t2.z
    min r2.xy, t2.w, r3
    mad_pp r1.xyz, r2.x, c19, r1
    mad_pp r1.xyz, r2.y, c21, r1
    add r2.xyz, -t0, c8
    dp3 r1.w, r2, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2
    nrm r4.xyz, r2
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r2, t1, s7
    texld_pp r3, r3, s3
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r2.xyz, r4, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.z
    mul_sat_pp r2.xyz, r4, c5.w
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, r3
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r3.xyz, r3, c3.x, r4
    add_pp r3.xyz, r3, c3.y
    mad_pp r0.xyz, r3, r1, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 85 instruction slots used (6 texture, 79 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, false, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c0.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c0.x, c0.y
    mul r0.xy, r0, c6.x
    mul r0.xy, r0, c6.z
    mul r3.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r3.xyzz
    add r0.xyw, r0, t3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c0.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s0
    texld_pp r6, r6, s2
    mul_pp r1.xyz, r1, r6.z
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r2, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul_pp r0.xyz, r2, r0
    add_pp r6.xyz, r0, r0
    mov_pp oC0, r6

// approximately 114 instruction slots used (5 texture, 109 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, false, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c0.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c0.x, c0.y
    mul r0.xy, r0, c6.x
    mul r0.xy, r0, c6.z
    mul r3.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r3.xyzz
    add r0.xyw, r0, t3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c0.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s0
    texld_pp r6, r6, s2
    mul_pp r1.xyz, r1, r6.z
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r2, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul_pp r6.xyz, r2, r0
    mov_pp oC0, r6

// approximately 113 instruction slots used (5 texture, 108 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, false, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c0.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c0.x, c0.y
    mul r0.xy, r0, c6.x
    mul r0.xy, r0, c6.z
    mul r3.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r3.xyzz
    add r0.xyw, r0, t3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c0.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s0
    texld_pp r6, r6, s2
    mul_pp r1.xyz, r1, r6.z
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r2, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    mad_pp r0.xyz, r2, c0.x, r0
    add_pp r6.xyz, r0, c0.y
    mov_pp oC0, r6

// approximately 114 instruction slots used (5 texture, 109 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, false, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c0.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c0.x, c0.y
    mul r0.xy, r0, c6.x
    mul r0.xy, r0, c6.z
    mul r3.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r3.xyzz
    add r0.xyw, r0, t3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c0.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    texld_pp r3, t2, s1
    texld_pp r6, t1, s0
    mul_pp r3.xyz, r3, r6
    mul_pp r2.xyz, r2, r3
    add_pp r2.xyz, r2, r2
    mov r3.xy, t1.zwzw
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, r3, s2
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r1, r3.z, r2
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r3.z
    mad_pp r3.xyz, r0, r0.w, r1
    mov_pp oC0, r3

// approximately 114 instruction slots used (5 texture, 109 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, false, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c0.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c0.x, c0.y
    mul r0.xy, r0, c6.x
    mul r0.xy, r0, c6.z
    mul r3.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r3.xyzz
    add r0.xyw, r0, t3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c0.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mov r6.xy, t1.zwzw
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r5, r6, s2
    texld_pp r0, r0, s3
    mul_pp r1.xyz, r1, r5.z
    texld_pp r6, t2, s1
    texld_pp r8, t1, s0
    mul_pp r6.xyz, r6, r8
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r6, r2, r1
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r5.z
    mad_pp r5.xyz, r0, r0.w, r1
    mov_pp oC0, r5

// approximately 113 instruction slots used (5 texture, 108 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, false, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c0.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c0.x, c0.y
    mul r0.xy, r0, c6.x
    mul r0.xy, r0, c6.z
    mul r3.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r3.xyzz
    add r0.xyw, r0, t3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c0.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mov r6.xy, t1.zwzw
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r5, r6, s2
    texld_pp r0, r0, s3
    mul_pp r1.xyz, r1, r5.z
    texld_pp r6, t2, s1
    texld_pp r8, t1, s0
    mad_pp r6.xyz, r6, c0.x, r8
    add_pp r6.xyz, r6, c0.y
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r6, r2, r1
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r5.z
    mad_pp r5.xyz, r0, r0.w, r1
    mov_pp oC0, r5

// approximately 114 instruction slots used (5 texture, 109 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyComplexFogDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, true, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.xz, c3
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.yw, r3.xxzy, c3.x, c3.y
    mul r0.yw, r0, c6.x
    mul r0.yw, r0, c6.z
    mul r3.xyz, r0.w, t4
    mad r3.xyz, t5, r0.y, r3
    add r3.xyz, r3, t3
    dp3_sat r3.w, r3, r1
    mul r2.w, r2.w, r3.w
    mul r4.x, r1.w, r2.w
    add r5.xyz, -t0, c15
    nrm r6.xyz, r5
    dp3 r1.w, r6, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r5.w, c15.w
    dp4 r2.w, r5, r5
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r3.w, r3, r6
    mul r2.w, r2.w, r3.w
    mul r4.y, r1.w, r2.w
    max r0.yz, r4.xxyw, t2.z
    min r4.xy, t2.w, r0.yzzw
    dp3 r1.w, -r6, r3
    add r1.w, r1.w, r1.w
    mad r0.yzw, r3.xxyz, -r1.w, -r6.xxyz
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r0.yzww, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r0.yzw, r1.w, c17.xxyz
    mul r0.yzw, r4.y, r0
    dp3 r1.w, -r1, r3
    add r1.w, r1.w, r1.w
    mad r1.xyz, r3, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r3.w, r2.w, c3.w
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r0.yzw, r1.xxyz, r4.x, r0
    dp3 r2.w, c18, r3
    add r2.w, r2.w, r2.w
    mad r1.xyz, r3, -r2.w, c18
    dp3 r2.w, r1, r6
    max r3.w, r2.w, c3.w
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c19
    dp3 r1.w, -c18, r3
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r3
    max r4.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r4.zwzw
    mad_pp r0.yzw, r1.xxyz, r7.x, r0
    dp3 r2.w, c20, r3
    add r2.w, r2.w, r2.w
    mad r1.xyz, r3, -r2.w, c20
    dp3 r2.w, r1, r6
    dp3 r3.w, r3, r6
    mov r1, c10
    mad_pp r1, r3.w, r1, c11
    max r3.w, r2.w, c3.w
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r6.xyz, r2.w, c21
    mad_pp r0.yzw, r6.xxyz, r7.y, r0
    mul_sat_pp r0.yzw, r1.xxyz, r0
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s0
    texld_pp r6, r6, s2
    mul_pp r0.yzw, r0, r6.z
    mad_pp r2.xyz, r4.x, r2, c22
    mad_pp r2.xyz, r4.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r0.yzw, r8.xxyz, r2.xxyz, r0
    dp3 r3.w, r5, r3
    mul r2.xyz, r3, r3.w
    mad_pp r2.xyz, r2, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r2, r2, s3
    mul_pp r1.xyz, r1, r2
    mul_pp r1.w, r1.w, r6.z
    mad_pp r0.yzw, r1.xxyz, r1.w, r0
    mul_pp r0.yzw, r3.xxyz, r0
    mul_pp r0.yzw, r0, c0.w
    add r1.xyz, r0.yzww, r0.yzww
    mad r0.xyz, r0.yzww, -r0.x, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 120 instruction slots used (5 texture, 115 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyComplexFogDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, true, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c3.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c3.x, c3.y
    mul r0.xy, r0, c6.x
    mul r0.xy, r0, c6.z
    mul r3.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r3.xyzz
    add r0.xyw, r0, t3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c3.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s0
    texld_pp r6, r6, s2
    mul_pp r1.xyz, r1, r6.z
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r2, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul_pp r0.xyz, r2, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 119 instruction slots used (5 texture, 114 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddComplexFogDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, true, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c3.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c3.x, c3.y
    mul r0.xy, r0, c6.x
    mul r0.xy, r0, c6.z
    mul r3.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r3.xyzz
    add r0.xyw, r0, t3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c3.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s0
    texld_pp r6, r6, s2
    mul_pp r1.xyz, r1, r6.z
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r2, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    mad_pp r0.xyz, r2, c3.x, r0
    add_pp r0.xyz, r0, c3.y
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 120 instruction slots used (5 texture, 115 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyComplexFogDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, true, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c3.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c3.x, c3.y
    mul r0.xy, r0, c6.x
    mul r0.xy, r0, c6.z
    mul r3.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r3.xyzz
    add r0.xyw, r0, t3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c3.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    texld_pp r3, t2, s1
    texld_pp r6, t1, s0
    mul_pp r3.xyz, r3, r6
    mul_pp r2.xyz, r2, r3
    add_pp r2.xyz, r2, r2
    mov r3.xy, t1.zwzw
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, r3, s2
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r1, r3.z, r2
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r3.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 120 instruction slots used (5 texture, 115 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyComplexFogDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, true, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c3.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c3.x, c3.y
    mul r0.xy, r0, c6.x
    mul r0.xy, r0, c6.z
    mul r3.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r3.xyzz
    add r0.xyw, r0, t3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c3.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mov r6.xy, t1.zwzw
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r5, r6, s2
    texld_pp r0, r0, s3
    mul_pp r1.xyz, r1, r5.z
    texld_pp r6, t2, s1
    texld_pp r8, t1, s0
    mul_pp r6.xyz, r6, r8
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r6, r2, r1
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r5.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r5.xyz, r0, r1
    mov_pp oC0, r5

// approximately 119 instruction slots used (5 texture, 114 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddComplexFogDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, true, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c3.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c3.x, c3.y
    mul r0.xy, r0, c6.x
    mul r0.xy, r0, c6.z
    mul r3.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r3.xyzz
    add r0.xyw, r0, t3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c3.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mov r6.xy, t1.zwzw
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r5, r6, s2
    texld_pp r0, r0, s3
    mul_pp r1.xyz, r1, r5.z
    texld_pp r6, t2, s1
    texld_pp r8, t1, s0
    mad_pp r6.xyz, r6, c3.x, r8
    add_pp r6.xyz, r6, c3.y
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r6, r2, r1
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r5.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r5.xyz, r0, r1
    mov_pp oC0, r5

// approximately 120 instruction slots used (5 texture, 115 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, false, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    dcl_2d s7
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c0.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c0.x, c0.y
    mul r0.xy, r0, c6.x
    mul r0.xy, r0, c6.z
    mul r3.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r3.xyzz
    add r0.xyw, r0, t3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c0.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r6, r6, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r4.xyz, r4, r8
    mul_sat_pp r1.xyz, r1, r4
    mul_pp r1.xyz, r6.z, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r5, t1, s0
    texld_pp r0, r0, s3
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r5, r2, r1
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    add_pp r6.xyz, r0, r0
    mov_pp oC0, r6

// approximately 124 instruction slots used (6 texture, 118 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, false, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    dcl_2d s7
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c0.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c0.x, c0.y
    mul r0.xy, r0, c6.x
    mul r0.xy, r0, c6.z
    mul r3.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r3.xyzz
    add r0.xyw, r0, t3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c0.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r6, r6, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r4.xyz, r4, r8
    mul_sat_pp r1.xyz, r1, r4
    mul_pp r1.xyz, r6.z, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r5, t1, s0
    texld_pp r0, r0, s3
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r5, r2, r1
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    texld_pp r1, t2, s1
    mul_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 123 instruction slots used (6 texture, 117 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, false, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    dcl_2d s7
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c0.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c0.x, c0.y
    mul r0.xy, r0, c6.x
    mul r0.xy, r0, c6.z
    mul r3.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r3.xyzz
    add r0.xyw, r0, t3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c0.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r6, r6, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r4.xyz, r4, r8
    mul_sat_pp r1.xyz, r1, r4
    mul_pp r1.xyz, r6.z, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r5, t1, s0
    texld_pp r0, r0, s3
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r5, r2, r1
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c0.x, r0
    add_pp r6.xyz, r0, c0.y
    mov_pp oC0, r6

// approximately 124 instruction slots used (6 texture, 118 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, false, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    dcl_2d s7
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c0.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c0.x, c0.y
    mul r0.xy, r0, c6.x
    mul r0.xy, r0, c6.z
    mul r3.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r3.xyzz
    add r0.xyw, r0, t3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c0.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r6, r6, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r4.xyz, r4, r8
    mul_sat_pp r1.xyz, r1, r4
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    texld_pp r3, t2, s1
    texld_pp r7, t1, s0
    mul_pp r3.xyz, r3, r7
    mul_pp r2.xyz, r2, r3
    add_pp r2.xyz, r2, r2
    mad_pp r1.xyz, r1, r6.z, r2
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r6.xyz, r0, r0.w, r1
    mov_pp oC0, r6

// approximately 124 instruction slots used (6 texture, 118 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, false, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    dcl_2d s7
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c0.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c0.x, c0.y
    mul r0.xy, r0, c6.x
    mul r0.xy, r0, c6.z
    mul r3.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r3.xyzz
    add r0.xyw, r0, t3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c0.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r6, r6, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r4.xyz, r4, r8
    mul_sat_pp r1.xyz, r1, r4
    mul_pp r1.xyz, r6.z, r1
    texld_pp r8, t2, s1
    texld_pp r9, t1, s0
    mul_pp r8.xyz, r8, r9
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r6.xyz, r0, r0.w, r1
    mov_pp oC0, r6

// approximately 123 instruction slots used (6 texture, 117 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, false, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    dcl_2d s7
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c0.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c0.x, c0.y
    mul r0.xy, r0, c6.x
    mul r0.xy, r0, c6.z
    mul r3.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r3.xyzz
    add r0.xyw, r0, t3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c0.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c0.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r6, r6, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r4.xyz, r4, r8
    mul_sat_pp r1.xyz, r1, r4
    mul_pp r1.xyz, r6.z, r1
    texld_pp r8, t2, s1
    texld_pp r9, t1, s0
    mad_pp r8.xyz, r8, c0.x, r9
    add_pp r8.xyz, r8, c0.y
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r6.xyz, r0, r0.w, r1
    mov_pp oC0, r6

// approximately 124 instruction slots used (6 texture, 118 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyComplexFogDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, true, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    dcl_2d s7
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.xz, c3
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.yw, r3.xxzy, c3.x, c3.y
    mul r0.yw, r0, c6.x
    mul r0.yw, r0, c6.z
    mul r3.xyz, r0.w, t4
    mad r3.xyz, t5, r0.y, r3
    add r3.xyz, r3, t3
    dp3_sat r3.w, r3, r1
    mul r2.w, r2.w, r3.w
    mul r4.x, r1.w, r2.w
    add r5.xyz, -t0, c15
    nrm r6.xyz, r5
    dp3 r1.w, r6, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r5.w, c15.w
    dp4 r2.w, r5, r5
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r3.w, r3, r6
    mul r2.w, r2.w, r3.w
    mul r4.y, r1.w, r2.w
    max r0.yz, r4.xxyw, t2.z
    min r4.xy, t2.w, r0.yzzw
    dp3 r1.w, -r6, r3
    add r1.w, r1.w, r1.w
    mad r0.yzw, r3.xxyz, -r1.w, -r6.xxyz
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r0.yzww, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r0.yzw, r1.w, c17.xxyz
    mul r0.yzw, r4.y, r0
    dp3 r1.w, -r1, r3
    add r1.w, r1.w, r1.w
    mad r1.xyz, r3, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r3.w, r2.w, c3.w
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r0.yzw, r1.xxyz, r4.x, r0
    dp3 r2.w, c18, r3
    add r2.w, r2.w, r2.w
    mad r1.xyz, r3, -r2.w, c18
    dp3 r2.w, r1, r6
    max r3.w, r2.w, c3.w
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c19
    dp3 r1.w, -c18, r3
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r3
    max r4.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r4.zwzw
    mad_pp r0.yzw, r1.xxyz, r7.x, r0
    dp3 r2.w, c20, r3
    add r2.w, r2.w, r2.w
    mad r1.xyz, r3, -r2.w, c20
    dp3 r2.w, r1, r6
    dp3 r3.w, r3, r6
    mov r1, c10
    mad_pp r1, r3.w, r1, c11
    max r3.w, r2.w, c3.w
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r6.xyz, r2.w, c21
    mad_pp r0.yzw, r6.xxyz, r7.y, r0
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r6, r6, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r1.xyz, r1, r8
    mul_sat_pp r0.yzw, r0, r1.xxyz
    mul_pp r0.yzw, r6.z, r0
    dp3 r2.w, r5, r3
    mul r3.xyz, r3, r2.w
    mad_pp r3.xyz, r3, c8.w, -r5
    texld_pp r5, t1, s0
    texld_pp r3, r3, s3
    mad_pp r2.xyz, r4.x, r2, c22
    mad_pp r2.xyz, r4.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r0.yzw, r5.xxyz, r2.xxyz, r0
    mul_pp r1.xyz, r1, r3
    mul_pp r1.w, r1.w, r6.z
    mad_pp r0.yzw, r1.xxyz, r1.w, r0
    texld_pp r1, t2, s1
    mul_pp r0.yzw, r0, r1.xxyz
    mul_pp r0.yzw, r0, c0.w
    add r1.xyz, r0.yzww, r0.yzww
    mad r0.xyz, r0.yzww, -r0.x, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 130 instruction slots used (6 texture, 124 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyComplexFogDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, true, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    dcl_2d s7
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c3.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c3.x, c3.y
    mul r0.xy, r0, c6.x
    mul r0.xy, r0, c6.z
    mul r3.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r3.xyzz
    add r0.xyw, r0, t3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c3.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r6, r6, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r4.xyz, r4, r8
    mul_sat_pp r1.xyz, r1, r4
    mul_pp r1.xyz, r6.z, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r5, t1, s0
    texld_pp r0, r0, s3
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r5, r2, r1
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 129 instruction slots used (6 texture, 123 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddComplexFogDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, true, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    dcl_2d s7
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c3.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c3.x, c3.y
    mul r0.xy, r0, c6.x
    mul r0.xy, r0, c6.z
    mul r3.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r3.xyzz
    add r0.xyw, r0, t3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c3.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r6, r6, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r4.xyz, r4, r8
    mul_sat_pp r1.xyz, r1, r4
    mul_pp r1.xyz, r6.z, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r5, t1, s0
    texld_pp r0, r0, s3
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r5, r2, r1
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c3.x, r0
    add_pp r0.xyz, r0, c3.y
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 130 instruction slots used (6 texture, 124 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyComplexFogDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, true, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    dcl_2d s7
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c3.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c3.x, c3.y
    mul r0.xy, r0, c6.x
    mul r0.xy, r0, c6.z
    mul r3.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r3.xyzz
    add r0.xyw, r0, t3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c3.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r6, r6, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r4.xyz, r4, r8
    mul_sat_pp r1.xyz, r1, r4
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    texld_pp r3, t2, s1
    texld_pp r7, t1, s0
    mul_pp r3.xyz, r3, r7
    mul_pp r2.xyz, r2, r3
    add_pp r2.xyz, r2, r2
    mad_pp r1.xyz, r1, r6.z, r2
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 130 instruction slots used (6 texture, 124 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyComplexFogDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, true, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    dcl_2d s7
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c3.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c3.x, c3.y
    mul r0.xy, r0, c6.x
    mul r0.xy, r0, c6.z
    mul r3.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r3.xyzz
    add r0.xyw, r0, t3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c3.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r6, r6, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r4.xyz, r4, r8
    mul_sat_pp r1.xyz, r1, r4
    mul_pp r1.xyz, r6.z, r1
    texld_pp r8, t2, s1
    texld_pp r9, t1, s0
    mul_pp r8.xyz, r8, r9
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 129 instruction slots used (6 texture, 123 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddComplexFogDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, true, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 2, -1, 1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl t4.xyz
    dcl t5.xyz
    dcl t6.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s5
    dcl_2d s7
    add r0.xyz, -t0, c12
    nrm r1.xyz, r0
    dp3 r1.w, r1, -c13
    mov r2, c14
    mad_sat r1.w, r1.w, c13.w, r2.w
    mov r0.w, c12.w
    dp4 r2.w, r0, r0
    mov r0.z, c3.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c3.x, c3.y
    mul r0.xy, r0, c6.x
    mul r0.xy, r0, c6.z
    mul r3.xyz, r0.y, t4
    mad r0.xyw, t5.xyzz, r0.x, r3.xyzz
    add r0.xyw, r0, t3.xyzz
    dp3_sat r3.x, r0.xyww, r1
    mul r2.w, r2.w, r3.x
    mul r3.x, r1.w, r2.w
    add r4.xyz, -t0, c15
    nrm r5.xyz, r4
    dp3 r1.w, r5, -c16
    mov r2.w, c17.w
    mad_sat r1.w, r1.w, c16.w, r2.w
    mov r4.w, c15.w
    dp4 r2.w, r4, r4
    mad r2.w, r2.w, -c15.w, r0.z
    dp3_sat r5.w, r0.xyww, r5
    mul r2.w, r2.w, r5.w
    mul r3.y, r1.w, r2.w
    max r4.xy, r3, t2.z
    min r3.xy, t2.w, r4
    dp3 r1.w, -r5, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, -r5
    add r5.xyz, -t0, c8
    nrm r6.xyz, r5
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c3.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c19
    dp3 r1.w, -c18, r0.xyww
    mul r2.w, r1.w, c9.z
    max r7.x, r1.w, r2.w
    dp3 r7.y, -c20, r0.xyww
    max r3.zw, r7.xyxy, t2.z
    min r7.xy, t2.w, r3.zwzw
    mad_pp r1.xyz, r4, r7.x, r1
    dp3 r1.w, c20, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c20
    dp3 r1.w, r4, r6
    dp3 r2.w, r0.xyww, r6
    mov r4, c10
    mad_pp r4, r2.w, r4, c11
    max r2.w, r1.w, c3.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r8, t1, s7
    texld_pp r6, r6, s2
    log r9.x, r8.x
    log r9.y, r8.y
    log r9.z, r8.z
    mul r8.xyz, r9, c5.z
    exp r9.x, r8.x
    exp r9.y, r8.y
    exp r9.z, r8.z
    mul_sat_pp r8.xyz, r9, c5.w
    mul_pp r4.xyz, r4, r8
    mul_sat_pp r1.xyz, r1, r4
    mul_pp r1.xyz, r6.z, r1
    texld_pp r8, t2, s1
    texld_pp r9, t1, s0
    mad_pp r8.xyz, r8, c3.x, r9
    add_pp r8.xyz, r8, c3.y
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r1.xyz, r8, r2, r1
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mul_pp r0.w, r4.w, r6.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 130 instruction slots used (6 texture, 124 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplySpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, false, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c12.w
    mad r1.w, r0.w, -r1.w, c0.x
    dp3_sat r0.x, t3, r1
    dp3 r0.y, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.y, r0.y, r0.w, c14.w
    mul r0.x, r0.x, r1.w
    mul r0.x, r0.y, r0.x
    mov r1.w, c15.w
    add r1.xyz, -t0, c15
    dp4 r1.w, r1, r1
    nrm r2.xyz, r1
    mov r2.w, c15.w
    mad r2.w, r1.w, -r2.w, c0.x
    dp3_sat r0.z, t3, r2
    dp3 r0.w, r2, -c16
    mov r1.w, c16.w
    mad_sat r0.w, r0.w, r1.w, c17.w
    mul r0.z, r0.z, r2.w
    mul r0.y, r0.w, r0.z
    max r1.xy, r0, t2.z
    min r0.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r0.xzw, r0.x, r1.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    dp3 r0.w, -c18, t3
    mul r1.x, r0.w, c9.z
    max r2.x, r0.w, r1.x
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xyz, r2.x, c19, r0
    mad_pp r0.xyz, r2.y, c21, r0
    add r1.xyz, -t0, c8
    dp3 r0.w, r1, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r1
    texld_pp r3, t1, s7
    texld_pp r2, r2, s3
    log r4.x, r3.x
    log r4.y, r3.y
    log r4.z, r3.z
    mul r3.xyz, r4, c5.z
    exp r4.x, r3.x
    exp r4.y, r3.y
    exp r4.z, r3.z
    mul_sat_pp r3.xyz, r4, c5.w
    nrm r4.xyz, r1
    dp3 r0.w, t3, r4
    mov r1, c10
    mad_pp r1, r0.w, r1, c11
    mul_pp r1.xyz, r3, r1
    mul_pp r1.xyz, r1, r2
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mul_pp r0.w, r1.w, r2.z
    mul_pp r1.xyz, r0.w, r1
    mad_pp r0.xyz, r3, r0, r1
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    add_pp r2.xyz, r0, r0
    mov_pp oC0, r2

// approximately 73 instruction slots used (5 texture, 68 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplySpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, false, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c12.w
    mad r1.w, r0.w, -r1.w, c0.x
    dp3_sat r0.x, t3, r1
    dp3 r0.y, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.y, r0.y, r0.w, c14.w
    mul r0.x, r0.x, r1.w
    mul r0.x, r0.y, r0.x
    mov r1.w, c15.w
    add r1.xyz, -t0, c15
    dp4 r1.w, r1, r1
    nrm r2.xyz, r1
    mov r2.w, c15.w
    mad r2.w, r1.w, -r2.w, c0.x
    dp3_sat r0.z, t3, r2
    dp3 r0.w, r2, -c16
    mov r1.w, c16.w
    mad_sat r0.w, r0.w, r1.w, c17.w
    mul r0.z, r0.z, r2.w
    mul r0.y, r0.w, r0.z
    max r1.xy, r0, t2.z
    min r0.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r0.xzw, r0.x, r1.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    dp3 r0.w, -c18, t3
    mul r1.x, r0.w, c9.z
    max r2.x, r0.w, r1.x
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xyz, r2.x, c19, r0
    mad_pp r0.xyz, r2.y, c21, r0
    add r1.xyz, -t0, c8
    dp3 r0.w, r1, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r1
    texld_pp r3, t1, s7
    texld_pp r2, r2, s3
    log r4.x, r3.x
    log r4.y, r3.y
    log r4.z, r3.z
    mul r3.xyz, r4, c5.z
    exp r4.x, r3.x
    exp r4.y, r3.y
    exp r4.z, r3.z
    mul_sat_pp r3.xyz, r4, c5.w
    nrm r4.xyz, r1
    dp3 r0.w, t3, r4
    mov r1, c10
    mad_pp r1, r0.w, r1, c11
    mul_pp r1.xyz, r3, r1
    mul_pp r1.xyz, r1, r2
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mul_pp r0.w, r1.w, r2.z
    mul_pp r1.xyz, r0.w, r1
    mad_pp r0.xyz, r3, r0, r1
    texld_pp r1, t2, s1
    mul_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 72 instruction slots used (5 texture, 67 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, false, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 1, 2, -1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    dp3 r0.z, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.z, r0.z, r0.w, c14.w
    mul r0.x, r0.y, r0.x
    mul r0.x, r0.z, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r1.xyz, r2
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r0.z, t3, r1
    dp3 r0.w, r1, -c16
    mov r2.w, c16.w
    mad_sat r0.w, r0.w, r2.w, c17.w
    mul r0.z, r0.z, r1.w
    mul r0.y, r0.w, r0.z
    max r1.xy, r0, t2.z
    min r0.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r0.xzw, r0.x, r1.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    dp3 r0.w, -c18, t3
    mul r1.x, r0.w, c9.z
    max r2.x, r0.w, r1.x
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xyz, r2.x, c19, r0
    mad_pp r0.xyz, r2.y, c21, r0
    add r1.xyz, -t0, c8
    dp3 r0.w, r1, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r1
    texld_pp r3, t1, s7
    texld_pp r2, r2, s3
    log r4.x, r3.x
    log r4.y, r3.y
    log r4.z, r3.z
    mul r3.xyz, r4, c5.z
    exp r4.x, r3.x
    exp r4.y, r3.y
    exp r4.z, r3.z
    mul_sat_pp r3.xyz, r4, c5.w
    nrm r4.xyz, r1
    dp3 r0.w, t3, r4
    mov r1, c10
    mad_pp r1, r0.w, r1, c11
    mul_pp r1.xyz, r3, r1
    mul_pp r1.xyz, r1, r2
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mul_pp r0.w, r1.w, r2.z
    mul_pp r1.xyz, r0.w, r1
    mad_pp r0.xyz, r3, r0, r1
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c0.y, r0
    add_pp r2.xyz, r0, c0.z
    mov_pp oC0, r2

// approximately 72 instruction slots used (5 texture, 67 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplySpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, false, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c12.w
    mad r1.w, r0.w, -r1.w, c0.x
    dp3_sat r0.x, t3, r1
    dp3 r0.y, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.y, r0.y, r0.w, c14.w
    mul r0.x, r0.x, r1.w
    mul r0.x, r0.y, r0.x
    mov r1.w, c15.w
    add r1.xyz, -t0, c15
    dp4 r1.w, r1, r1
    nrm r2.xyz, r1
    mov r2.w, c15.w
    mad r2.w, r1.w, -r2.w, c0.x
    dp3_sat r0.z, t3, r2
    dp3 r0.w, r2, -c16
    mov r1.w, c16.w
    mad_sat r0.w, r0.w, r1.w, c17.w
    mul r0.z, r0.z, r2.w
    mul r0.y, r0.w, r0.z
    max r1.xy, r0, t2.z
    min r0.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r0.xzw, r0.x, r1.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    dp3 r0.w, -c18, t3
    mul r1.x, r0.w, c9.z
    max r2.x, r0.w, r1.x
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xyz, r2.x, c19, r0
    mad_pp r0.xyz, r2.y, c21, r0
    texld_pp r1, t2, s1
    texld_pp r2, t1, s0
    mul_pp r1.xyz, r1, r2
    mul_pp r0.xyz, r0, r1
    add_pp r0.xyz, r0, r0
    add r1.xyz, -t0, c8
    dp3 r0.w, r1, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r1
    texld_pp r3, t1, s7
    texld_pp r2, r2, s3
    log r4.x, r3.x
    log r4.y, r3.y
    log r4.z, r3.z
    mul r3.xyz, r4, c5.z
    exp r4.x, r3.x
    exp r4.y, r3.y
    exp r4.z, r3.z
    mul_sat_pp r3.xyz, r4, c5.w
    nrm r4.xyz, r1
    dp3 r0.w, t3, r4
    mov r1, c10
    mad_pp r1, r0.w, r1, c11
    mul_pp r1.xyz, r3, r1
    mul_pp r1.xyz, r1, r2
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mul_pp r0.w, r1.w, r2.z
    mad_pp r2.xyz, r1, r0.w, r0
    mov_pp oC0, r2

// approximately 73 instruction slots used (5 texture, 68 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplySpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, false, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c12.w
    mad r1.w, r0.w, -r1.w, c0.x
    dp3_sat r0.x, t3, r1
    dp3 r0.y, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.y, r0.y, r0.w, c14.w
    mul r0.x, r0.x, r1.w
    mul r0.x, r0.y, r0.x
    mov r1.w, c15.w
    add r1.xyz, -t0, c15
    dp4 r1.w, r1, r1
    nrm r2.xyz, r1
    mov r2.w, c15.w
    mad r2.w, r1.w, -r2.w, c0.x
    dp3_sat r0.z, t3, r2
    dp3 r0.w, r2, -c16
    mov r1.w, c16.w
    mad_sat r0.w, r0.w, r1.w, c17.w
    mul r0.z, r0.z, r2.w
    mul r0.y, r0.w, r0.z
    max r1.xy, r0, t2.z
    min r0.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r0.xzw, r0.x, r1.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    dp3 r0.w, -c18, t3
    mul r1.x, r0.w, c9.z
    max r2.x, r0.w, r1.x
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xyz, r2.x, c19, r0
    mad_pp r0.xyz, r2.y, c21, r0
    add r1.xyz, -t0, c8
    dp3 r0.w, r1, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r1
    texld_pp r3, t1, s7
    texld_pp r2, r2, s3
    log r4.x, r3.x
    log r4.y, r3.y
    log r4.z, r3.z
    mul r3.xyz, r4, c5.z
    exp r4.x, r3.x
    exp r4.y, r3.y
    exp r4.z, r3.z
    mul_sat_pp r3.xyz, r4, c5.w
    nrm r4.xyz, r1
    dp3 r0.w, t3, r4
    mov r1, c10
    mad_pp r1, r0.w, r1, c11
    mul_pp r1.xyz, r3, r1
    mul_pp r1.xyz, r1, r2
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mul_pp r0.w, r1.w, r2.z
    mul_pp r1.xyz, r0.w, r1
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mul_pp r3.xyz, r3, r4
    mad_pp r2.xyz, r3, r0, r1
    mov_pp oC0, r2

// approximately 72 instruction slots used (5 texture, 67 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, false, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 1, 2, -1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    dp3 r0.z, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.z, r0.z, r0.w, c14.w
    mul r0.x, r0.y, r0.x
    mul r0.x, r0.z, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r1.xyz, r2
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r0.z, t3, r1
    dp3 r0.w, r1, -c16
    mov r2.w, c16.w
    mad_sat r0.w, r0.w, r2.w, c17.w
    mul r0.z, r0.z, r1.w
    mul r0.y, r0.w, r0.z
    max r1.xy, r0, t2.z
    min r0.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r0.xzw, r0.x, r1.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    dp3 r0.w, -c18, t3
    mul r1.x, r0.w, c9.z
    max r2.x, r0.w, r1.x
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xyz, r2.x, c19, r0
    mad_pp r0.xyz, r2.y, c21, r0
    add r1.xyz, -t0, c8
    dp3 r0.w, r1, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r1
    texld_pp r3, t1, s7
    texld_pp r2, r2, s3
    log r4.x, r3.x
    log r4.y, r3.y
    log r4.z, r3.z
    mul r3.xyz, r4, c5.z
    exp r4.x, r3.x
    exp r4.y, r3.y
    exp r4.z, r3.z
    mul_sat_pp r3.xyz, r4, c5.w
    nrm r4.xyz, r1
    dp3 r0.w, t3, r4
    mov r1, c10
    mad_pp r1, r0.w, r1, c11
    mul_pp r1.xyz, r3, r1
    mul_pp r1.xyz, r1, r2
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mul_pp r0.w, r1.w, r2.z
    mul_pp r1.xyz, r0.w, r1
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r3.xyz, r3, c0.y, r4
    add_pp r3.xyz, r3, c0.z
    mad_pp r2.xyz, r3, r0, r1
    mov_pp oC0, r2

// approximately 72 instruction slots used (5 texture, 67 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyComplexFogSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, true, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 1, 2, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c12.w
    mad r1.w, r0.w, -r1.w, c3.x
    dp3_sat r0.x, t3, r1
    dp3 r0.y, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.y, r0.y, r0.w, c14.w
    mul r0.x, r0.x, r1.w
    mul r0.x, r0.y, r0.x
    mov r1.w, c15.w
    add r1.xyz, -t0, c15
    dp4 r1.w, r1, r1
    nrm r2.xyz, r1
    mov r2.w, c15.w
    mad r2.w, r1.w, -r2.w, c3.x
    dp3_sat r0.z, t3, r2
    dp3 r0.w, r2, -c16
    mov r1.w, c16.w
    mad_sat r0.w, r0.w, r1.w, c17.w
    mul r0.z, r0.z, r2.w
    mul r0.y, r0.w, r0.z
    max r1.xy, r0, t2.z
    min r0.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r0.xzw, r0.x, r1.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    dp3 r0.w, -c18, t3
    mul r1.x, r0.w, c9.z
    max r2.x, r0.w, r1.x
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xyz, r2.x, c19, r0
    mad_pp r0.xyz, r2.y, c21, r0
    add r1.xyz, -t0, c8
    dp3 r0.w, r1, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r1
    texld_pp r3, t1, s7
    texld_pp r2, r2, s3
    log r4.x, r3.x
    log r4.y, r3.y
    log r4.z, r3.z
    mul r3.xyz, r4, c5.z
    exp r4.x, r3.x
    exp r4.y, r3.y
    exp r4.z, r3.z
    mul_sat_pp r3.xyz, r4, c5.w
    nrm r4.xyz, r1
    dp3 r0.w, t3, r4
    mov r1, c10
    mad_pp r1, r0.w, r1, c11
    mul_pp r1.xyz, r3, r1
    mul_pp r1.xyz, r1, r2
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mul_pp r0.w, r1.w, r2.z
    mul_pp r1.xyz, r0.w, r1
    mad_pp r0.xyz, r3, r0, r1
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    mul_pp r0.xyz, r0, c0.w
    add r1.xyz, r0, r0
    mov r3.xyz, c0
    mad r0.xyz, r0, -c3.y, r3
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 80 instruction slots used (5 texture, 75 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyComplexFogSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, true, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c12.w
    mad r1.w, r0.w, -r1.w, c3.x
    dp3_sat r0.x, t3, r1
    dp3 r0.y, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.y, r0.y, r0.w, c14.w
    mul r0.x, r0.x, r1.w
    mul r0.x, r0.y, r0.x
    mov r1.w, c15.w
    add r1.xyz, -t0, c15
    dp4 r1.w, r1, r1
    nrm r2.xyz, r1
    mov r2.w, c15.w
    mad r2.w, r1.w, -r2.w, c3.x
    dp3_sat r0.z, t3, r2
    dp3 r0.w, r2, -c16
    mov r1.w, c16.w
    mad_sat r0.w, r0.w, r1.w, c17.w
    mul r0.z, r0.z, r2.w
    mul r0.y, r0.w, r0.z
    max r1.xy, r0, t2.z
    min r0.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r0.xzw, r0.x, r1.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    dp3 r0.w, -c18, t3
    mul r1.x, r0.w, c9.z
    max r2.x, r0.w, r1.x
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xyz, r2.x, c19, r0
    mad_pp r0.xyz, r2.y, c21, r0
    add r1.xyz, -t0, c8
    dp3 r0.w, r1, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r1
    texld_pp r3, t1, s7
    texld_pp r2, r2, s3
    log r4.x, r3.x
    log r4.y, r3.y
    log r4.z, r3.z
    mul r3.xyz, r4, c5.z
    exp r4.x, r3.x
    exp r4.y, r3.y
    exp r4.z, r3.z
    mul_sat_pp r3.xyz, r4, c5.w
    nrm r4.xyz, r1
    dp3 r0.w, t3, r4
    mov r1, c10
    mad_pp r1, r0.w, r1, c11
    mul_pp r1.xyz, r3, r1
    mul_pp r1.xyz, r1, r2
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mul_pp r0.w, r1.w, r2.z
    mul_pp r1.xyz, r0.w, r1
    mad_pp r0.xyz, r3, r0, r1
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 78 instruction slots used (5 texture, 73 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddComplexFogSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, true, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 1, 2, -1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    dp3 r0.z, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.z, r0.z, r0.w, c14.w
    mul r0.x, r0.y, r0.x
    mul r0.x, r0.z, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r1.xyz, r2
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r0.z, t3, r1
    dp3 r0.w, r1, -c16
    mov r2.w, c16.w
    mad_sat r0.w, r0.w, r2.w, c17.w
    mul r0.z, r0.z, r1.w
    mul r0.y, r0.w, r0.z
    max r1.xy, r0, t2.z
    min r0.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r0.xzw, r0.x, r1.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    dp3 r0.w, -c18, t3
    mul r1.x, r0.w, c9.z
    max r2.x, r0.w, r1.x
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xyz, r2.x, c19, r0
    mad_pp r0.xyz, r2.y, c21, r0
    add r1.xyz, -t0, c8
    dp3 r0.w, r1, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r1
    texld_pp r3, t1, s7
    texld_pp r2, r2, s3
    log r4.x, r3.x
    log r4.y, r3.y
    log r4.z, r3.z
    mul r3.xyz, r4, c5.z
    exp r4.x, r3.x
    exp r4.y, r3.y
    exp r4.z, r3.z
    mul_sat_pp r3.xyz, r4, c5.w
    nrm r4.xyz, r1
    dp3 r0.w, t3, r4
    mov r1, c10
    mad_pp r1, r0.w, r1, c11
    mul_pp r1.xyz, r3, r1
    mul_pp r1.xyz, r1, r2
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mul_pp r0.w, r1.w, r2.z
    mul_pp r1.xyz, r0.w, r1
    mad_pp r0.xyz, r3, r0, r1
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c3.y, r0
    add_pp r0.xyz, r0, c3.z
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 78 instruction slots used (5 texture, 73 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyComplexFogSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, true, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c12.w
    mad r1.w, r0.w, -r1.w, c3.x
    dp3_sat r0.x, t3, r1
    dp3 r0.y, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.y, r0.y, r0.w, c14.w
    mul r0.x, r0.x, r1.w
    mul r0.x, r0.y, r0.x
    mov r1.w, c15.w
    add r1.xyz, -t0, c15
    dp4 r1.w, r1, r1
    nrm r2.xyz, r1
    mov r2.w, c15.w
    mad r2.w, r1.w, -r2.w, c3.x
    dp3_sat r0.z, t3, r2
    dp3 r0.w, r2, -c16
    mov r1.w, c16.w
    mad_sat r0.w, r0.w, r1.w, c17.w
    mul r0.z, r0.z, r2.w
    mul r0.y, r0.w, r0.z
    max r1.xy, r0, t2.z
    min r0.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r0.xzw, r0.x, r1.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    dp3 r0.w, -c18, t3
    mul r1.x, r0.w, c9.z
    max r2.x, r0.w, r1.x
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xyz, r2.x, c19, r0
    mad_pp r0.xyz, r2.y, c21, r0
    texld_pp r1, t2, s1
    texld_pp r2, t1, s0
    mul_pp r1.xyz, r1, r2
    mul_pp r0.xyz, r0, r1
    add_pp r0.xyz, r0, r0
    add r1.xyz, -t0, c8
    dp3 r0.w, r1, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r1
    texld_pp r3, t1, s7
    texld_pp r2, r2, s3
    log r4.x, r3.x
    log r4.y, r3.y
    log r4.z, r3.z
    mul r3.xyz, r4, c5.z
    exp r4.x, r3.x
    exp r4.y, r3.y
    exp r4.z, r3.z
    mul_sat_pp r3.xyz, r4, c5.w
    nrm r4.xyz, r1
    dp3 r0.w, t3, r4
    mov r1, c10
    mad_pp r1, r0.w, r1, c11
    mul_pp r1.xyz, r3, r1
    mul_pp r1.xyz, r1, r2
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mul_pp r0.w, r1.w, r2.z
    mad_pp r0.xyz, r1, r0.w, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 79 instruction slots used (5 texture, 74 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyComplexFogSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, true, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c12.w
    mad r1.w, r0.w, -r1.w, c3.x
    dp3_sat r0.x, t3, r1
    dp3 r0.y, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.y, r0.y, r0.w, c14.w
    mul r0.x, r0.x, r1.w
    mul r0.x, r0.y, r0.x
    mov r1.w, c15.w
    add r1.xyz, -t0, c15
    dp4 r1.w, r1, r1
    nrm r2.xyz, r1
    mov r2.w, c15.w
    mad r2.w, r1.w, -r2.w, c3.x
    dp3_sat r0.z, t3, r2
    dp3 r0.w, r2, -c16
    mov r1.w, c16.w
    mad_sat r0.w, r0.w, r1.w, c17.w
    mul r0.z, r0.z, r2.w
    mul r0.y, r0.w, r0.z
    max r1.xy, r0, t2.z
    min r0.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r0.xzw, r0.x, r1.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    dp3 r0.w, -c18, t3
    mul r1.x, r0.w, c9.z
    max r2.x, r0.w, r1.x
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xyz, r2.x, c19, r0
    mad_pp r0.xyz, r2.y, c21, r0
    add r1.xyz, -t0, c8
    dp3 r0.w, r1, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r1
    texld_pp r3, t1, s7
    texld_pp r2, r2, s3
    log r4.x, r3.x
    log r4.y, r3.y
    log r4.z, r3.z
    mul r3.xyz, r4, c5.z
    exp r4.x, r3.x
    exp r4.y, r3.y
    exp r4.z, r3.z
    mul_sat_pp r3.xyz, r4, c5.w
    nrm r4.xyz, r1
    dp3 r0.w, t3, r4
    mov r1, c10
    mad_pp r1, r0.w, r1, c11
    mul_pp r1.xyz, r3, r1
    mul_pp r1.xyz, r1, r2
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mul_pp r0.w, r1.w, r2.z
    mul_pp r1.xyz, r0.w, r1
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mul_pp r3.xyz, r3, r4
    mad_pp r0.xyz, r3, r0, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 78 instruction slots used (5 texture, 73 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddComplexFogSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, true, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 1, 2, -1, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    dp3 r0.z, r1, -c13
    mov r0.w, c13.w
    mad_sat r0.z, r0.z, r0.w, c14.w
    mul r0.x, r0.y, r0.x
    mul r0.x, r0.z, r0.x
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r1.xyz, r2
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r0.z, t3, r1
    dp3 r0.w, r1, -c16
    mov r2.w, c16.w
    mad_sat r0.w, r0.w, r2.w, c17.w
    mul r0.z, r0.z, r1.w
    mul r0.y, r0.w, r0.z
    max r1.xy, r0, t2.z
    min r0.xy, t2.w, r1
    mov r1.xyz, c14
    mad_pp r0.xzw, r0.x, r1.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    dp3 r0.w, -c18, t3
    mul r1.x, r0.w, c9.z
    max r2.x, r0.w, r1.x
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xyz, r2.x, c19, r0
    mad_pp r0.xyz, r2.y, c21, r0
    add r1.xyz, -t0, c8
    dp3 r0.w, r1, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r1
    texld_pp r3, t1, s7
    texld_pp r2, r2, s3
    log r4.x, r3.x
    log r4.y, r3.y
    log r4.z, r3.z
    mul r3.xyz, r4, c5.z
    exp r4.x, r3.x
    exp r4.y, r3.y
    exp r4.z, r3.z
    mul_sat_pp r3.xyz, r4, c5.w
    nrm r4.xyz, r1
    dp3 r0.w, t3, r4
    mov r1, c10
    mad_pp r1, r0.w, r1, c11
    mul_pp r1.xyz, r3, r1
    mul_pp r1.xyz, r1, r2
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mul_pp r0.w, r1.w, r2.z
    mul_pp r1.xyz, r0.w, r1
    texld_pp r3, t2, s1
    texld_pp r4, t1, s0
    mad_pp r3.xyz, r3, c3.y, r4
    add_pp r3.xyz, r3, c3.z
    mad_pp r0.xyz, r3, r0, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 78 instruction slots used (5 texture, 73 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplySpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, false, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    mul r0.x, r0.y, r0.x
    dp3 r0.y, r1, -c13
    mov r2, c14
    mad_sat r2.w, r0.y, c13.w, r2.w
    mul r0.x, r0.x, r2.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r2.w, t3, r4
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r4, -c16
    mov r4.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r4.w
    mul r0.y, r1.w, r2.w
    max r3.xy, r0, t2.z
    min r0.xy, t2.w, r3
    dp3 r1.w, -r4, t3
    add r1.w, r1.w, r1.w
    mad r3.xyz, t3, -r1.w, -r4
    add r4.xyz, -t0, c8
    nrm r5.xyz, r4
    dp3 r1.w, r3, r5
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c0.y
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r0.x, r3
    dp3 r1.w, -c18, t3
    mul r2.w, r1.w, c9.z
    max r3.x, r1.w, r2.w
    dp3 r3.y, -c20, t3
    max r0.zw, r3.xyxy, t2.z
    min r3.xy, t2.w, r0.zwzw
    dp3 r1.w, c18, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c18
    dp3 r1.w, r6, r5
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c19
    mad_pp r1.xyz, r6, r3.x, r1
    dp3 r1.w, c20, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c20
    dp3 r1.w, r6, r5
    dp3 r2.w, t3, r5
    mov r5, c10
    mad_pp r5, r2.w, r5, c11
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r7, t1, s7
    texld_pp r6, r6, s2
    log r8.x, r7.x
    log r8.y, r7.y
    log r8.z, r7.z
    mul r7.xyz, r8, c5.z
    exp r8.x, r7.x
    exp r8.y, r7.y
    exp r8.z, r7.z
    mul_sat_pp r7.xyz, r8, c5.w
    mul_pp r5.xyz, r5, r7
    mul_sat_pp r1.xyz, r1, r5
    mul_pp r1.xyz, r6.z, r1
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    dp3 r0.w, r4, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r4
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.xyz, r5, r2
    mul_pp r0.w, r5.w, r6.z
    mad_pp r0.xyz, r1, r0.w, r0
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    add_pp r6.xyz, r0, r0
    mov_pp oC0, r6

// approximately 117 instruction slots used (5 texture, 112 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplySpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, false, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    mul r0.x, r0.y, r0.x
    dp3 r0.y, r1, -c13
    mov r2, c14
    mad_sat r2.w, r0.y, c13.w, r2.w
    mul r0.x, r0.x, r2.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r2.w, t3, r4
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r4, -c16
    mov r4.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r4.w
    mul r0.y, r1.w, r2.w
    max r3.xy, r0, t2.z
    min r0.xy, t2.w, r3
    dp3 r1.w, -r4, t3
    add r1.w, r1.w, r1.w
    mad r3.xyz, t3, -r1.w, -r4
    add r4.xyz, -t0, c8
    nrm r5.xyz, r4
    dp3 r1.w, r3, r5
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c0.y
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r0.x, r3
    dp3 r1.w, -c18, t3
    mul r2.w, r1.w, c9.z
    max r3.x, r1.w, r2.w
    dp3 r3.y, -c20, t3
    max r0.zw, r3.xyxy, t2.z
    min r3.xy, t2.w, r0.zwzw
    dp3 r1.w, c18, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c18
    dp3 r1.w, r6, r5
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c19
    mad_pp r1.xyz, r6, r3.x, r1
    dp3 r1.w, c20, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c20
    dp3 r1.w, r6, r5
    dp3 r2.w, t3, r5
    mov r5, c10
    mad_pp r5, r2.w, r5, c11
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r7, t1, s7
    texld_pp r6, r6, s2
    log r8.x, r7.x
    log r8.y, r7.y
    log r8.z, r7.z
    mul r7.xyz, r8, c5.z
    exp r8.x, r7.x
    exp r8.y, r7.y
    exp r8.z, r7.z
    mul_sat_pp r7.xyz, r8, c5.w
    mul_pp r5.xyz, r5, r7
    mul_sat_pp r1.xyz, r1, r5
    mul_pp r1.xyz, r6.z, r1
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    dp3 r0.w, r4, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r4
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.xyz, r5, r2
    mul_pp r0.w, r5.w, r6.z
    mad_pp r0.xyz, r1, r0.w, r0
    texld_pp r1, t2, s1
    mul_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 116 instruction slots used (5 texture, 111 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, false, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 1, 0, 2, -1
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    mul r0.x, r0.y, r0.x
    dp3 r0.y, r1, -c13
    mov r2, c14
    mad_sat r2.w, r0.y, c13.w, r2.w
    mul r0.x, r0.x, r2.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r2.w, t3, r4
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r4, -c16
    mov r4.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r4.w
    mul r0.y, r1.w, r2.w
    max r3.xy, r0, t2.z
    min r0.xy, t2.w, r3
    dp3 r1.w, -r4, t3
    add r1.w, r1.w, r1.w
    mad r3.xyz, t3, -r1.w, -r4
    add r4.xyz, -t0, c8
    nrm r5.xyz, r4
    dp3 r1.w, r3, r5
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c0.y
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r0.x, r3
    dp3 r1.w, -c18, t3
    mul r2.w, r1.w, c9.z
    max r3.x, r1.w, r2.w
    dp3 r3.y, -c20, t3
    max r0.zw, r3.xyxy, t2.z
    min r3.xy, t2.w, r0.zwzw
    dp3 r1.w, c18, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c18
    dp3 r1.w, r6, r5
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c19
    mad_pp r1.xyz, r6, r3.x, r1
    dp3 r1.w, c20, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c20
    dp3 r1.w, r6, r5
    dp3 r2.w, t3, r5
    mov r5, c10
    mad_pp r5, r2.w, r5, c11
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r7, t1, s7
    texld_pp r6, r6, s2
    log r8.x, r7.x
    log r8.y, r7.y
    log r8.z, r7.z
    mul r7.xyz, r8, c5.z
    exp r8.x, r7.x
    exp r8.y, r7.y
    exp r8.z, r7.z
    mul_sat_pp r7.xyz, r8, c5.w
    mul_pp r5.xyz, r5, r7
    mul_sat_pp r1.xyz, r1, r5
    mul_pp r1.xyz, r6.z, r1
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    dp3 r0.w, r4, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r4
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.xyz, r5, r2
    mul_pp r0.w, r5.w, r6.z
    mad_pp r0.xyz, r1, r0.w, r0
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c0.z, r0
    add_pp r6.xyz, r0, c0.w
    mov_pp oC0, r6

// approximately 117 instruction slots used (5 texture, 112 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplySpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, false, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    mul r0.x, r0.y, r0.x
    dp3 r0.y, r1, -c13
    mov r2, c14
    mad_sat r2.w, r0.y, c13.w, r2.w
    mul r0.x, r0.x, r2.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r2.w, t3, r4
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r4, -c16
    mov r4.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r4.w
    mul r0.y, r1.w, r2.w
    max r3.xy, r0, t2.z
    min r0.xy, t2.w, r3
    dp3 r1.w, -r4, t3
    add r1.w, r1.w, r1.w
    mad r3.xyz, t3, -r1.w, -r4
    add r4.xyz, -t0, c8
    nrm r5.xyz, r4
    dp3 r1.w, r3, r5
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c0.y
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r0.x, r3
    dp3 r1.w, -c18, t3
    mul r2.w, r1.w, c9.z
    max r3.x, r1.w, r2.w
    dp3 r3.y, -c20, t3
    max r0.zw, r3.xyxy, t2.z
    min r3.xy, t2.w, r0.zwzw
    dp3 r1.w, c18, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c18
    dp3 r1.w, r6, r5
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c19
    mad_pp r1.xyz, r6, r3.x, r1
    dp3 r1.w, c20, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c20
    dp3 r1.w, r6, r5
    dp3 r2.w, t3, r5
    mov r5, c10
    mad_pp r5, r2.w, r5, c11
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r7, t1, s7
    texld_pp r6, r6, s2
    log r8.x, r7.x
    log r8.y, r7.y
    log r8.z, r7.z
    mul r7.xyz, r8, c5.z
    exp r8.x, r7.x
    exp r8.y, r7.y
    exp r8.z, r7.z
    mul_sat_pp r7.xyz, r8, c5.w
    mul_pp r5.xyz, r5, r7
    mul_sat_pp r1.xyz, r1, r5
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mul_pp r2.xyz, r2, r3
    mul_pp r0.xyz, r0, r2
    add_pp r0.xyz, r0, r0
    mad_pp r0.xyz, r1, r6.z, r0
    dp3 r0.w, r4, t3
    mul r1.xyz, r0.w, t3
    mad_pp r1.xyz, r1, c8.w, -r4
    texld_pp r1, r1, s3
    mul_pp r1.xyz, r5, r1
    mul_pp r0.w, r5.w, r6.z
    mad_pp r6.xyz, r1, r0.w, r0
    mov_pp oC0, r6

// approximately 117 instruction slots used (5 texture, 112 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplySpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, false, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    mul r0.x, r0.y, r0.x
    dp3 r0.y, r1, -c13
    mov r2, c14
    mad_sat r2.w, r0.y, c13.w, r2.w
    mul r0.x, r0.x, r2.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r2.w, t3, r4
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r4, -c16
    mov r4.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r4.w
    mul r0.y, r1.w, r2.w
    max r3.xy, r0, t2.z
    min r0.xy, t2.w, r3
    dp3 r1.w, -r4, t3
    add r1.w, r1.w, r1.w
    mad r3.xyz, t3, -r1.w, -r4
    add r4.xyz, -t0, c8
    nrm r5.xyz, r4
    dp3 r1.w, r3, r5
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c0.y
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r0.x, r3
    dp3 r1.w, -c18, t3
    mul r2.w, r1.w, c9.z
    max r3.x, r1.w, r2.w
    dp3 r3.y, -c20, t3
    max r0.zw, r3.xyxy, t2.z
    min r3.xy, t2.w, r0.zwzw
    dp3 r1.w, c18, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c18
    dp3 r1.w, r6, r5
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c19
    mad_pp r1.xyz, r6, r3.x, r1
    dp3 r1.w, c20, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c20
    dp3 r1.w, r6, r5
    dp3 r2.w, t3, r5
    mov r5, c10
    mad_pp r5, r2.w, r5, c11
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r7, t1, s7
    texld_pp r6, r6, s2
    log r8.x, r7.x
    log r8.y, r7.y
    log r8.z, r7.z
    mul r7.xyz, r8, c5.z
    exp r8.x, r7.x
    exp r8.y, r7.y
    exp r8.z, r7.z
    mul_sat_pp r7.xyz, r8, c5.w
    mul_pp r5.xyz, r5, r7
    mul_sat_pp r1.xyz, r1, r5
    mul_pp r1.xyz, r6.z, r1
    texld_pp r7, t2, s1
    texld_pp r8, t1, s0
    mul_pp r7.xyz, r7, r8
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    mad_pp r0.xyz, r7, r0, r1
    dp3 r0.w, r4, t3
    mul r1.xyz, r0.w, t3
    mad_pp r1.xyz, r1, c8.w, -r4
    texld_pp r1, r1, s3
    mul_pp r1.xyz, r5, r1
    mul_pp r0.w, r5.w, r6.z
    mad_pp r6.xyz, r1, r0.w, r0
    mov_pp oC0, r6

// approximately 116 instruction slots used (5 texture, 111 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, false, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 1, 0, 2, -1
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    mul r0.x, r0.y, r0.x
    dp3 r0.y, r1, -c13
    mov r2, c14
    mad_sat r2.w, r0.y, c13.w, r2.w
    mul r0.x, r0.x, r2.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r2.w, t3, r4
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r4, -c16
    mov r4.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r4.w
    mul r0.y, r1.w, r2.w
    max r3.xy, r0, t2.z
    min r0.xy, t2.w, r3
    dp3 r1.w, -r4, t3
    add r1.w, r1.w, r1.w
    mad r3.xyz, t3, -r1.w, -r4
    add r4.xyz, -t0, c8
    nrm r5.xyz, r4
    dp3 r1.w, r3, r5
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c0.y
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r0.x, r3
    dp3 r1.w, -c18, t3
    mul r2.w, r1.w, c9.z
    max r3.x, r1.w, r2.w
    dp3 r3.y, -c20, t3
    max r0.zw, r3.xyxy, t2.z
    min r3.xy, t2.w, r0.zwzw
    dp3 r1.w, c18, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c18
    dp3 r1.w, r6, r5
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c19
    mad_pp r1.xyz, r6, r3.x, r1
    dp3 r1.w, c20, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c20
    dp3 r1.w, r6, r5
    dp3 r2.w, t3, r5
    mov r5, c10
    mad_pp r5, r2.w, r5, c11
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r7, t1, s7
    texld_pp r6, r6, s2
    log r8.x, r7.x
    log r8.y, r7.y
    log r8.z, r7.z
    mul r7.xyz, r8, c5.z
    exp r8.x, r7.x
    exp r8.y, r7.y
    exp r8.z, r7.z
    mul_sat_pp r7.xyz, r8, c5.w
    mul_pp r5.xyz, r5, r7
    mul_sat_pp r1.xyz, r1, r5
    mul_pp r1.xyz, r6.z, r1
    texld_pp r7, t2, s1
    texld_pp r8, t1, s0
    mad_pp r7.xyz, r7, c0.z, r8
    add_pp r7.xyz, r7, c0.w
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    mad_pp r0.xyz, r7, r0, r1
    dp3 r0.w, r4, t3
    mul r1.xyz, r0.w, t3
    mad_pp r1.xyz, r1, c8.w, -r4
    texld_pp r1, r1, s3
    mul_pp r1.xyz, r5, r1
    mul_pp r0.w, r5.w, r6.z
    mad_pp r6.xyz, r1, r0.w, r0
    mov_pp oC0, r6

// approximately 117 instruction slots used (5 texture, 112 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyComplexFogSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, true, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 1, 0, 2, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r0.xz, c3
    mad r1.w, r0.w, -c12.w, r0.x
    dp3_sat r0.y, t3, r1
    mul r1.w, r0.y, r1.w
    dp3 r0.y, r1, -c13
    mov r2, c14
    mad_sat r2.w, r0.y, c13.w, r2.w
    mul r3.x, r1.w, r2.w
    mov r4.w, c15.w
    add r4.xyz, -t0, c15
    dp4 r1.w, r4, r4
    nrm r5.xyz, r4
    mad r1.w, r1.w, -c15.w, r0.x
    dp3_sat r2.w, t3, r5
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r5, -c16
    mov r5.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r5.w
    mul r3.y, r1.w, r2.w
    max r0.xy, r3, t2.z
    min r3.xy, t2.w, r0
    dp3 r1.w, -r5, t3
    add r1.w, r1.w, r1.w
    mad r0.xyw, t3.xyzz, -r1.w, -r5.xyzz
    add r4.xyz, -t0, c8
    nrm r5.xyz, r4
    dp3 r1.w, r0.xyww, r5
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r0.xyw, r1.w, c17.xyzz
    mul r0.xyw, r3.y, r0
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r4.w, r2.w, c3.y
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r0.xyw, r1.xyzz, r3.x, r0
    dp3 r2.w, -c18, t3
    mul r4.w, r2.w, c9.z
    max r1.x, r2.w, r4.w
    dp3 r1.y, -c20, t3
    max r3.zw, r1.xyxy, t2.z
    min r1.xy, t2.w, r3.zwzw
    dp3 r2.w, c18, t3
    add r2.w, r2.w, r2.w
    mad r6.xyz, t3, -r2.w, c18
    dp3 r2.w, r6, r5
    max r4.w, r2.w, c3.y
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r6.xyz, r2.w, c19
    mad_pp r0.xyw, r6.xyzz, r1.x, r0
    dp3 r2.w, c20, t3
    add r2.w, r2.w, r2.w
    mad r6.xyz, t3, -r2.w, c20
    dp3 r2.w, r6, r5
    dp3 r4.w, t3, r5
    mov r5, c10
    mad_pp r5, r4.w, r5, c11
    max r4.w, r2.w, c3.y
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r6.xyz, r2.w, c21
    mad_pp r0.xyw, r6.xyzz, r1.y, r0
    mov r6.xy, t1.zwzw
    texld_pp r7, t1, s7
    texld_pp r6, r6, s2
    log r8.x, r7.x
    log r8.y, r7.y
    log r8.z, r7.z
    mul r7.xyz, r8, c5.z
    exp r8.x, r7.x
    exp r8.y, r7.y
    exp r8.z, r7.z
    mul_sat_pp r7.xyz, r8, c5.w
    mul_pp r5.xyz, r5, r7
    mul_sat_pp r0.xyw, r0, r5.xyzz
    mul_pp r0.xyw, r6.z, r0
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r1.xzw, r1.x, c19.xyyz, r2.xyyz
    mad_pp r1.xyz, r1.y, c21, r1.xzww
    dp3 r1.w, r4, t3
    mul r2.xyz, r1.w, t3
    mad_pp r2.xyz, r2, c8.w, -r4
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mad_pp r0.xyw, r3.xyzz, r1.xyzz, r0
    mul_pp r1.xyz, r5, r2
    mul_pp r1.w, r5.w, r6.z
    mad_pp r0.xyw, r1.xyzz, r1.w, r0
    texld_pp r1, t2, s1
    mul_pp r0.xyw, r0, r1.xyzz
    mul_pp r0.xyw, r0, c0.w
    add r1.xyz, r0.xyww, r0.xyww
    mad r0.xyz, r0.xyww, -r0.z, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 123 instruction slots used (5 texture, 118 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyComplexFogSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, true, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    mul r0.x, r0.y, r0.x
    dp3 r0.y, r1, -c13
    mov r2, c14
    mad_sat r2.w, r0.y, c13.w, r2.w
    mul r0.x, r0.x, r2.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r2.w, t3, r4
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r4, -c16
    mov r4.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r4.w
    mul r0.y, r1.w, r2.w
    max r3.xy, r0, t2.z
    min r0.xy, t2.w, r3
    dp3 r1.w, -r4, t3
    add r1.w, r1.w, r1.w
    mad r3.xyz, t3, -r1.w, -r4
    add r4.xyz, -t0, c8
    nrm r5.xyz, r4
    dp3 r1.w, r3, r5
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c3.y
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r0.x, r3
    dp3 r1.w, -c18, t3
    mul r2.w, r1.w, c9.z
    max r3.x, r1.w, r2.w
    dp3 r3.y, -c20, t3
    max r0.zw, r3.xyxy, t2.z
    min r3.xy, t2.w, r0.zwzw
    dp3 r1.w, c18, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c18
    dp3 r1.w, r6, r5
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c19
    mad_pp r1.xyz, r6, r3.x, r1
    dp3 r1.w, c20, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c20
    dp3 r1.w, r6, r5
    dp3 r2.w, t3, r5
    mov r5, c10
    mad_pp r5, r2.w, r5, c11
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r7, t1, s7
    texld_pp r6, r6, s2
    log r8.x, r7.x
    log r8.y, r7.y
    log r8.z, r7.z
    mul r7.xyz, r8, c5.z
    exp r8.x, r7.x
    exp r8.y, r7.y
    exp r8.z, r7.z
    mul_sat_pp r7.xyz, r8, c5.w
    mul_pp r5.xyz, r5, r7
    mul_sat_pp r1.xyz, r1, r5
    mul_pp r1.xyz, r6.z, r1
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    dp3 r0.w, r4, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r4
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.xyz, r5, r2
    mul_pp r0.w, r5.w, r6.z
    mad_pp r0.xyz, r1, r0.w, r0
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 122 instruction slots used (5 texture, 117 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddComplexFogSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, true, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 1, 0, 2, -1
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    mul r0.x, r0.y, r0.x
    dp3 r0.y, r1, -c13
    mov r2, c14
    mad_sat r2.w, r0.y, c13.w, r2.w
    mul r0.x, r0.x, r2.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r2.w, t3, r4
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r4, -c16
    mov r4.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r4.w
    mul r0.y, r1.w, r2.w
    max r3.xy, r0, t2.z
    min r0.xy, t2.w, r3
    dp3 r1.w, -r4, t3
    add r1.w, r1.w, r1.w
    mad r3.xyz, t3, -r1.w, -r4
    add r4.xyz, -t0, c8
    nrm r5.xyz, r4
    dp3 r1.w, r3, r5
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c3.y
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r0.x, r3
    dp3 r1.w, -c18, t3
    mul r2.w, r1.w, c9.z
    max r3.x, r1.w, r2.w
    dp3 r3.y, -c20, t3
    max r0.zw, r3.xyxy, t2.z
    min r3.xy, t2.w, r0.zwzw
    dp3 r1.w, c18, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c18
    dp3 r1.w, r6, r5
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c19
    mad_pp r1.xyz, r6, r3.x, r1
    dp3 r1.w, c20, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c20
    dp3 r1.w, r6, r5
    dp3 r2.w, t3, r5
    mov r5, c10
    mad_pp r5, r2.w, r5, c11
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r7, t1, s7
    texld_pp r6, r6, s2
    log r8.x, r7.x
    log r8.y, r7.y
    log r8.z, r7.z
    mul r7.xyz, r8, c5.z
    exp r8.x, r7.x
    exp r8.y, r7.y
    exp r8.z, r7.z
    mul_sat_pp r7.xyz, r8, c5.w
    mul_pp r5.xyz, r5, r7
    mul_sat_pp r1.xyz, r1, r5
    mul_pp r1.xyz, r6.z, r1
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    dp3 r0.w, r4, t3
    mul r2.xyz, r0.w, t3
    mad_pp r2.xyz, r2, c8.w, -r4
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.xyz, r5, r2
    mul_pp r0.w, r5.w, r6.z
    mad_pp r0.xyz, r1, r0.w, r0
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c3.z, r0
    add_pp r0.xyz, r0, c3.w
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 123 instruction slots used (5 texture, 118 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyComplexFogSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, true, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    mul r0.x, r0.y, r0.x
    dp3 r0.y, r1, -c13
    mov r2, c14
    mad_sat r2.w, r0.y, c13.w, r2.w
    mul r0.x, r0.x, r2.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r2.w, t3, r4
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r4, -c16
    mov r4.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r4.w
    mul r0.y, r1.w, r2.w
    max r3.xy, r0, t2.z
    min r0.xy, t2.w, r3
    dp3 r1.w, -r4, t3
    add r1.w, r1.w, r1.w
    mad r3.xyz, t3, -r1.w, -r4
    add r4.xyz, -t0, c8
    nrm r5.xyz, r4
    dp3 r1.w, r3, r5
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c3.y
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r0.x, r3
    dp3 r1.w, -c18, t3
    mul r2.w, r1.w, c9.z
    max r3.x, r1.w, r2.w
    dp3 r3.y, -c20, t3
    max r0.zw, r3.xyxy, t2.z
    min r3.xy, t2.w, r0.zwzw
    dp3 r1.w, c18, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c18
    dp3 r1.w, r6, r5
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c19
    mad_pp r1.xyz, r6, r3.x, r1
    dp3 r1.w, c20, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c20
    dp3 r1.w, r6, r5
    dp3 r2.w, t3, r5
    mov r5, c10
    mad_pp r5, r2.w, r5, c11
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r7, t1, s7
    texld_pp r6, r6, s2
    log r8.x, r7.x
    log r8.y, r7.y
    log r8.z, r7.z
    mul r7.xyz, r8, c5.z
    exp r8.x, r7.x
    exp r8.y, r7.y
    exp r8.z, r7.z
    mul_sat_pp r7.xyz, r8, c5.w
    mul_pp r5.xyz, r5, r7
    mul_sat_pp r1.xyz, r1, r5
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mul_pp r2.xyz, r2, r3
    mul_pp r0.xyz, r0, r2
    add_pp r0.xyz, r0, r0
    mad_pp r0.xyz, r1, r6.z, r0
    dp3 r0.w, r4, t3
    mul r1.xyz, r0.w, t3
    mad_pp r1.xyz, r1, c8.w, -r4
    texld_pp r1, r1, s3
    mul_pp r1.xyz, r5, r1
    mul_pp r0.w, r5.w, r6.z
    mad_pp r0.xyz, r1, r0.w, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 123 instruction slots used (5 texture, 118 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyComplexFogSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, true, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    mul r0.x, r0.y, r0.x
    dp3 r0.y, r1, -c13
    mov r2, c14
    mad_sat r2.w, r0.y, c13.w, r2.w
    mul r0.x, r0.x, r2.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r2.w, t3, r4
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r4, -c16
    mov r4.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r4.w
    mul r0.y, r1.w, r2.w
    max r3.xy, r0, t2.z
    min r0.xy, t2.w, r3
    dp3 r1.w, -r4, t3
    add r1.w, r1.w, r1.w
    mad r3.xyz, t3, -r1.w, -r4
    add r4.xyz, -t0, c8
    nrm r5.xyz, r4
    dp3 r1.w, r3, r5
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c3.y
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r0.x, r3
    dp3 r1.w, -c18, t3
    mul r2.w, r1.w, c9.z
    max r3.x, r1.w, r2.w
    dp3 r3.y, -c20, t3
    max r0.zw, r3.xyxy, t2.z
    min r3.xy, t2.w, r0.zwzw
    dp3 r1.w, c18, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c18
    dp3 r1.w, r6, r5
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c19
    mad_pp r1.xyz, r6, r3.x, r1
    dp3 r1.w, c20, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c20
    dp3 r1.w, r6, r5
    dp3 r2.w, t3, r5
    mov r5, c10
    mad_pp r5, r2.w, r5, c11
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r7, t1, s7
    texld_pp r6, r6, s2
    log r8.x, r7.x
    log r8.y, r7.y
    log r8.z, r7.z
    mul r7.xyz, r8, c5.z
    exp r8.x, r7.x
    exp r8.y, r7.y
    exp r8.z, r7.z
    mul_sat_pp r7.xyz, r8, c5.w
    mul_pp r5.xyz, r5, r7
    mul_sat_pp r1.xyz, r1, r5
    mul_pp r1.xyz, r6.z, r1
    texld_pp r7, t2, s1
    texld_pp r8, t1, s0
    mul_pp r7.xyz, r7, r8
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    mad_pp r0.xyz, r7, r0, r1
    dp3 r0.w, r4, t3
    mul r1.xyz, r0.w, t3
    mad_pp r1.xyz, r1, c8.w, -r4
    texld_pp r1, r1, s3
    mul_pp r1.xyz, r5, r1
    mul_pp r0.w, r5.w, r6.z
    mad_pp r0.xyz, r1, r0.w, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 122 instruction slots used (5 texture, 117 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddComplexFogSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, true, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 1, 0, 2, -1
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_2d s7
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    mul r0.x, r0.y, r0.x
    dp3 r0.y, r1, -c13
    mov r2, c14
    mad_sat r2.w, r0.y, c13.w, r2.w
    mul r0.x, r0.x, r2.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r2.w, t3, r4
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r4, -c16
    mov r4.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r4.w
    mul r0.y, r1.w, r2.w
    max r3.xy, r0, t2.z
    min r0.xy, t2.w, r3
    dp3 r1.w, -r4, t3
    add r1.w, r1.w, r1.w
    mad r3.xyz, t3, -r1.w, -r4
    add r4.xyz, -t0, c8
    nrm r5.xyz, r4
    dp3 r1.w, r3, r5
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c3.y
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r0.x, r3
    dp3 r1.w, -c18, t3
    mul r2.w, r1.w, c9.z
    max r3.x, r1.w, r2.w
    dp3 r3.y, -c20, t3
    max r0.zw, r3.xyxy, t2.z
    min r3.xy, t2.w, r0.zwzw
    dp3 r1.w, c18, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c18
    dp3 r1.w, r6, r5
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c19
    mad_pp r1.xyz, r6, r3.x, r1
    dp3 r1.w, c20, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c20
    dp3 r1.w, r6, r5
    dp3 r2.w, t3, r5
    mov r5, c10
    mad_pp r5, r2.w, r5, c11
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mov r6.xy, t1.zwzw
    texld_pp r7, t1, s7
    texld_pp r6, r6, s2
    log r8.x, r7.x
    log r8.y, r7.y
    log r8.z, r7.z
    mul r7.xyz, r8, c5.z
    exp r8.x, r7.x
    exp r8.y, r7.y
    exp r8.z, r7.z
    mul_sat_pp r7.xyz, r8, c5.w
    mul_pp r5.xyz, r5, r7
    mul_sat_pp r1.xyz, r1, r5
    mul_pp r1.xyz, r6.z, r1
    texld_pp r7, t2, s1
    texld_pp r8, t1, s0
    mad_pp r7.xyz, r7, c3.z, r8
    add_pp r7.xyz, r7, c3.w
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    mad_pp r0.xyz, r7, r0, r1
    dp3 r0.w, r4, t3
    mul r1.xyz, r0.w, t3
    mad_pp r1.xyz, r1, c8.w, -r4
    texld_pp r1, r1, s3
    mul_pp r1.xyz, r5, r1
    mul_pp r0.w, r5.w, r6.z
    mad_pp r0.xyz, r1, r0.w, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 123 instruction slots used (5 texture, 118 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplySpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, false, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    mul r0.x, r0.y, r0.x
    dp3 r0.y, r1, -c13
    mov r2, c14
    mad_sat r2.w, r0.y, c13.w, r2.w
    mul r0.x, r0.x, r2.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r2.w, t3, r4
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r4, -c16
    mov r4.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r4.w
    mul r0.y, r1.w, r2.w
    max r3.xy, r0, t2.z
    min r0.xy, t2.w, r3
    dp3 r1.w, -r4, t3
    add r1.w, r1.w, r1.w
    mad r3.xyz, t3, -r1.w, -r4
    add r4.xyz, -t0, c8
    nrm r5.xyz, r4
    dp3 r1.w, r3, r5
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c0.y
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r0.x, r3
    dp3 r1.w, -c18, t3
    mul r2.w, r1.w, c9.z
    max r3.x, r1.w, r2.w
    dp3 r3.y, -c20, t3
    max r0.zw, r3.xyxy, t2.z
    min r3.xy, t2.w, r0.zwzw
    dp3 r1.w, c18, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c18
    dp3 r1.w, r6, r5
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c19
    mad_pp r1.xyz, r6, r3.x, r1
    dp3 r1.w, c20, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c20
    dp3 r1.w, r6, r5
    dp3 r2.w, t3, r5
    mov r5, c10
    mad_pp r5, r2.w, r5, c11
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mul_sat_pp r1.xyz, r5, r1
    mov r6.xy, t1.zwzw
    texld_pp r7, t1, s0
    texld_pp r6, r6, s2
    mul_pp r1.xyz, r1, r6.z
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    mad_pp r0.xyz, r7, r0, r1
    dp3 r0.w, r4, t3
    mul r1.xyz, r0.w, t3
    mad_pp r1.xyz, r1, c8.w, -r4
    texld_pp r2, t2, s1
    texld_pp r1, r1, s3
    mul_pp r1.xyz, r5, r1
    mul_pp r0.w, r5.w, r6.z
    mad_pp r0.xyz, r1, r0.w, r0
    mul_pp r0.xyz, r2, r0
    add_pp r6.xyz, r0, r0
    mov_pp oC0, r6

// approximately 107 instruction slots used (4 texture, 103 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplySpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, false, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    mul r0.x, r0.y, r0.x
    dp3 r0.y, r1, -c13
    mov r2, c14
    mad_sat r2.w, r0.y, c13.w, r2.w
    mul r0.x, r0.x, r2.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r2.w, t3, r4
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r4, -c16
    mov r4.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r4.w
    mul r0.y, r1.w, r2.w
    max r3.xy, r0, t2.z
    min r0.xy, t2.w, r3
    dp3 r1.w, -r4, t3
    add r1.w, r1.w, r1.w
    mad r3.xyz, t3, -r1.w, -r4
    add r4.xyz, -t0, c8
    nrm r5.xyz, r4
    dp3 r1.w, r3, r5
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c0.y
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r0.x, r3
    dp3 r1.w, -c18, t3
    mul r2.w, r1.w, c9.z
    max r3.x, r1.w, r2.w
    dp3 r3.y, -c20, t3
    max r0.zw, r3.xyxy, t2.z
    min r3.xy, t2.w, r0.zwzw
    dp3 r1.w, c18, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c18
    dp3 r1.w, r6, r5
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c19
    mad_pp r1.xyz, r6, r3.x, r1
    dp3 r1.w, c20, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c20
    dp3 r1.w, r6, r5
    dp3 r2.w, t3, r5
    mov r5, c10
    mad_pp r5, r2.w, r5, c11
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mul_sat_pp r1.xyz, r5, r1
    mov r6.xy, t1.zwzw
    texld_pp r7, t1, s0
    texld_pp r6, r6, s2
    mul_pp r1.xyz, r1, r6.z
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    mad_pp r0.xyz, r7, r0, r1
    dp3 r0.w, r4, t3
    mul r1.xyz, r0.w, t3
    mad_pp r1.xyz, r1, c8.w, -r4
    texld_pp r2, t2, s1
    texld_pp r1, r1, s3
    mul_pp r1.xyz, r5, r1
    mul_pp r0.w, r5.w, r6.z
    mad_pp r0.xyz, r1, r0.w, r0
    mul_pp r6.xyz, r2, r0
    mov_pp oC0, r6

// approximately 106 instruction slots used (4 texture, 102 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, false, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 1, 0, 2, -1
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    mul r0.x, r0.y, r0.x
    dp3 r0.y, r1, -c13
    mov r2, c14
    mad_sat r2.w, r0.y, c13.w, r2.w
    mul r0.x, r0.x, r2.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r2.w, t3, r4
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r4, -c16
    mov r4.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r4.w
    mul r0.y, r1.w, r2.w
    max r3.xy, r0, t2.z
    min r0.xy, t2.w, r3
    dp3 r1.w, -r4, t3
    add r1.w, r1.w, r1.w
    mad r3.xyz, t3, -r1.w, -r4
    add r4.xyz, -t0, c8
    nrm r5.xyz, r4
    dp3 r1.w, r3, r5
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c0.y
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r0.x, r3
    dp3 r1.w, -c18, t3
    mul r2.w, r1.w, c9.z
    max r3.x, r1.w, r2.w
    dp3 r3.y, -c20, t3
    max r0.zw, r3.xyxy, t2.z
    min r3.xy, t2.w, r0.zwzw
    dp3 r1.w, c18, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c18
    dp3 r1.w, r6, r5
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c19
    mad_pp r1.xyz, r6, r3.x, r1
    dp3 r1.w, c20, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c20
    dp3 r1.w, r6, r5
    dp3 r2.w, t3, r5
    mov r5, c10
    mad_pp r5, r2.w, r5, c11
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mul_sat_pp r1.xyz, r5, r1
    mov r6.xy, t1.zwzw
    texld_pp r7, t1, s0
    texld_pp r6, r6, s2
    mul_pp r1.xyz, r1, r6.z
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    mad_pp r0.xyz, r7, r0, r1
    dp3 r0.w, r4, t3
    mul r1.xyz, r0.w, t3
    mad_pp r1.xyz, r1, c8.w, -r4
    texld_pp r2, t2, s1
    texld_pp r1, r1, s3
    mul_pp r1.xyz, r5, r1
    mul_pp r0.w, r5.w, r6.z
    mad_pp r0.xyz, r1, r0.w, r0
    mad_pp r0.xyz, r2, c0.z, r0
    add_pp r6.xyz, r0, c0.w
    mov_pp oC0, r6

// approximately 107 instruction slots used (4 texture, 103 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplySpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, false, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    mul r0.x, r0.y, r0.x
    dp3 r0.y, r1, -c13
    mov r2, c14
    mad_sat r2.w, r0.y, c13.w, r2.w
    mul r0.x, r0.x, r2.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r2.w, t3, r4
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r4, -c16
    mov r4.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r4.w
    mul r0.y, r1.w, r2.w
    max r3.xy, r0, t2.z
    min r0.xy, t2.w, r3
    dp3 r1.w, -r4, t3
    add r1.w, r1.w, r1.w
    mad r3.xyz, t3, -r1.w, -r4
    add r4.xyz, -t0, c8
    nrm r5.xyz, r4
    dp3 r1.w, r3, r5
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c0.y
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r0.x, r3
    dp3 r1.w, -c18, t3
    mul r2.w, r1.w, c9.z
    max r3.x, r1.w, r2.w
    dp3 r3.y, -c20, t3
    max r0.zw, r3.xyxy, t2.z
    min r3.xy, t2.w, r0.zwzw
    dp3 r1.w, c18, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c18
    dp3 r1.w, r6, r5
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c19
    mad_pp r1.xyz, r6, r3.x, r1
    dp3 r1.w, c20, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c20
    dp3 r1.w, r6, r5
    dp3 r2.w, t3, r5
    mov r5, c10
    mad_pp r5, r2.w, r5, c11
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mul_sat_pp r1.xyz, r5, r1
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mul_pp r2.xyz, r2, r3
    mul_pp r0.xyz, r0, r2
    add_pp r0.xyz, r0, r0
    mov r2.xy, t1.zwzw
    dp3 r0.w, r4, t3
    mul r3.xyz, r0.w, t3
    mad_pp r3.xyz, r3, c8.w, -r4
    texld_pp r2, r2, s2
    texld_pp r3, r3, s3
    mad_pp r0.xyz, r1, r2.z, r0
    mul_pp r1.xyz, r5, r3
    mul_pp r0.w, r5.w, r2.z
    mad_pp r2.xyz, r1, r0.w, r0
    mov_pp oC0, r2

// approximately 107 instruction slots used (4 texture, 103 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplySpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, false, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    mul r0.x, r0.y, r0.x
    dp3 r0.y, r1, -c13
    mov r2, c14
    mad_sat r2.w, r0.y, c13.w, r2.w
    mul r0.x, r0.x, r2.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r2.w, t3, r4
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r4, -c16
    mov r4.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r4.w
    mul r0.y, r1.w, r2.w
    max r3.xy, r0, t2.z
    min r0.xy, t2.w, r3
    dp3 r1.w, -r4, t3
    add r1.w, r1.w, r1.w
    mad r3.xyz, t3, -r1.w, -r4
    add r4.xyz, -t0, c8
    nrm r5.xyz, r4
    dp3 r1.w, r3, r5
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c0.y
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r0.x, r3
    dp3 r1.w, -c18, t3
    mul r2.w, r1.w, c9.z
    max r3.x, r1.w, r2.w
    dp3 r3.y, -c20, t3
    max r0.zw, r3.xyxy, t2.z
    min r3.xy, t2.w, r0.zwzw
    dp3 r1.w, c18, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c18
    dp3 r1.w, r6, r5
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c19
    mad_pp r1.xyz, r6, r3.x, r1
    dp3 r1.w, c20, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c20
    dp3 r1.w, r6, r5
    dp3 r2.w, t3, r5
    mov r5, c10
    mad_pp r5, r2.w, r5, c11
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mul_sat_pp r1.xyz, r5, r1
    mov r6.xy, t1.zwzw
    dp3 r1.w, r4, t3
    mul r7.xyz, r1.w, t3
    mad_pp r4.xyz, r7, c8.w, -r4
    texld_pp r6, r6, s2
    texld_pp r4, r4, s3
    mul_pp r1.xyz, r1, r6.z
    texld_pp r7, t2, s1
    texld_pp r8, t1, s0
    mul_pp r7.xyz, r7, r8
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    mad_pp r0.xyz, r7, r0, r1
    mul_pp r1.xyz, r5, r4
    mul_pp r0.w, r5.w, r6.z
    mad_pp r6.xyz, r1, r0.w, r0
    mov_pp oC0, r6

// approximately 106 instruction slots used (4 texture, 102 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, false, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c0, 1, 0, 2, -1
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c0.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    mul r0.x, r0.y, r0.x
    dp3 r0.y, r1, -c13
    mov r2, c14
    mad_sat r2.w, r0.y, c13.w, r2.w
    mul r0.x, r0.x, r2.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r2.w, t3, r4
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r4, -c16
    mov r4.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r4.w
    mul r0.y, r1.w, r2.w
    max r3.xy, r0, t2.z
    min r0.xy, t2.w, r3
    dp3 r1.w, -r4, t3
    add r1.w, r1.w, r1.w
    mad r3.xyz, t3, -r1.w, -r4
    add r4.xyz, -t0, c8
    nrm r5.xyz, r4
    dp3 r1.w, r3, r5
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c0.y
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r0.x, r3
    dp3 r1.w, -c18, t3
    mul r2.w, r1.w, c9.z
    max r3.x, r1.w, r2.w
    dp3 r3.y, -c20, t3
    max r0.zw, r3.xyxy, t2.z
    min r3.xy, t2.w, r0.zwzw
    dp3 r1.w, c18, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c18
    dp3 r1.w, r6, r5
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c19
    mad_pp r1.xyz, r6, r3.x, r1
    dp3 r1.w, c20, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c20
    dp3 r1.w, r6, r5
    dp3 r2.w, t3, r5
    mov r5, c10
    mad_pp r5, r2.w, r5, c11
    max r2.w, r1.w, c0.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mul_sat_pp r1.xyz, r5, r1
    mov r6.xy, t1.zwzw
    dp3 r1.w, r4, t3
    mul r7.xyz, r1.w, t3
    mad_pp r4.xyz, r7, c8.w, -r4
    texld_pp r6, r6, s2
    texld_pp r4, r4, s3
    mul_pp r1.xyz, r1, r6.z
    texld_pp r7, t2, s1
    texld_pp r8, t1, s0
    mad_pp r7.xyz, r7, c0.z, r8
    add_pp r7.xyz, r7, c0.w
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    mad_pp r0.xyz, r7, r0, r1
    mul_pp r1.xyz, r5, r4
    mul_pp r0.w, r5.w, r6.z
    mad_pp r6.xyz, r1, r0.w, r0
    mov_pp oC0, r6

// approximately 107 instruction slots used (4 texture, 103 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedMultiplyComplexFogSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, true, true, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 1, 0, 2, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r0.xz, c3
    mad r1.w, r0.w, -c12.w, r0.x
    dp3_sat r0.y, t3, r1
    mul r1.w, r0.y, r1.w
    dp3 r0.y, r1, -c13
    mov r2, c14
    mad_sat r2.w, r0.y, c13.w, r2.w
    mul r3.x, r1.w, r2.w
    mov r4.w, c15.w
    add r4.xyz, -t0, c15
    dp4 r1.w, r4, r4
    nrm r5.xyz, r4
    mad r1.w, r1.w, -c15.w, r0.x
    dp3_sat r2.w, t3, r5
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r5, -c16
    mov r5.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r5.w
    mul r3.y, r1.w, r2.w
    max r0.xy, r3, t2.z
    min r3.xy, t2.w, r0
    dp3 r1.w, -r5, t3
    add r1.w, r1.w, r1.w
    mad r0.xyw, t3.xyzz, -r1.w, -r5.xyzz
    add r4.xyz, -t0, c8
    nrm r5.xyz, r4
    dp3 r1.w, r0.xyww, r5
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r0.xyw, r1.w, c17.xyzz
    mul r0.xyw, r3.y, r0
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r4.w, r2.w, c3.y
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r0.xyw, r1.xyzz, r3.x, r0
    dp3 r2.w, -c18, t3
    mul r4.w, r2.w, c9.z
    max r1.x, r2.w, r4.w
    dp3 r1.y, -c20, t3
    max r3.zw, r1.xyxy, t2.z
    min r1.xy, t2.w, r3.zwzw
    dp3 r2.w, c18, t3
    add r2.w, r2.w, r2.w
    mad r6.xyz, t3, -r2.w, c18
    dp3 r2.w, r6, r5
    max r4.w, r2.w, c3.y
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r6.xyz, r2.w, c19
    mad_pp r0.xyw, r6.xyzz, r1.x, r0
    dp3 r2.w, c20, t3
    add r2.w, r2.w, r2.w
    mad r6.xyz, t3, -r2.w, c20
    dp3 r2.w, r6, r5
    dp3 r4.w, t3, r5
    mov r5, c10
    mad_pp r5, r4.w, r5, c11
    max r4.w, r2.w, c3.y
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r6.xyz, r2.w, c21
    mad_pp r0.xyw, r6.xyzz, r1.y, r0
    mul_sat_pp r0.xyw, r5.xyzz, r0
    mov r6.xy, t1.zwzw
    texld_pp r7, t1, s0
    texld_pp r6, r6, s2
    mul_pp r0.xyw, r0, r6.z
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r1.xzw, r1.x, c19.xyyz, r2.xyyz
    mad_pp r1.xyz, r1.y, c21, r1.xzww
    mad_pp r0.xyw, r7.xyzz, r1.xyzz, r0
    dp3 r4.w, r4, t3
    mul r1.xyz, r4.w, t3
    mad_pp r1.xyz, r1, c8.w, -r4
    texld_pp r2, t2, s1
    texld_pp r1, r1, s3
    mul_pp r1.xyz, r5, r1
    mul_pp r1.w, r5.w, r6.z
    mad_pp r0.xyw, r1.xyzz, r1.w, r0
    mul_pp r0.xyw, r2.xyzz, r0
    mul_pp r0.xyw, r0, c0.w
    add r1.xyz, r0.xyww, r0.xyww
    mad r0.xyz, r0.xyww, -r0.z, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 113 instruction slots used (4 texture, 109 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionMultiplyComplexFogSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, true, true, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    mul r0.x, r0.y, r0.x
    dp3 r0.y, r1, -c13
    mov r2, c14
    mad_sat r2.w, r0.y, c13.w, r2.w
    mul r0.x, r0.x, r2.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r2.w, t3, r4
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r4, -c16
    mov r4.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r4.w
    mul r0.y, r1.w, r2.w
    max r3.xy, r0, t2.z
    min r0.xy, t2.w, r3
    dp3 r1.w, -r4, t3
    add r1.w, r1.w, r1.w
    mad r3.xyz, t3, -r1.w, -r4
    add r4.xyz, -t0, c8
    nrm r5.xyz, r4
    dp3 r1.w, r3, r5
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c3.y
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r0.x, r3
    dp3 r1.w, -c18, t3
    mul r2.w, r1.w, c9.z
    max r3.x, r1.w, r2.w
    dp3 r3.y, -c20, t3
    max r0.zw, r3.xyxy, t2.z
    min r3.xy, t2.w, r0.zwzw
    dp3 r1.w, c18, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c18
    dp3 r1.w, r6, r5
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c19
    mad_pp r1.xyz, r6, r3.x, r1
    dp3 r1.w, c20, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c20
    dp3 r1.w, r6, r5
    dp3 r2.w, t3, r5
    mov r5, c10
    mad_pp r5, r2.w, r5, c11
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mul_sat_pp r1.xyz, r5, r1
    mov r6.xy, t1.zwzw
    texld_pp r7, t1, s0
    texld_pp r6, r6, s2
    mul_pp r1.xyz, r1, r6.z
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    mad_pp r0.xyz, r7, r0, r1
    dp3 r0.w, r4, t3
    mul r1.xyz, r0.w, t3
    mad_pp r1.xyz, r1, c8.w, -r4
    texld_pp r2, t2, s1
    texld_pp r1, r1, s3
    mul_pp r1.xyz, r5, r1
    mul_pp r0.w, r5.w, r6.z
    mad_pp r0.xyz, r1, r0.w, r0
    mul_pp r0.xyz, r2, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 112 instruction slots used (4 texture, 108 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailBeforeReflectionBiasedAddComplexFogSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, true, true, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 1, 0, 2, -1
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    mul r0.x, r0.y, r0.x
    dp3 r0.y, r1, -c13
    mov r2, c14
    mad_sat r2.w, r0.y, c13.w, r2.w
    mul r0.x, r0.x, r2.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r2.w, t3, r4
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r4, -c16
    mov r4.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r4.w
    mul r0.y, r1.w, r2.w
    max r3.xy, r0, t2.z
    min r0.xy, t2.w, r3
    dp3 r1.w, -r4, t3
    add r1.w, r1.w, r1.w
    mad r3.xyz, t3, -r1.w, -r4
    add r4.xyz, -t0, c8
    nrm r5.xyz, r4
    dp3 r1.w, r3, r5
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c3.y
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r0.x, r3
    dp3 r1.w, -c18, t3
    mul r2.w, r1.w, c9.z
    max r3.x, r1.w, r2.w
    dp3 r3.y, -c20, t3
    max r0.zw, r3.xyxy, t2.z
    min r3.xy, t2.w, r0.zwzw
    dp3 r1.w, c18, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c18
    dp3 r1.w, r6, r5
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c19
    mad_pp r1.xyz, r6, r3.x, r1
    dp3 r1.w, c20, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c20
    dp3 r1.w, r6, r5
    dp3 r2.w, t3, r5
    mov r5, c10
    mad_pp r5, r2.w, r5, c11
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mul_sat_pp r1.xyz, r5, r1
    mov r6.xy, t1.zwzw
    texld_pp r7, t1, s0
    texld_pp r6, r6, s2
    mul_pp r1.xyz, r1, r6.z
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    mad_pp r0.xyz, r7, r0, r1
    dp3 r0.w, r4, t3
    mul r1.xyz, r0.w, t3
    mad_pp r1.xyz, r1, c8.w, -r4
    texld_pp r2, t2, s1
    texld_pp r1, r1, s3
    mul_pp r1.xyz, r5, r1
    mul_pp r0.w, r5.w, r6.z
    mad_pp r0.xyz, r1, r0.w, r0
    mad_pp r0.xyz, r2, c3.z, r0
    add_pp r0.xyz, r0, c3.w
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 113 instruction slots used (4 texture, 109 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedMultiplyComplexFogSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_multiply, false, true, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    mul r0.x, r0.y, r0.x
    dp3 r0.y, r1, -c13
    mov r2, c14
    mad_sat r2.w, r0.y, c13.w, r2.w
    mul r0.x, r0.x, r2.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r2.w, t3, r4
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r4, -c16
    mov r4.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r4.w
    mul r0.y, r1.w, r2.w
    max r3.xy, r0, t2.z
    min r0.xy, t2.w, r3
    dp3 r1.w, -r4, t3
    add r1.w, r1.w, r1.w
    mad r3.xyz, t3, -r1.w, -r4
    add r4.xyz, -t0, c8
    nrm r5.xyz, r4
    dp3 r1.w, r3, r5
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c3.y
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r0.x, r3
    dp3 r1.w, -c18, t3
    mul r2.w, r1.w, c9.z
    max r3.x, r1.w, r2.w
    dp3 r3.y, -c20, t3
    max r0.zw, r3.xyxy, t2.z
    min r3.xy, t2.w, r0.zwzw
    dp3 r1.w, c18, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c18
    dp3 r1.w, r6, r5
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c19
    mad_pp r1.xyz, r6, r3.x, r1
    dp3 r1.w, c20, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c20
    dp3 r1.w, r6, r5
    dp3 r2.w, t3, r5
    mov r5, c10
    mad_pp r5, r2.w, r5, c11
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mul_sat_pp r1.xyz, r5, r1
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mul_pp r2.xyz, r2, r3
    mul_pp r0.xyz, r0, r2
    add_pp r0.xyz, r0, r0
    mov r2.xy, t1.zwzw
    dp3 r0.w, r4, t3
    mul r3.xyz, r0.w, t3
    mad_pp r3.xyz, r3, c8.w, -r4
    texld_pp r2, r2, s2
    texld_pp r3, r3, s3
    mad_pp r0.xyz, r1, r2.z, r0
    mul_pp r1.xyz, r5, r3
    mul_pp r0.w, r5.w, r2.z
    mad_pp r0.xyz, r1, r0.w, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 113 instruction slots used (4 texture, 109 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionMultiplyComplexFogSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_multiply, false, true, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 1, 0, 0, 0
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    mul r0.x, r0.y, r0.x
    dp3 r0.y, r1, -c13
    mov r2, c14
    mad_sat r2.w, r0.y, c13.w, r2.w
    mul r0.x, r0.x, r2.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r2.w, t3, r4
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r4, -c16
    mov r4.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r4.w
    mul r0.y, r1.w, r2.w
    max r3.xy, r0, t2.z
    min r0.xy, t2.w, r3
    dp3 r1.w, -r4, t3
    add r1.w, r1.w, r1.w
    mad r3.xyz, t3, -r1.w, -r4
    add r4.xyz, -t0, c8
    nrm r5.xyz, r4
    dp3 r1.w, r3, r5
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c3.y
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r0.x, r3
    dp3 r1.w, -c18, t3
    mul r2.w, r1.w, c9.z
    max r3.x, r1.w, r2.w
    dp3 r3.y, -c20, t3
    max r0.zw, r3.xyxy, t2.z
    min r3.xy, t2.w, r0.zwzw
    dp3 r1.w, c18, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c18
    dp3 r1.w, r6, r5
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c19
    mad_pp r1.xyz, r6, r3.x, r1
    dp3 r1.w, c20, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c20
    dp3 r1.w, r6, r5
    dp3 r2.w, t3, r5
    mov r5, c10
    mad_pp r5, r2.w, r5, c11
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mul_sat_pp r1.xyz, r5, r1
    mov r6.xy, t1.zwzw
    dp3 r1.w, r4, t3
    mul r7.xyz, r1.w, t3
    mad_pp r4.xyz, r7, c8.w, -r4
    texld_pp r6, r6, s2
    texld_pp r4, r4, s3
    mul_pp r1.xyz, r1, r6.z
    texld_pp r7, t2, s1
    texld_pp r8, t1, s0
    mul_pp r7.xyz, r7, r8
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    mad_pp r0.xyz, r7, r0, r1
    mul_pp r1.xyz, r5, r4
    mul_pp r0.w, r5.w, r6.z
    mad_pp r0.xyz, r1, r0.w, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 112 instruction slots used (4 texture, 108 arithmetic)
		};
#endif
	}
}

Technique EnvironmentNoMaskDetailAfterReflectionBiasedAddComplexFogSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelEnvironmentNoMask(detail_function_biased_add, false, true, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c3, 1, 0, 2, -1
    dcl t0.xyz
    dcl t1
    dcl t2
    dcl t3.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    mov r0.w, c12.w
    add r0.xyz, -t0, c12
    dp4 r0.w, r0, r0
    nrm r1.xyz, r0
    mov r1.w, c3.x
    mad r0.x, r0.w, -c12.w, r1.w
    dp3_sat r0.y, t3, r1
    mul r0.x, r0.y, r0.x
    dp3 r0.y, r1, -c13
    mov r2, c14
    mad_sat r2.w, r0.y, c13.w, r2.w
    mul r0.x, r0.x, r2.w
    mov r3.w, c15.w
    add r3.xyz, -t0, c15
    dp4 r2.w, r3, r3
    nrm r4.xyz, r3
    mad r1.w, r2.w, -c15.w, r1.w
    dp3_sat r2.w, t3, r4
    mul r1.w, r1.w, r2.w
    dp3 r2.w, r4, -c16
    mov r4.w, c17.w
    mad_sat r2.w, r2.w, c16.w, r4.w
    mul r0.y, r1.w, r2.w
    max r3.xy, r0, t2.z
    min r0.xy, t2.w, r3
    dp3 r1.w, -r4, t3
    add r1.w, r1.w, r1.w
    mad r3.xyz, t3, -r1.w, -r4
    add r4.xyz, -t0, c8
    nrm r5.xyz, r4
    dp3 r1.w, r3, r5
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c3.y
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r0.x, r3
    dp3 r1.w, -c18, t3
    mul r2.w, r1.w, c9.z
    max r3.x, r1.w, r2.w
    dp3 r3.y, -c20, t3
    max r0.zw, r3.xyxy, t2.z
    min r3.xy, t2.w, r0.zwzw
    dp3 r1.w, c18, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c18
    dp3 r1.w, r6, r5
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c19
    mad_pp r1.xyz, r6, r3.x, r1
    dp3 r1.w, c20, t3
    add r1.w, r1.w, r1.w
    mad r6.xyz, t3, -r1.w, c20
    dp3 r1.w, r6, r5
    dp3 r2.w, t3, r5
    mov r5, c10
    mad_pp r5, r2.w, r5, c11
    max r2.w, r1.w, c3.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mul_sat_pp r1.xyz, r5, r1
    mov r6.xy, t1.zwzw
    dp3 r1.w, r4, t3
    mul r7.xyz, r1.w, t3
    mad_pp r4.xyz, r7, c8.w, -r4
    texld_pp r6, r6, s2
    texld_pp r4, r4, s3
    mul_pp r1.xyz, r1, r6.z
    texld_pp r7, t2, s1
    texld_pp r8, t1, s0
    mad_pp r7.xyz, r7, c3.z, r8
    add_pp r7.xyz, r7, c3.w
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    mad_pp r0.xyz, r7, r0, r1
    mul_pp r1.xyz, r5, r4
    mul_pp r0.w, r5.w, r6.z
    mad_pp r0.xyz, r1, r0.w, r0
    mul r1.xyz, r0, c0.w
    mad r0.xyz, r0, -c0.w, c0
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c1
    mad_sat_pp r1.xyz, t2.z, -r1, c2
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 113 instruction slots used (4 texture, 109 arithmetic)
		};
#endif
	}
}