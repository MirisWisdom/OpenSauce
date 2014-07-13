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
	float3x3	TBNTranspose 	: TEXCOORD3;
	float4		NormalDetailUV	: TEXCOORD6; 
};

///////////////////////////////////////////////////////////////////////////////
// Pixel Shader 2.0 shaders
///////////////////////////////////////////////////////////////////////////////
half4 ModelNoMask(PS_INPUT IN,
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
	//diffuse lighting + self illumination * self illumination colour
	D0.rgb	= D0 + (T2.g*c_self_illumination_color);

	// combiner 2
	//specular intensity * specular amount
	R1.a	= T2.b*D1.a;
	//change colour * change colour amount + inverse change colour amount
	R0	= (T2.a*c_primary_change_color) + (1-T2.a);

	// combiner 4
	//reflection cube map * specular intensity
	T3.rgb	= T3*D1;
	//lighting * colour change
	D0.rgb	= D0*R0;

	if(bDetailBeforeReflection)
	{
		// combiner 6
		//diffuse * lighting + reflection * specular intensity
		R0.rgb = (T0*D0) + (T3*R1.a);

		// combiner 7
		if(detail_function_biased_multiply==nDetailFunction)
		{
			//(diffuse lit * detail) * 2
			R0.rgb	= (R0*T1)*2;
		}
		else if(detail_function_multiply==nDetailFunction)
		{
			//diffuse lit * detail
			R0.rgb	= R0*T1;
		}
		else if(detail_function_biased_add==nDetailFunction)
		{
			//diffuse lit + ((detail * 2) - 1)
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
		R0.rgb = (T0*D0) + (T3*R1.a);
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

	SRCALPHA = T0.a * c_primary_change_color.a;

	return half4( SRCCOLOR, SRCALPHA );
}

Technique NoMaskDetailBeforeReflectionBiasedMultiply
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mad r1.w, r0.w, -r1.w, c2.x
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
    mad r2.w, r1.w, -r2.w, c2.x
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
    mov r1.xy, t1.zwzw
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, t3
    mul r3.xyz, r0.w, t3
    mad_pp r3.xyz, r3, c8.w, -r2
    texld_pp r1, r1, s2
    texld_pp r3, r3, s3
    mad_pp r0.xyz, r1.y, c1, r0
    add_pp r0.w, -r1.w, c2.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    nrm r4.xyz, r2
    dp3 r0.w, t3, r4
    mov r2, c10
    mad_pp r2, r0.w, r2, c11
    mul_pp r1.xyw, r2.xyzz, r3.xyzz
    mul_pp r0.w, r1.z, r2.w
    mul_pp r1.xyz, r0.w, r1.xyww
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.w, r3.w, c0.w
    mul_pp r0.xyz, r2, r0
    add_pp r1.xyz, r0, r0
    mov_pp oC0, r1

// approximately 68 instruction slots used (4 texture, 64 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiply
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mad r1.w, r0.w, -r1.w, c2.x
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
    mad r2.w, r1.w, -r2.w, c2.x
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
    mov r1.xy, t1.zwzw
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, t3
    mul r3.xyz, r0.w, t3
    mad_pp r3.xyz, r3, c8.w, -r2
    texld_pp r1, r1, s2
    texld_pp r3, r3, s3
    mad_pp r0.xyz, r1.y, c1, r0
    add_pp r0.w, -r1.w, c2.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    nrm r4.xyz, r2
    dp3 r0.w, t3, r4
    mov r2, c10
    mad_pp r2, r0.w, r2, c11
    mul_pp r1.xyw, r2.xyzz, r3.xyzz
    mul_pp r0.w, r1.z, r2.w
    mul_pp r1.xyz, r0.w, r1.xyww
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.w, r3.w, c0.w
    mul_pp r1.xyz, r2, r0
    mov_pp oC0, r1

// approximately 67 instruction slots used (4 texture, 63 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAdd
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 2, -1, 0
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
    mov_pp r1.w, c2.x
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
    mov r1.xy, t1.zwzw
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, t3
    mul r3.xyz, r0.w, t3
    mad_pp r3.xyz, r3, c8.w, -r2
    texld_pp r1, r1, s2
    texld_pp r3, r3, s3
    mad_pp r0.xyz, r1.y, c1, r0
    add_pp r0.w, -r1.w, c2.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    nrm r4.xyz, r2
    dp3 r0.w, t3, r4
    mov r2, c10
    mad_pp r2, r0.w, r2, c11
    mul_pp r1.xyw, r2.xyzz, r3.xyzz
    mul_pp r0.w, r1.z, r2.w
    mul_pp r1.xyz, r0.w, r1.xyww
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.w, r3.w, c0.w
    mad_pp r0.xyz, r2, c2.y, r0
    add_pp r1.xyz, r0, c2.z
    mov_pp oC0, r1

// approximately 67 instruction slots used (4 texture, 63 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiply
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mad r1.w, r0.w, -r1.w, c2.x
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
    mad r2.w, r1.w, -r2.w, c2.x
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
    mov r1.xy, t1.zwzw
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, t3
    mul r3.xyz, r0.w, t3
    mad_pp r3.xyz, r3, c8.w, -r2
    texld_pp r1, r1, s2
    texld_pp r3, r3, s3
    mad_pp r0.xyz, r1.y, c1, r0
    add_pp r0.w, -r1.w, c2.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    texld_pp r4, t1, s0
    texld_pp r5, t2, s1
    mul_pp r1.xyw, r4.xyzz, r5.xyzz
    mul_pp r4.w, r4.w, c0.w
    mul_pp r0.xyz, r0, r1.xyww
    add_pp r0.xyz, r0, r0
    nrm r5.xyz, r2
    dp3 r0.w, t3, r5
    mov r2, c10
    mad_pp r2, r0.w, r2, c11
    mul_pp r1.xyw, r2.xyzz, r3.xyzz
    mul_pp r0.w, r1.z, r2.w
    mad_pp r4.xyz, r1.xyww, r0.w, r0
    mov_pp oC0, r4

// approximately 68 instruction slots used (4 texture, 64 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiply
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mad r1.w, r0.w, -r1.w, c2.x
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
    mad r2.w, r1.w, -r2.w, c2.x
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
    mov r1.xy, t1.zwzw
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, t3
    mul r3.xyz, r0.w, t3
    mad_pp r3.xyz, r3, c8.w, -r2
    texld_pp r1, r1, s2
    texld_pp r3, r3, s3
    mad_pp r0.xyz, r1.y, c1, r0
    add_pp r0.w, -r1.w, c2.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    nrm r4.xyz, r2
    dp3 r0.w, t3, r4
    mov r2, c10
    mad_pp r2, r0.w, r2, c11
    mul_pp r1.xyw, r2.xyzz, r3.xyzz
    mul_pp r0.w, r1.z, r2.w
    mul_pp r1.xyz, r0.w, r1.xyww
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r3.xyz, r2, r0, r1
    mov_pp oC0, r3

// approximately 67 instruction slots used (4 texture, 63 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAdd
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 2, -1, 0
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
    mov_pp r1.w, c2.x
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
    mov r1.xy, t1.zwzw
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, t3
    mul r3.xyz, r0.w, t3
    mad_pp r3.xyz, r3, c8.w, -r2
    texld_pp r1, r1, s2
    texld_pp r3, r3, s3
    mad_pp r0.xyz, r1.y, c1, r0
    add_pp r0.w, -r1.w, c2.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    nrm r4.xyz, r2
    dp3 r0.w, t3, r4
    mov r2, c10
    mad_pp r2, r0.w, r2, c11
    mul_pp r1.xyw, r2.xyzz, r3.xyzz
    mul_pp r0.w, r1.z, r2.w
    mul_pp r1.xyz, r0.w, r1.xyww
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c2.y, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c2.z
    mad_pp r3.xyz, r2, r0, r1
    mov_pp oC0, r3

// approximately 67 instruction slots used (4 texture, 63 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyComplexFog
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, true, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c5, 1, 2, 0, 0
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
    mov r0.xy, c5
    mad r1.w, r0.w, -c12.w, r0.x
    dp3_sat r0.z, t3, r1
    dp3 r0.w, r1, -c13
    mov r2.w, c13.w
    mad_sat r0.w, r0.w, r2.w, c14.w
    mul r0.z, r0.z, r1.w
    mul r1.x, r0.w, r0.z
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r0.x
    dp3_sat r0.x, t3, r3
    dp3 r0.z, r3, -c16
    mov r0.w, c16.w
    mad_sat r0.z, r0.z, r0.w, c17.w
    mul r0.x, r0.x, r3.w
    mul r1.y, r0.z, r0.x
    max r0.xz, r1.xyyw, t2.z
    min r1.xy, t2.w, r0.xzzw
    mov r2.xyz, c14
    mad_pp r0.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r0.xzw, r1.y, c17.xyyz, r0
    dp3 r1.x, -c18, t3
    mul r1.y, r1.x, c9.z
    max r2.x, r1.x, r1.y
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xzw, r2.x, c19.xyyz, r0
    mad_pp r0.xzw, r2.y, c21.xyyz, r0
    mov r1.xy, t1.zwzw
    add r2.xyz, -t0, c8
    dp3 r2.w, r2, t3
    mul r3.xyz, r2.w, t3
    mad_pp r3.xyz, r3, c8.w, -r2
    texld_pp r1, r1, s2
    texld_pp r3, r3, s3
    mad_pp r0.xzw, r1.y, c4.xyyz, r0
    add_pp r2.w, -r1.w, c5.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r2.w
    mul_pp r0.xzw, r0, r1.xyyw
    nrm r4.xyz, r2
    dp3 r3.w, t3, r4
    mov r2, c10
    mad_pp r2, r3.w, r2, c11
    mul_pp r1.xyw, r2.xyzz, r3.xyzz
    mul_pp r1.z, r1.z, r2.w
    mul_pp r1.xyz, r1.z, r1.xyww
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xzw, r3.xyyz, r0, r1.xyyz
    mul_pp r1.w, r3.w, c0.w
    mul_pp r0.xzw, r2.xyyz, r0
    mul_pp r0.xzw, r0, c1.w
    add r2.xyz, r0.xzww, r0.xzww
    mad r0.xyz, r0.xzww, -r0.y, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 73 instruction slots used (4 texture, 69 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyComplexFog
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, true, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c5, 1, 0, 0, 0
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
    mad r1.w, r0.w, -r1.w, c5.x
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
    mad r2.w, r1.w, -r2.w, c5.x
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
    mov r1.xy, t1.zwzw
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, t3
    mul r3.xyz, r0.w, t3
    mad_pp r3.xyz, r3, c8.w, -r2
    texld_pp r1, r1, s2
    texld_pp r3, r3, s3
    mad_pp r0.xyz, r1.y, c4, r0
    add_pp r0.w, -r1.w, c5.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    nrm r4.xyz, r2
    dp3 r0.w, t3, r4
    mov r2, c10
    mad_pp r2, r0.w, r2, c11
    mul_pp r1.xyw, r2.xyzz, r3.xyzz
    mul_pp r0.w, r1.z, r2.w
    mul_pp r1.xyz, r0.w, r1.xyww
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.w, r3.w, c0.w
    mul_pp r0.xyz, r2, r0
    mul r2.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 73 instruction slots used (4 texture, 69 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddComplexFog
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, true, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c5, 1, 2, -1, 0
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
    mov_pp r1.w, c5.x
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
    mov r1.xy, t1.zwzw
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, t3
    mul r3.xyz, r0.w, t3
    mad_pp r3.xyz, r3, c8.w, -r2
    texld_pp r1, r1, s2
    texld_pp r3, r3, s3
    mad_pp r0.xyz, r1.y, c4, r0
    add_pp r0.w, -r1.w, c5.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    nrm r4.xyz, r2
    dp3 r0.w, t3, r4
    mov r2, c10
    mad_pp r2, r0.w, r2, c11
    mul_pp r1.xyw, r2.xyzz, r3.xyzz
    mul_pp r0.w, r1.z, r2.w
    mul_pp r1.xyz, r0.w, r1.xyww
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.w, r3.w, c0.w
    mad_pp r0.xyz, r2, c5.y, r0
    add_pp r0.xyz, r0, c5.z
    mul r2.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 73 instruction slots used (4 texture, 69 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyComplexFog
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, true, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c5, 1, 0, 0, 0
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
    mad r1.w, r0.w, -r1.w, c5.x
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
    mad r2.w, r1.w, -r2.w, c5.x
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
    mov r1.xy, t1.zwzw
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, t3
    mul r3.xyz, r0.w, t3
    mad_pp r3.xyz, r3, c8.w, -r2
    texld_pp r1, r1, s2
    texld_pp r3, r3, s3
    mad_pp r0.xyz, r1.y, c4, r0
    add_pp r0.w, -r1.w, c5.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    texld_pp r4, t1, s0
    texld_pp r5, t2, s1
    mul_pp r1.xyw, r4.xyzz, r5.xyzz
    mul_pp r4.w, r4.w, c0.w
    mul_pp r0.xyz, r0, r1.xyww
    add_pp r0.xyz, r0, r0
    nrm r5.xyz, r2
    dp3 r0.w, t3, r5
    mov r2, c10
    mad_pp r2, r0.w, r2, c11
    mul_pp r1.xyw, r2.xyzz, r3.xyzz
    mul_pp r0.w, r1.z, r2.w
    mad_pp r0.xyz, r1.xyww, r0.w, r0
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r4.xyz, r0, r1
    mov_pp oC0, r4

// approximately 74 instruction slots used (4 texture, 70 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyComplexFog
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, true, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c5, 1, 0, 0, 0
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
    mad r1.w, r0.w, -r1.w, c5.x
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
    mad r2.w, r1.w, -r2.w, c5.x
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
    mov r1.xy, t1.zwzw
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, t3
    mul r3.xyz, r0.w, t3
    mad_pp r3.xyz, r3, c8.w, -r2
    texld_pp r1, r1, s2
    texld_pp r3, r3, s3
    mad_pp r0.xyz, r1.y, c4, r0
    add_pp r0.w, -r1.w, c5.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    nrm r4.xyz, r2
    dp3 r0.w, t3, r4
    mov r2, c10
    mad_pp r2, r0.w, r2, c11
    mul_pp r1.xyw, r2.xyzz, r3.xyzz
    mul_pp r0.w, r1.z, r2.w
    mul_pp r1.xyz, r0.w, r1.xyww
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r0.xyz, r2, r0, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 73 instruction slots used (4 texture, 69 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddComplexFog
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, true, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c5, 1, 2, -1, 0
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
    mov_pp r1.w, c5.x
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
    mov r1.xy, t1.zwzw
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, t3
    mul r3.xyz, r0.w, t3
    mad_pp r3.xyz, r3, c8.w, -r2
    texld_pp r1, r1, s2
    texld_pp r3, r3, s3
    mad_pp r0.xyz, r1.y, c4, r0
    add_pp r0.w, -r1.w, c5.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    nrm r4.xyz, r2
    dp3 r0.w, t3, r4
    mov r2, c10
    mad_pp r2, r0.w, r2, c11
    mul_pp r1.xyw, r2.xyzz, r3.xyzz
    mul_pp r0.w, r1.z, r2.w
    mul_pp r1.xyz, r0.w, r1.xyww
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c5.y, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c5.z
    mad_pp r0.xyz, r2, r0, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 73 instruction slots used (4 texture, 69 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplySelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mad r1.w, r0.w, -r1.w, c2.x
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
    mad r2.w, r1.w, -r2.w, c2.x
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
    mov r1.xy, t1.zwzw
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, t3
    mul r3.xyz, r0.w, t3
    mad_pp r3.xyz, r3, c8.w, -r2
    texld_pp r1, r1, s2
    texld_pp r3, r3, s3
    mad_pp r0.xyz, r1.y, c1, r0
    add_pp r0.w, -r1.w, c2.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    nrm r4.xyz, r2
    dp3 r0.w, t3, r4
    mov r2, c10
    mad_pp r2, r0.w, r2, c11
    mul_pp r1.xyw, r2.xyzz, r3.xyzz
    mul_pp r0.w, r1.z, r2.w
    mul_pp r1.xyz, r0.w, r1.xyww
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.w, r3.w, c0.w
    mul_pp r0.xyz, r2, r0
    add_pp r1.xyz, r0, r0
    mov_pp oC0, r1

// approximately 68 instruction slots used (4 texture, 64 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplySelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mad r1.w, r0.w, -r1.w, c2.x
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
    mad r2.w, r1.w, -r2.w, c2.x
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
    mov r1.xy, t1.zwzw
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, t3
    mul r3.xyz, r0.w, t3
    mad_pp r3.xyz, r3, c8.w, -r2
    texld_pp r1, r1, s2
    texld_pp r3, r3, s3
    mad_pp r0.xyz, r1.y, c1, r0
    add_pp r0.w, -r1.w, c2.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    nrm r4.xyz, r2
    dp3 r0.w, t3, r4
    mov r2, c10
    mad_pp r2, r0.w, r2, c11
    mul_pp r1.xyw, r2.xyzz, r3.xyzz
    mul_pp r0.w, r1.z, r2.w
    mul_pp r1.xyz, r0.w, r1.xyww
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.w, r3.w, c0.w
    mul_pp r1.xyz, r2, r0
    mov_pp oC0, r1

// approximately 67 instruction slots used (4 texture, 63 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 2, -1, 0
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
    mov_pp r1.w, c2.x
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
    mov r1.xy, t1.zwzw
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, t3
    mul r3.xyz, r0.w, t3
    mad_pp r3.xyz, r3, c8.w, -r2
    texld_pp r1, r1, s2
    texld_pp r3, r3, s3
    mad_pp r0.xyz, r1.y, c1, r0
    add_pp r0.w, -r1.w, c2.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    nrm r4.xyz, r2
    dp3 r0.w, t3, r4
    mov r2, c10
    mad_pp r2, r0.w, r2, c11
    mul_pp r1.xyw, r2.xyzz, r3.xyzz
    mul_pp r0.w, r1.z, r2.w
    mul_pp r1.xyz, r0.w, r1.xyww
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.w, r3.w, c0.w
    mad_pp r0.xyz, r2, c2.y, r0
    add_pp r1.xyz, r0, c2.z
    mov_pp oC0, r1

// approximately 67 instruction slots used (4 texture, 63 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplySelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mad r1.w, r0.w, -r1.w, c2.x
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
    mad r2.w, r1.w, -r2.w, c2.x
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
    mov r1.xy, t1.zwzw
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, t3
    mul r3.xyz, r0.w, t3
    mad_pp r3.xyz, r3, c8.w, -r2
    texld_pp r1, r1, s2
    texld_pp r3, r3, s3
    mad_pp r0.xyz, r1.y, c1, r0
    add_pp r0.w, -r1.w, c2.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    texld_pp r4, t1, s0
    texld_pp r5, t2, s1
    mul_pp r1.xyw, r4.xyzz, r5.xyzz
    mul_pp r4.w, r4.w, c0.w
    mul_pp r0.xyz, r0, r1.xyww
    add_pp r0.xyz, r0, r0
    nrm r5.xyz, r2
    dp3 r0.w, t3, r5
    mov r2, c10
    mad_pp r2, r0.w, r2, c11
    mul_pp r1.xyw, r2.xyzz, r3.xyzz
    mul_pp r0.w, r1.z, r2.w
    mad_pp r4.xyz, r1.xyww, r0.w, r0
    mov_pp oC0, r4

// approximately 68 instruction slots used (4 texture, 64 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplySelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mad r1.w, r0.w, -r1.w, c2.x
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
    mad r2.w, r1.w, -r2.w, c2.x
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
    mov r1.xy, t1.zwzw
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, t3
    mul r3.xyz, r0.w, t3
    mad_pp r3.xyz, r3, c8.w, -r2
    texld_pp r1, r1, s2
    texld_pp r3, r3, s3
    mad_pp r0.xyz, r1.y, c1, r0
    add_pp r0.w, -r1.w, c2.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    nrm r4.xyz, r2
    dp3 r0.w, t3, r4
    mov r2, c10
    mad_pp r2, r0.w, r2, c11
    mul_pp r1.xyw, r2.xyzz, r3.xyzz
    mul_pp r0.w, r1.z, r2.w
    mul_pp r1.xyz, r0.w, r1.xyww
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r3.xyz, r2, r0, r1
    mov_pp oC0, r3

// approximately 67 instruction slots used (4 texture, 63 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, false, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 2, -1, 0
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
    mov_pp r1.w, c2.x
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
    mov r1.xy, t1.zwzw
    add r2.xyz, -t0, c8
    dp3 r0.w, r2, t3
    mul r3.xyz, r0.w, t3
    mad_pp r3.xyz, r3, c8.w, -r2
    texld_pp r1, r1, s2
    texld_pp r3, r3, s3
    mad_pp r0.xyz, r1.y, c1, r0
    add_pp r0.w, -r1.w, c2.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    nrm r4.xyz, r2
    dp3 r0.w, t3, r4
    mov r2, c10
    mad_pp r2, r0.w, r2, c11
    mul_pp r1.xyw, r2.xyzz, r3.xyzz
    mul_pp r0.w, r1.z, r2.w
    mul_pp r1.xyz, r0.w, r1.xyww
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c2.y, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c2.z
    mad_pp r3.xyz, r2, r0, r1
    mov_pp oC0, r3

// approximately 67 instruction slots used (4 texture, 63 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyBaseNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    mul_pp r0.xyz, r2, r0
    add_pp r1.xyz, r0, r0
    mov_pp oC0, r1

// approximately 73 instruction slots used (5 texture, 68 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyBaseNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    mul_pp r1.xyz, r2, r0
    mov_pp oC0, r1

// approximately 72 instruction slots used (5 texture, 67 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddBaseNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    mad_pp r0.xyz, r2, c2.x, r0
    add_pp r1.xyz, r0, c2.y
    mov_pp oC0, r1

// approximately 73 instruction slots used (5 texture, 68 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyBaseNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    texld_pp r5, t1, s0
    texld_pp r6, t2, s1
    mul_pp r2.xyw, r5.xyzz, r6.xyzz
    mul_pp r5.w, r5.w, c0.w
    mul_pp r1.xyz, r1, r2.xyww
    add_pp r1.xyz, r1, r1
    nrm r6.xyz, r3
    dp3 r1.w, r0.yzww, r6
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mad_pp r5.xyz, r0, r0.w, r1
    mov_pp oC0, r5

// approximately 73 instruction slots used (5 texture, 68 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyBaseNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 72 instruction slots used (5 texture, 67 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddBaseNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    texld_pp r5, t1, s0
    texld_pp r6, t2, s1
    mad_pp r2.xyw, r6.xyzz, c2.x, r5.xyzz
    mul_pp r5.w, r5.w, c0.w
    add_pp r2.xyw, r2, c2.y
    nrm r6.xyz, r3
    dp3 r1.w, r0.yzww, r6
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r5.xyz, r2.xyww, r1, r0
    mov_pp oC0, r5

// approximately 73 instruction slots used (5 texture, 68 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyComplexFogBaseNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, true, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 2, -1, 1, 0
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
    mov r0.xz, c6
    mad r1.w, r0.w, -c12.w, r0.z
    texld r2, t1, s4
    mad r2.xyz, r2, c6.x, c6.y
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
    mov r1.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r2.w, r3, r2
    mul r4.xyz, r2, r2.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r1, r1, s2
    texld_pp r4, r4, s3
    mad_pp r0.yzw, r1.y, c4.xxyz, r0
    add_pp r2.w, -r1.w, c6.z
    mad_pp r1.xyw, r1.w, c0.xyzz, r2.w
    mul_pp r0.yzw, r0, r1.xxyw
    nrm r5.xyz, r3
    dp3 r4.w, r2, r5
    mov r2, c10
    mad_pp r2, r4.w, r2, c11
    mul_pp r1.xyw, r2.xyzz, r4.xyzz
    mul_pp r1.z, r1.z, r2.w
    mul_pp r1.xyz, r1.z, r1.xyww
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.yzw, r3.xxyz, r0, r1.xxyz
    mul_pp r1.w, r3.w, c0.w
    mul_pp r0.yzw, r2.xxyz, r0
    mul_pp r0.yzw, r0, c1.w
    add r2.xyz, r0.yzww, r0.yzww
    mad r0.xyz, r0.yzww, -r0.x, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 79 instruction slots used (5 texture, 74 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyComplexFogBaseNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, true, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 2, -1, 1, 0
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
    mov r1.w, c6.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c6.x, c6.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r1.w, -r2.w, c6.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    mul_pp r0.xyz, r2, r0
    mul r2.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 78 instruction slots used (5 texture, 73 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddComplexFogBaseNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, true, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 2, -1, 1, 0
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
    mov r1.w, c6.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c6.x, c6.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r1.w, -r2.w, c6.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    mad_pp r0.xyz, r2, c6.x, r0
    add_pp r0.xyz, r0, c6.y
    mul r2.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 79 instruction slots used (5 texture, 74 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyComplexFogBaseNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, true, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 2, -1, 1, 0
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
    mov r1.w, c6.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c6.x, c6.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r1.w, -r2.w, c6.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    texld_pp r5, t1, s0
    texld_pp r6, t2, s1
    mul_pp r2.xyw, r5.xyzz, r6.xyzz
    mul_pp r5.w, r5.w, c0.w
    mul_pp r1.xyz, r1, r2.xyww
    add_pp r1.xyz, r1, r1
    nrm r6.xyz, r3
    dp3 r1.w, r0.yzww, r6
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r5.xyz, r0, r1
    mov_pp oC0, r5

// approximately 79 instruction slots used (5 texture, 74 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyComplexFogBaseNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, true, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 2, -1, 1, 0
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
    mov r1.w, c6.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c6.x, c6.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r1.w, -r2.w, c6.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r0.xyz, r2, r1, r0
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 78 instruction slots used (5 texture, 73 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddComplexFogBaseNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, true, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 2, -1, 1, 0
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
    mov r1.w, c6.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c6.x, c6.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r1.w, -r2.w, c6.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    texld_pp r5, t1, s0
    texld_pp r6, t2, s1
    mad_pp r2.xyw, r6.xyzz, c6.x, r5.xyzz
    mul_pp r5.w, r5.w, c0.w
    add_pp r2.xyw, r2, c6.y
    nrm r6.xyz, r3
    dp3 r1.w, r0.yzww, r6
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r2.xyww, r1, r0
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r5.xyz, r0, r1
    mov_pp oC0, r5

// approximately 79 instruction slots used (5 texture, 74 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyBaseNormSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    mul_pp r0.xyz, r2, r0
    add_pp r1.xyz, r0, r0
    mov_pp oC0, r1

// approximately 73 instruction slots used (5 texture, 68 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyBaseNormSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    mul_pp r1.xyz, r2, r0
    mov_pp oC0, r1

// approximately 72 instruction slots used (5 texture, 67 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddBaseNormSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    mad_pp r0.xyz, r2, c2.x, r0
    add_pp r1.xyz, r0, c2.y
    mov_pp oC0, r1

// approximately 73 instruction slots used (5 texture, 68 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyBaseNormSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    texld_pp r5, t1, s0
    texld_pp r6, t2, s1
    mul_pp r2.xyw, r5.xyzz, r6.xyzz
    mul_pp r5.w, r5.w, c0.w
    mul_pp r1.xyz, r1, r2.xyww
    add_pp r1.xyz, r1, r1
    nrm r6.xyz, r3
    dp3 r1.w, r0.yzww, r6
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mad_pp r5.xyz, r0, r0.w, r1
    mov_pp oC0, r5

// approximately 73 instruction slots used (5 texture, 68 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyBaseNormSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 72 instruction slots used (5 texture, 67 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddBaseNormSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, true, false, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    texld_pp r5, t1, s0
    texld_pp r6, t2, s1
    mad_pp r2.xyw, r6.xyzz, c2.x, r5.xyzz
    mul_pp r5.w, r5.w, c0.w
    add_pp r2.xyw, r2, c2.y
    nrm r6.xyz, r3
    dp3 r1.w, r0.yzww, r6
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    mad_pp r5.xyz, r2.xyww, r1, r0
    mov_pp oC0, r5

// approximately 73 instruction slots used (5 texture, 68 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyBaseNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mul_pp r0.xyz, r0, r2
    add_pp r1.xyz, r0, r0
    mov_pp oC0, r1

// approximately 83 instruction slots used (6 texture, 77 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyBaseNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mul_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 82 instruction slots used (6 texture, 76 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddBaseNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mad_pp r0.xyz, r2, c2.x, r0
    add_pp r1.xyz, r0, c2.y
    mov_pp oC0, r1

// approximately 83 instruction slots used (6 texture, 77 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyBaseNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mul_pp r2.xyw, r3.xyzz, r4.xyzz
    mul_pp r3.w, r3.w, c0.w
    mul_pp r1.xyz, r1, r2.xyww
    add_pp r1.xyz, r1, r1
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mad_pp r3.xyz, r0, r0.w, r1
    mov_pp oC0, r3

// approximately 83 instruction slots used (6 texture, 77 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyBaseNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2.xyww
    nrm r4.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r4, t1, s7
    texld_pp r3, r3, s3
    log r5.x, r4.x
    log r5.y, r4.y
    log r5.z, r4.z
    mul r2.xyw, r5.xyzz, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.w
    mul_sat_pp r2.xyw, r4.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r3
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 82 instruction slots used (6 texture, 76 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddBaseNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2.xyww
    nrm r4.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r4, t1, s7
    texld_pp r3, r3, s3
    log r5.x, r4.x
    log r5.y, r4.y
    log r5.z, r4.z
    mul r2.xyw, r5.xyzz, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.w
    mul_sat_pp r2.xyw, r4.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r3
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c2.x, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c2.y
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 83 instruction slots used (6 texture, 77 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyComplexFogBaseNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, true, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 2, -1, 1, 0
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
    mov r0.xz, c6
    mad r1.w, r0.w, -c12.w, r0.z
    texld r2, t1, s4
    mad r2.xyz, r2, c6.x, c6.y
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
    mov r1.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r1, r1, s2
    mad_pp r0.yzw, r1.y, c4.xxyz, r0
    add_pp r2.w, -r1.w, c6.z
    mad_pp r1.xyw, r1.w, c0.xyzz, r2.w
    mul_pp r0.yzw, r0, r1.xxyw
    add r1.xyw, -t0.xyzz, c8.xyzz
    dp3 r2.w, r1.xyww, r2
    mul r4.xyz, r2, r2.w
    mad_pp r4.xyz, r4, c8.w, -r1.xyww
    nrm r5.xyz, r1.xyww
    dp3 r4.w, r2, r5
    mov r2, c10
    mad_pp r2, r4.w, r2, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r1.xyw, r6.xyzz, c5.z
    exp r5.x, r1.x
    exp r5.y, r1.y
    exp r5.z, r1.w
    mul_sat_pp r1.xyw, r5.xyzz, c5.w
    mul_pp r1.xyw, r1, r2.xyzz
    mul_pp r4.w, r1.z, r2.w
    mul_pp r1.xyz, r1.xyww, r4
    mul_pp r1.xyz, r4.w, r1
    mad_pp r0.yzw, r3.xxyz, r0, r1.xxyz
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mul_pp r0.yzw, r0, r2.xxyz
    mul_pp r0.yzw, r0, c1.w
    add r2.xyz, r0.yzww, r0.yzww
    mad r0.xyz, r0.yzww, -r0.x, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 89 instruction slots used (6 texture, 83 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyComplexFogBaseNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, true, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 2, -1, 1, 0
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
    mov r1.w, c6.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c6.x, c6.y
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
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r1.w, -r2.w, c6.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mul_pp r0.xyz, r0, r2
    mul r2.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 88 instruction slots used (6 texture, 82 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddComplexFogBaseNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, true, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 2, -1, 1, 0
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
    mov r1.w, c6.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c6.x, c6.y
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
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r1.w, -r2.w, c6.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mad_pp r0.xyz, r2, c6.x, r0
    add_pp r0.xyz, r0, c6.y
    mul r2.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 89 instruction slots used (6 texture, 83 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyComplexFogBaseNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, true, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 2, -1, 1, 0
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
    mov r1.w, c6.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c6.x, c6.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r1.w, -r2.w, c6.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mul_pp r2.xyw, r3.xyzz, r4.xyzz
    mul_pp r3.w, r3.w, c0.w
    mul_pp r1.xyz, r1, r2.xyww
    add_pp r1.xyz, r1, r1
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 89 instruction slots used (6 texture, 83 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyComplexFogBaseNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, true, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 2, -1, 1, 0
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
    mov r1.w, c6.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c6.x, c6.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r1.w, -r2.w, c6.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2.xyww
    nrm r4.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r4, t1, s7
    texld_pp r3, r3, s3
    log r5.x, r4.x
    log r5.y, r4.y
    log r5.z, r4.z
    mul r2.xyw, r5.xyzz, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.w
    mul_sat_pp r2.xyw, r4.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r3
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r0.xyz, r2, r1, r0
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 88 instruction slots used (6 texture, 82 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddComplexFogBaseNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, true, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 2, -1, 1, 0
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
    mov r1.w, c6.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c6.x, c6.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r1.w, -r2.w, c6.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2.xyww
    nrm r4.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r4, t1, s7
    texld_pp r3, r3, s3
    log r5.x, r4.x
    log r5.y, r4.y
    log r5.z, r4.z
    mul r2.xyw, r5.xyzz, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.w
    mul_sat_pp r2.xyw, r4.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r3
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c6.x, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c6.y
    mad_pp r0.xyz, r2, r1, r0
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 89 instruction slots used (6 texture, 83 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyBaseNormSpecMapSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mul_pp r0.xyz, r0, r2
    add_pp r1.xyz, r0, r0
    mov_pp oC0, r1

// approximately 83 instruction slots used (6 texture, 77 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyBaseNormSpecMapSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mul_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 82 instruction slots used (6 texture, 76 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddBaseNormSpecMapSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mad_pp r0.xyz, r2, c2.x, r0
    add_pp r1.xyz, r0, c2.y
    mov_pp oC0, r1

// approximately 83 instruction slots used (6 texture, 77 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyBaseNormSpecMapSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mul_pp r2.xyw, r3.xyzz, r4.xyzz
    mul_pp r3.w, r3.w, c0.w
    mul_pp r1.xyz, r1, r2.xyww
    add_pp r1.xyz, r1, r1
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mad_pp r3.xyz, r0, r0.w, r1
    mov_pp oC0, r3

// approximately 83 instruction slots used (6 texture, 77 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyBaseNormSpecMapSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2.xyww
    nrm r4.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r4, t1, s7
    texld_pp r3, r3, s3
    log r5.x, r4.x
    log r5.y, r4.y
    log r5.z, r4.z
    mul r2.xyw, r5.xyzz, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.w
    mul_sat_pp r2.xyw, r4.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r3
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 82 instruction slots used (6 texture, 76 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddBaseNormSpecMapSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, true, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t1, s4
    mad r0.yzw, r2.xxyz, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2.xyww
    nrm r4.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r4, t1, s7
    texld_pp r3, r3, s3
    log r5.x, r4.x
    log r5.y, r4.y
    log r5.z, r4.z
    mul r2.xyw, r5.xyzz, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.w
    mul_sat_pp r2.xyw, r4.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r3
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c2.x, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c2.y
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 83 instruction slots used (6 texture, 77 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyBaseNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov_pp r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    mul_pp r2.w, r8.w, c0.w
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mul_pp r0.xyz, r3, r0
    add_pp r2.xyz, r0, r0
    mov_pp oC0, r2

// approximately 118 instruction slots used (5 texture, 113 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyBaseNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    mul_pp r2.w, r8.w, c0.w
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mul_pp r2.xyz, r3, r0
    mov_pp oC0, r2

// approximately 117 instruction slots used (5 texture, 112 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddBaseNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov_pp r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    mul_pp r2.w, r8.w, c0.w
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mad_pp r0.xyz, r3, c2.x, r0
    add_pp r2.xyz, r0, c2.y
    mov_pp oC0, r2

// approximately 118 instruction slots used (5 texture, 113 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyBaseNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov_pp r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mov r3.xy, t1.zwzw
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, r3, s2
    texld_pp r0, r0, s3
    mad_pp r2.xyz, r3.y, c1, r2
    add_pp r0.w, -r3.w, c2.z
    mad_pp r3.xyw, r3.w, c0.xyzz, r0.w
    mul_pp r2.xyz, r2, r3.xyww
    texld_pp r5, t1, s0
    texld_pp r6, t2, s1
    mul_pp r3.xyw, r5.xyzz, r6.xyzz
    mul_pp r5.w, r5.w, c0.w
    mul_pp r2.xyz, r2, r3.xyww
    add_pp r2.xyz, r2, r2
    mad_pp r1.xyz, r1, r3.z, r2
    mul_pp r0.w, r4.w, r3.z
    mul_pp r0.xyz, r4, r0
    mad_pp r5.xyz, r0, r0.w, r1
    mov_pp oC0, r5

// approximately 118 instruction slots used (5 texture, 113 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyBaseNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r5.y, c1, r2
    add_pp r0.w, -r5.w, c2.z
    mad_pp r3.xyz, r5.w, c0, r0.w
    mul_pp r0.w, r4.w, r5.z
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r5, t2, s1
    mul_pp r3.xyz, r3, r5
    mul_pp r5.w, r3.w, c0.w
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r0.xyz, r4, r0
    mad_pp r5.xyz, r0, r0.w, r1
    mov_pp oC0, r5

// approximately 117 instruction slots used (5 texture, 112 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddBaseNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov_pp r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    texld_pp r6, t1, s0
    texld_pp r8, t2, s1
    mad_pp r6.xyz, r8, c2.x, r6
    mul_pp r8.w, r6.w, c0.w
    add_pp r6.xyz, r6, c2.y
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r5.y, c1, r2
    add_pp r0.w, -r5.w, c2.z
    mad_pp r3.xyz, r5.w, c0, r0.w
    mul_pp r0.w, r4.w, r5.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r6, r2, r1
    mul_pp r0.xyz, r4, r0
    mad_pp r8.xyz, r0, r0.w, r1
    mov_pp oC0, r8

// approximately 118 instruction slots used (5 texture, 113 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyComplexFogBaseNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, true, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 2, -1, 1, 0
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
    mov r0.xz, c6
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r3.xyz, r3, c6.x, c6.y
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
    max r2.w, r1.w, c6.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r0.yzw, r1.w, c17.xxyz
    mul r0.yzw, r4.y, r0
    dp3 r1.w, -r1, r3
    add r1.w, r1.w, r1.w
    mad r1.xyz, r3, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r3.w, r2.w, c6.w
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r0.yzw, r1.xxyz, r4.x, r0
    dp3 r2.w, c18, r3
    add r2.w, r2.w, r2.w
    mad r1.xyz, r3, -r2.w, c18
    dp3 r2.w, r1, r6
    max r3.w, r2.w, c6.w
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
    max r3.w, r2.w, c6.w
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
    mad_pp r2.xyz, r6.y, c4, r2
    add_pp r2.w, -r6.w, c6.z
    mad_pp r4.xyz, r6.w, c0, r2.w
    mul_pp r1.w, r1.w, r6.z
    mul_pp r2.xyz, r2, r4
    mad_pp r0.yzw, r8.xxyz, r2.xxyz, r0
    mul_pp r2.w, r8.w, c0.w
    dp3 r3.w, r5, r3
    mul r3.xyz, r3, r3.w
    mad_pp r3.xyz, r3, c8.w, -r5
    texld_pp r4, t2, s1
    texld_pp r3, r3, s3
    mul_pp r1.xyz, r1, r3
    mad_pp r0.yzw, r1.xxyz, r1.w, r0
    mul_pp r0.yzw, r4.xxyz, r0
    mul_pp r0.yzw, r0, c1.w
    add r1.xyz, r0.yzww, r0.yzww
    mad r0.xyz, r0.yzww, -r0.x, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 124 instruction slots used (5 texture, 119 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyComplexFogBaseNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, true, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 2, -1, 1, 0
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
    mov r0.z, c6.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c6.x, c6.y
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
    max r2.w, r1.w, c6.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c6.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c6.w
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
    max r2.w, r1.w, c6.w
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
    mad_pp r2.xyz, r6.y, c4, r2
    add_pp r1.w, -r6.w, c6.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    mul_pp r2.w, r8.w, c0.w
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mul_pp r0.xyz, r3, r0
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 123 instruction slots used (5 texture, 118 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddComplexFogBaseNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, true, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 2, -1, 1, 0
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
    mov_pp r0.z, c6.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c6.x, c6.y
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
    max r2.w, r1.w, c6.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c6.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c6.w
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
    max r2.w, r1.w, c6.w
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
    mad_pp r2.xyz, r6.y, c4, r2
    add_pp r1.w, -r6.w, c6.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    mul_pp r2.w, r8.w, c0.w
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mad_pp r0.xyz, r3, c6.x, r0
    add_pp r0.xyz, r0, c6.y
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 124 instruction slots used (5 texture, 119 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyComplexFogBaseNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, true, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 2, -1, 1, 0
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
    mov_pp r0.z, c6.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c6.x, c6.y
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
    max r2.w, r1.w, c6.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c6.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c6.w
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
    max r2.w, r1.w, c6.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mov r3.xy, t1.zwzw
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, r3, s2
    texld_pp r0, r0, s3
    mad_pp r2.xyz, r3.y, c4, r2
    add_pp r0.w, -r3.w, c6.z
    mad_pp r3.xyw, r3.w, c0.xyzz, r0.w
    mul_pp r2.xyz, r2, r3.xyww
    texld_pp r5, t1, s0
    texld_pp r6, t2, s1
    mul_pp r3.xyw, r5.xyzz, r6.xyzz
    mul_pp r5.w, r5.w, c0.w
    mul_pp r2.xyz, r2, r3.xyww
    add_pp r2.xyz, r2, r2
    mad_pp r1.xyz, r1, r3.z, r2
    mul_pp r0.w, r4.w, r3.z
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r5.xyz, r0, r1
    mov_pp oC0, r5

// approximately 124 instruction slots used (5 texture, 119 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyComplexFogBaseNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, true, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 2, -1, 1, 0
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
    mov r0.z, c6.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c6.x, c6.y
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
    max r2.w, r1.w, c6.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c6.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c6.w
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
    max r2.w, r1.w, c6.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r5.y, c4, r2
    add_pp r0.w, -r5.w, c6.z
    mad_pp r3.xyz, r5.w, c0, r0.w
    mul_pp r0.w, r4.w, r5.z
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r5, t2, s1
    mul_pp r3.xyz, r3, r5
    mul_pp r5.w, r3.w, c0.w
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r5.xyz, r0, r1
    mov_pp oC0, r5

// approximately 123 instruction slots used (5 texture, 118 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddComplexFogBaseNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, true, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 2, -1, 1, 0
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
    mov_pp r0.z, c6.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c6.x, c6.y
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
    max r2.w, r1.w, c6.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c6.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c6.w
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
    max r2.w, r1.w, c6.w
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
    texld_pp r6, t1, s0
    texld_pp r8, t2, s1
    mad_pp r6.xyz, r8, c6.x, r6
    mul_pp r8.w, r6.w, c0.w
    add_pp r6.xyz, r6, c6.y
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r5.y, c4, r2
    add_pp r0.w, -r5.w, c6.z
    mad_pp r3.xyz, r5.w, c0, r0.w
    mul_pp r0.w, r4.w, r5.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r6, r2, r1
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r8.xyz, r0, r1
    mov_pp oC0, r8

// approximately 124 instruction slots used (5 texture, 119 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyBaseNormSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov_pp r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    mul_pp r2.w, r8.w, c0.w
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mul_pp r0.xyz, r3, r0
    add_pp r2.xyz, r0, r0
    mov_pp oC0, r2

// approximately 118 instruction slots used (5 texture, 113 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyBaseNormSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    mul_pp r2.w, r8.w, c0.w
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mul_pp r2.xyz, r3, r0
    mov_pp oC0, r2

// approximately 117 instruction slots used (5 texture, 112 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddBaseNormSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov_pp r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    mul_pp r2.w, r8.w, c0.w
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mad_pp r0.xyz, r3, c2.x, r0
    add_pp r2.xyz, r0, c2.y
    mov_pp oC0, r2

// approximately 118 instruction slots used (5 texture, 113 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyBaseNormSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov_pp r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mov r3.xy, t1.zwzw
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, r3, s2
    texld_pp r0, r0, s3
    mad_pp r2.xyz, r3.y, c1, r2
    add_pp r0.w, -r3.w, c2.z
    mad_pp r3.xyw, r3.w, c0.xyzz, r0.w
    mul_pp r2.xyz, r2, r3.xyww
    texld_pp r5, t1, s0
    texld_pp r6, t2, s1
    mul_pp r3.xyw, r5.xyzz, r6.xyzz
    mul_pp r5.w, r5.w, c0.w
    mul_pp r2.xyz, r2, r3.xyww
    add_pp r2.xyz, r2, r2
    mad_pp r1.xyz, r1, r3.z, r2
    mul_pp r0.w, r4.w, r3.z
    mul_pp r0.xyz, r4, r0
    mad_pp r5.xyz, r0, r0.w, r1
    mov_pp oC0, r5

// approximately 118 instruction slots used (5 texture, 113 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyBaseNormSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r5.y, c1, r2
    add_pp r0.w, -r5.w, c2.z
    mad_pp r3.xyz, r5.w, c0, r0.w
    mul_pp r0.w, r4.w, r5.z
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r5, t2, s1
    mul_pp r3.xyz, r3, r5
    mul_pp r5.w, r3.w, c0.w
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r0.xyz, r4, r0
    mad_pp r5.xyz, r0, r0.w, r1
    mov_pp oC0, r5

// approximately 117 instruction slots used (5 texture, 112 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddBaseNormSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, true, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov_pp r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    texld_pp r6, t1, s0
    texld_pp r8, t2, s1
    mad_pp r6.xyz, r8, c2.x, r6
    mul_pp r8.w, r6.w, c0.w
    add_pp r6.xyz, r6, c2.y
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r5.y, c1, r2
    add_pp r0.w, -r5.w, c2.z
    mad_pp r3.xyz, r5.w, c0, r0.w
    mul_pp r0.w, r4.w, r5.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r6, r2, r1
    mul_pp r0.xyz, r4, r0
    mad_pp r8.xyz, r0, r0.w, r1
    mov_pp oC0, r8

// approximately 118 instruction slots used (5 texture, 113 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyBaseNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov_pp r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    dp3 r2.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r2.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r2.w, r3.w, c0.w
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    add_pp r2.xyz, r0, r0
    mov_pp oC0, r2

// approximately 128 instruction slots used (6 texture, 122 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyBaseNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    dp3 r2.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r2.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r2.w, r3.w, c0.w
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    texld_pp r1, t2, s1
    mul_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 127 instruction slots used (6 texture, 121 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddBaseNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov_pp r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    dp3 r2.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r2.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r2.w, r3.w, c0.w
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c2.x, r0
    add_pp r2.xyz, r0, c2.y
    mov_pp oC0, r2

// approximately 128 instruction slots used (6 texture, 122 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyBaseNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov_pp r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r7, t2, s1
    mul_pp r3.xyz, r3, r7
    mul_pp r7.w, r3.w, c0.w
    mul_pp r2.xyz, r2, r3
    add_pp r2.xyz, r2, r2
    mad_pp r1.xyz, r1, r6.z, r2
    mul_pp r1.w, r4.w, r6.z
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r7.xyz, r0, r1.w, r1
    mov_pp oC0, r7

// approximately 128 instruction slots used (6 texture, 122 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyBaseNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r6, t2, s1
    mul_pp r3.xyz, r3, r6
    mul_pp r6.w, r3.w, c0.w
    mad_pp r1.xyz, r3, r2, r1
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r6.xyz, r0, r1.w, r1
    mov_pp oC0, r6

// approximately 127 instruction slots used (6 texture, 121 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddBaseNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov_pp r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    texld_pp r8, t1, s0
    texld_pp r9, t2, s1
    mad_pp r8.xyz, r9, c2.x, r8
    mul_pp r9.w, r8.w, c0.w
    add_pp r8.xyz, r8, c2.y
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r9.xyz, r0, r1.w, r1
    mov_pp oC0, r9

// approximately 128 instruction slots used (6 texture, 122 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyComplexFogBaseNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, true, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 2, -1, 1, 0
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
    mov r0.xz, c6
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r3.xyz, r3, c6.x, c6.y
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
    max r2.w, r1.w, c6.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r0.yzw, r1.w, c17.xxyz
    mul r0.yzw, r4.y, r0
    dp3 r1.w, -r1, r3
    add r1.w, r1.w, r1.w
    mad r1.xyz, r3, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r3.w, r2.w, c6.w
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r0.yzw, r1.xxyz, r4.x, r0
    dp3 r2.w, c18, r3
    add r2.w, r2.w, r2.w
    mad r1.xyz, r3, -r2.w, c18
    dp3 r2.w, r1, r6
    max r3.w, r2.w, c6.w
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
    max r3.w, r2.w, c6.w
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
    mad_pp r2.xyz, r4.x, r2, c22
    mad_pp r2.xyz, r4.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c4, r2
    add_pp r2.w, -r6.w, c6.z
    mad_pp r4.xyz, r6.w, c0, r2.w
    mul_pp r1.w, r1.w, r6.z
    mul_pp r2.xyz, r2, r4
    dp3 r2.w, r5, r3
    mul r3.xyz, r3, r2.w
    mad_pp r3.xyz, r3, c8.w, -r5
    texld_pp r4, t1, s0
    texld_pp r3, r3, s3
    mad_pp r0.yzw, r4.xxyz, r2.xxyz, r0
    mul_pp r2.w, r4.w, c0.w
    mul_pp r1.xyz, r1, r3
    mad_pp r0.yzw, r1.xxyz, r1.w, r0
    texld_pp r1, t2, s1
    mul_pp r0.yzw, r0, r1.xxyz
    mul_pp r0.yzw, r0, c1.w
    add r1.xyz, r0.yzww, r0.yzww
    mad r0.xyz, r0.yzww, -r0.x, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 134 instruction slots used (6 texture, 128 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyComplexFogBaseNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, true, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 2, -1, 1, 0
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
    mov r0.z, c6.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c6.x, c6.y
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
    max r2.w, r1.w, c6.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c6.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c6.w
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
    max r2.w, r1.w, c6.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c4, r2
    add_pp r1.w, -r6.w, c6.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    dp3 r2.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r2.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r2.w, r3.w, c0.w
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 133 instruction slots used (6 texture, 127 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddComplexFogBaseNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, true, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 2, -1, 1, 0
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
    mov_pp r0.z, c6.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c6.x, c6.y
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
    max r2.w, r1.w, c6.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c6.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c6.w
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
    max r2.w, r1.w, c6.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c4, r2
    add_pp r1.w, -r6.w, c6.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    dp3 r2.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r2.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r2.w, r3.w, c0.w
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c6.x, r0
    add_pp r0.xyz, r0, c6.y
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 134 instruction slots used (6 texture, 128 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyComplexFogBaseNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, true, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 2, -1, 1, 0
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
    mov_pp r0.z, c6.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c6.x, c6.y
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
    max r2.w, r1.w, c6.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c6.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c6.w
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
    max r2.w, r1.w, c6.w
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
    mad_pp r2.xyz, r6.y, c4, r2
    add_pp r1.w, -r6.w, c6.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r7, t2, s1
    mul_pp r3.xyz, r3, r7
    mul_pp r7.w, r3.w, c0.w
    mul_pp r2.xyz, r2, r3
    add_pp r2.xyz, r2, r2
    mad_pp r1.xyz, r1, r6.z, r2
    mul_pp r1.w, r4.w, r6.z
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r7.xyz, r0, r1
    mov_pp oC0, r7

// approximately 134 instruction slots used (6 texture, 128 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyComplexFogBaseNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, true, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 2, -1, 1, 0
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
    mov r0.z, c6.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c6.x, c6.y
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
    max r2.w, r1.w, c6.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c6.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c6.w
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
    max r2.w, r1.w, c6.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c4, r2
    add_pp r1.w, -r6.w, c6.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r6, t2, s1
    mul_pp r3.xyz, r3, r6
    mul_pp r6.w, r3.w, c0.w
    mad_pp r1.xyz, r3, r2, r1
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 133 instruction slots used (6 texture, 127 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddComplexFogBaseNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, true, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 2, -1, 1, 0
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
    mov_pp r0.z, c6.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c6.x, c6.y
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
    max r2.w, r1.w, c6.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c6.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c6.w
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
    max r2.w, r1.w, c6.w
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
    texld_pp r8, t1, s0
    texld_pp r9, t2, s1
    mad_pp r8.xyz, r9, c6.x, r8
    mul_pp r9.w, r8.w, c0.w
    add_pp r8.xyz, r8, c6.y
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c4, r2
    add_pp r1.w, -r6.w, c6.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r9.xyz, r0, r1
    mov_pp oC0, r9

// approximately 134 instruction slots used (6 texture, 128 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyBaseNormSpecMapSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov_pp r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    dp3 r2.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r2.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r2.w, r3.w, c0.w
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    add_pp r2.xyz, r0, r0
    mov_pp oC0, r2

// approximately 128 instruction slots used (6 texture, 122 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyBaseNormSpecMapSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    dp3 r2.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r2.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r2.w, r3.w, c0.w
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    texld_pp r1, t2, s1
    mul_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 127 instruction slots used (6 texture, 121 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddBaseNormSpecMapSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov_pp r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    dp3 r2.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r2.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r2.w, r3.w, c0.w
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c2.x, r0
    add_pp r2.xyz, r0, c2.y
    mov_pp oC0, r2

// approximately 128 instruction slots used (6 texture, 122 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyBaseNormSpecMapSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov_pp r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r7, t2, s1
    mul_pp r3.xyz, r3, r7
    mul_pp r7.w, r3.w, c0.w
    mul_pp r2.xyz, r2, r3
    add_pp r2.xyz, r2, r2
    mad_pp r1.xyz, r1, r6.z, r2
    mul_pp r1.w, r4.w, r6.z
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r7.xyz, r0, r1.w, r1
    mov_pp oC0, r7

// approximately 128 instruction slots used (6 texture, 122 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyBaseNormSpecMapSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r6, t2, s1
    mul_pp r3.xyz, r3, r6
    mul_pp r6.w, r3.w, c0.w
    mad_pp r1.xyz, r3, r2, r1
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r6.xyz, r0, r1.w, r1
    mov_pp oC0, r6

// approximately 127 instruction slots used (6 texture, 121 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddBaseNormSpecMapSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, true, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov_pp r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t1, s4
    mad r0.xyw, r3.xyzz, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    texld_pp r8, t1, s0
    texld_pp r9, t2, s1
    mad_pp r8.xyz, r9, c2.x, r8
    mul_pp r9.w, r8.w, c0.w
    add_pp r8.xyz, r8, c2.y
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r9.xyz, r0, r1.w, r1
    mov_pp oC0, r9

// approximately 128 instruction slots used (6 texture, 122 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyBaseNormDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mad r0.xy, r0, c2.x, c2.y
    mad r0.zw, r1.xyxy, c2.x, c2.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c2.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c2.z, c2.ywzw
    mad r1.xyz, r1, c2.x, c2.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c3.x
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
    mov r1.y, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r0.w, r3, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r0.w, -r2.w, -c2.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    mul_pp r0.xyz, r2, r0
    add_pp r1.xyz, r0, r0
    mov_pp oC0, r1

// approximately 88 instruction slots used (7 texture, 81 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyBaseNormDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mad r0.xy, r0, c2.x, c2.y
    mad r0.zw, r1.xyxy, c2.x, c2.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c2.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c2.z, c2.ywzw
    mad r1.xyz, r1, c2.x, c2.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c3.x
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
    mov r1.y, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r0.w, r3, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r0.w, -r2.w, -c2.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    mul_pp r1.xyz, r2, r0
    mov_pp oC0, r1

// approximately 87 instruction slots used (7 texture, 80 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddBaseNormDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mad r0.xy, r0, c2.x, c2.y
    mad r0.zw, r1.xyxy, c2.x, c2.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c2.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c2.z, c2.ywzw
    mad r1.xyz, r1, c2.x, c2.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c3.x
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
    mov r1.y, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r0.w, r3, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r0.w, -r2.w, -c2.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    mad_pp r0.xyz, r2, c2.x, r0
    add_pp r1.xyz, r0, c2.y
    mov_pp oC0, r1

// approximately 88 instruction slots used (7 texture, 81 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyBaseNormDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mad r0.xy, r0, c2.x, c2.y
    mad r0.zw, r1.xyxy, c2.x, c2.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c2.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c2.z, c2.ywzw
    mad r1.xyz, r1, c2.x, c2.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov_pp r0.z, c3.x
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
    mov r1.y, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r0.w, r3, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r0.w, -r2.w, -c2.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    texld_pp r5, t1, s0
    texld_pp r6, t2, s1
    mul_pp r2.xyw, r5.xyzz, r6.xyzz
    mul_pp r5.w, r5.w, c0.w
    mul_pp r1.xyz, r1, r2.xyww
    add_pp r1.xyz, r1, r1
    nrm r6.xyz, r3
    dp3 r1.w, r0, r6
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mad_pp r5.xyz, r0, r0.w, r1
    mov_pp oC0, r5

// approximately 88 instruction slots used (7 texture, 81 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyBaseNormDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mad r0.xy, r0, c2.x, c2.y
    mad r0.zw, r1.xyxy, c2.x, c2.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c2.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c2.z, c2.ywzw
    mad r1.xyz, r1, c2.x, c2.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c3.x
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
    mov r1.y, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r0.w, r3, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r0.w, -r2.w, -c2.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 87 instruction slots used (7 texture, 80 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddBaseNormDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mad r0.xy, r0, c2.x, c2.y
    mad r0.zw, r1.xyxy, c2.x, c2.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c2.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c2.z, c2.ywzw
    mad r1.xyz, r1, c2.x, c2.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c3.x
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
    mov r1.y, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r0.w, r3, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r0.w, -r2.w, -c2.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c2.x, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c2.y
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 88 instruction slots used (7 texture, 81 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyComplexFogBaseNormDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, true, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c7, 2, -1, 3, -2
    def c23, 0, 0, 0, 0
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
    mad r0.xy, r0, c7.x, c7.y
    mad r0.zw, r1.xyxy, c7.x, c7.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c7.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c7.z, c7.ywzw
    mad r1.xyz, r1, c7.x, c7.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c23.x
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
    mov r3.xy, c7
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
    mov r2.xy, t1.zwzw
    add r3.yzw, -t0.xxyz, c8.xxyz
    dp3 r0.w, r3.yzww, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r3.yzww
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r0.w, -r2.w, -c7.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3.yzww
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r4, t1, s0
    mad_pp r0.xyz, r4, r1, r0
    mul_pp r1.w, r4.w, c0.w
    mul_pp r0.xyz, r2, r0
    mul_pp r0.xyz, r0, c1.w
    add r2.xyz, r0, r0
    mad r0.xyz, r0, -r3.x, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 94 instruction slots used (7 texture, 87 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyComplexFogBaseNormDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, true, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c7, 2, -1, 3, -2
    def c23, 0, 0, 0, 0
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
    mad r0.xy, r0, c7.x, c7.y
    mad r0.zw, r1.xyxy, c7.x, c7.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c7.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c7.z, c7.ywzw
    mad r1.xyz, r1, c7.x, c7.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c23.x
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
    mov r1.y, c7.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r0.w, r3, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r0.w, -r2.w, -c7.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    mul_pp r0.xyz, r2, r0
    mul r2.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 93 instruction slots used (7 texture, 86 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddComplexFogBaseNormDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, true, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c7, 2, -1, 3, -2
    def c23, 0, 0, 0, 0
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
    mad r0.xy, r0, c7.x, c7.y
    mad r0.zw, r1.xyxy, c7.x, c7.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c7.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c7.z, c7.ywzw
    mad r1.xyz, r1, c7.x, c7.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c23.x
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
    mov r1.y, c7.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r0.w, r3, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r0.w, -r2.w, -c7.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    mad_pp r0.xyz, r2, c7.x, r0
    add_pp r0.xyz, r0, c7.y
    mul r2.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 94 instruction slots used (7 texture, 87 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyComplexFogBaseNormDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, true, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c7, 2, -1, 3, -2
    def c23, 0, 0, 0, 0
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
    mad r0.xy, r0, c7.x, c7.y
    mad r0.zw, r1.xyxy, c7.x, c7.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c7.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c7.z, c7.ywzw
    mad r1.xyz, r1, c7.x, c7.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov_pp r0.z, c23.x
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
    mov r1.y, c7.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r0.w, r3, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r0.w, -r2.w, -c7.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    texld_pp r5, t1, s0
    texld_pp r6, t2, s1
    mul_pp r2.xyw, r5.xyzz, r6.xyzz
    mul_pp r5.w, r5.w, c0.w
    mul_pp r1.xyz, r1, r2.xyww
    add_pp r1.xyz, r1, r1
    nrm r6.xyz, r3
    dp3 r1.w, r0, r6
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r5.xyz, r0, r1
    mov_pp oC0, r5

// approximately 94 instruction slots used (7 texture, 87 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyComplexFogBaseNormDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, true, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c7, 2, -1, 3, -2
    def c23, 0, 0, 0, 0
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
    mad r0.xy, r0, c7.x, c7.y
    mad r0.zw, r1.xyxy, c7.x, c7.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c7.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c7.z, c7.ywzw
    mad r1.xyz, r1, c7.x, c7.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c23.x
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
    mov r1.y, c7.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r0.w, r3, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r0.w, -r2.w, -c7.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r0.xyz, r2, r1, r0
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 93 instruction slots used (7 texture, 86 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddComplexFogBaseNormDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, true, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c7, 2, -1, 3, -2
    def c23, 0, 0, 0, 0
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
    mad r0.xy, r0, c7.x, c7.y
    mad r0.zw, r1.xyxy, c7.x, c7.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c7.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c7.z, c7.ywzw
    mad r1.xyz, r1, c7.x, c7.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c23.x
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
    mov r1.y, c7.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r0.w, r3, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r0.w, -r2.w, -c7.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c7.x, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c7.y
    mad_pp r0.xyz, r2, r1, r0
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 94 instruction slots used (7 texture, 87 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyBaseNormDetailNormSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mad r0.xy, r0, c2.x, c2.y
    mad r0.zw, r1.xyxy, c2.x, c2.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c2.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c2.z, c2.ywzw
    mad r1.xyz, r1, c2.x, c2.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c3.x
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
    mov r1.y, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r0.w, r3, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r0.w, -r2.w, -c2.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    mul_pp r0.xyz, r2, r0
    add_pp r1.xyz, r0, r0
    mov_pp oC0, r1

// approximately 88 instruction slots used (7 texture, 81 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyBaseNormDetailNormSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mad r0.xy, r0, c2.x, c2.y
    mad r0.zw, r1.xyxy, c2.x, c2.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c2.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c2.z, c2.ywzw
    mad r1.xyz, r1, c2.x, c2.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c3.x
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
    mov r1.y, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r0.w, r3, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r0.w, -r2.w, -c2.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    mul_pp r1.xyz, r2, r0
    mov_pp oC0, r1

// approximately 87 instruction slots used (7 texture, 80 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddBaseNormDetailNormSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mad r0.xy, r0, c2.x, c2.y
    mad r0.zw, r1.xyxy, c2.x, c2.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c2.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c2.z, c2.ywzw
    mad r1.xyz, r1, c2.x, c2.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c3.x
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
    mov r1.y, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r0.w, r3, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r0.w, -r2.w, -c2.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    mad_pp r0.xyz, r2, c2.x, r0
    add_pp r1.xyz, r0, c2.y
    mov_pp oC0, r1

// approximately 88 instruction slots used (7 texture, 81 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyBaseNormDetailNormSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mad r0.xy, r0, c2.x, c2.y
    mad r0.zw, r1.xyxy, c2.x, c2.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c2.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c2.z, c2.ywzw
    mad r1.xyz, r1, c2.x, c2.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov_pp r0.z, c3.x
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
    mov r1.y, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r0.w, r3, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r0.w, -r2.w, -c2.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    texld_pp r5, t1, s0
    texld_pp r6, t2, s1
    mul_pp r2.xyw, r5.xyzz, r6.xyzz
    mul_pp r5.w, r5.w, c0.w
    mul_pp r1.xyz, r1, r2.xyww
    add_pp r1.xyz, r1, r1
    nrm r6.xyz, r3
    dp3 r1.w, r0, r6
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mad_pp r5.xyz, r0, r0.w, r1
    mov_pp oC0, r5

// approximately 88 instruction slots used (7 texture, 81 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyBaseNormDetailNormSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mad r0.xy, r0, c2.x, c2.y
    mad r0.zw, r1.xyxy, c2.x, c2.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c2.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c2.z, c2.ywzw
    mad r1.xyz, r1, c2.x, c2.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c3.x
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
    mov r1.y, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r0.w, r3, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r0.w, -r2.w, -c2.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 87 instruction slots used (7 texture, 80 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddBaseNormDetailNormSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, true, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mad r0.xy, r0, c2.x, c2.y
    mad r0.zw, r1.xyxy, c2.x, c2.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c2.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c2.z, c2.ywzw
    mad r1.xyz, r1, c2.x, c2.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c3.x
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
    mov r1.y, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r0.w, r3, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r0.w, -r2.w, -c2.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c2.x, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c2.y
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 88 instruction slots used (7 texture, 81 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyBaseNormDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mad r0.xy, r0, c2.x, c2.y
    mad r0.zw, r1.xyxy, c2.x, c2.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c2.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c2.z, c2.ywzw
    mad r1.xyz, r1, c2.x, c2.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c3.x
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
    mov r1.y, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r0.w, -r2.w, -c2.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r0.w, r2.xyww, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mul_pp r0.xyz, r0, r2
    add_pp r1.xyz, r0, r0
    mov_pp oC0, r1

// approximately 98 instruction slots used (8 texture, 90 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyBaseNormDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mad r0.xy, r0, c2.x, c2.y
    mad r0.zw, r1.xyxy, c2.x, c2.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c2.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c2.z, c2.ywzw
    mad r1.xyz, r1, c2.x, c2.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c3.x
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
    mov r1.y, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r0.w, -r2.w, -c2.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r0.w, r2.xyww, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mul_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 97 instruction slots used (8 texture, 89 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddBaseNormDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mad r0.xy, r0, c2.x, c2.y
    mad r0.zw, r1.xyxy, c2.x, c2.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c2.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c2.z, c2.ywzw
    mad r1.xyz, r1, c2.x, c2.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c3.x
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
    mov r1.y, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r0.w, -r2.w, -c2.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r0.w, r2.xyww, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mad_pp r0.xyz, r2, c2.x, r0
    add_pp r1.xyz, r0, c2.y
    mov_pp oC0, r1

// approximately 98 instruction slots used (8 texture, 90 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyBaseNormDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mad r0.xy, r0, c2.x, c2.y
    mad r0.zw, r1.xyxy, c2.x, c2.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c2.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c2.z, c2.ywzw
    mad r1.xyz, r1, c2.x, c2.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov_pp r0.z, c3.x
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
    mov r1.y, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r0.w, -r2.w, -c2.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mul_pp r2.xyw, r3.xyzz, r4.xyzz
    mul_pp r3.w, r3.w, c0.w
    mul_pp r1.xyz, r1, r2.xyww
    add_pp r1.xyz, r1, r1
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r0.w, r2.xyww, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mad_pp r3.xyz, r0, r0.w, r1
    mov_pp oC0, r3

// approximately 98 instruction slots used (8 texture, 90 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyBaseNormDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mad r0.xy, r0, c2.x, c2.y
    mad r0.zw, r1.xyxy, c2.x, c2.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c2.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c2.z, c2.ywzw
    mad r1.xyz, r1, c2.x, c2.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c3.x
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
    mov r1.y, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r0.w, -r2.w, -c2.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r0.w, r2.xyww, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2.xyww
    nrm r4.xyz, r2.xyww
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r4, t1, s7
    texld_pp r3, r3, s3
    log r5.x, r4.x
    log r5.y, r4.y
    log r5.z, r4.z
    mul r2.xyw, r5.xyzz, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.w
    mul_sat_pp r2.xyw, r4.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r3
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 97 instruction slots used (8 texture, 89 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddBaseNormDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mad r0.xy, r0, c2.x, c2.y
    mad r0.zw, r1.xyxy, c2.x, c2.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c2.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c2.z, c2.ywzw
    mad r1.xyz, r1, c2.x, c2.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c3.x
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
    mov r1.y, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r0.w, -r2.w, -c2.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r0.w, r2.xyww, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2.xyww
    nrm r4.xyz, r2.xyww
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r4, t1, s7
    texld_pp r3, r3, s3
    log r5.x, r4.x
    log r5.y, r4.y
    log r5.z, r4.z
    mul r2.xyw, r5.xyzz, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.w
    mul_sat_pp r2.xyw, r4.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r3
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c2.x, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c2.y
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 98 instruction slots used (8 texture, 90 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyComplexFogBaseNormDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, true, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c7, 2, -1, 3, -2
    def c23, 0, 0, 0, 0
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
    mad r0.xy, r0, c7.x, c7.y
    mad r0.zw, r1.xyxy, c7.x, c7.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c7.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c7.z, c7.ywzw
    mad r1.xyz, r1, c7.x, c7.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c23.x
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
    mov r3.xy, c7
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
    mov r2.xy, t1.zwzw
    texld_pp r4, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r0.w, -r2.w, -c7.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r0.w, r2.xyww, r0
    mul r3.yzw, r0.xxyz, r0.w
    mad_pp r5.xyz, r3.yzww, c8.w, -r2.xyww
    nrm r6.xyz, r2.xyww
    dp3 r1.w, r0, r6
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r6, t1, s7
    texld_pp r5, r5, s3
    log r7.x, r6.x
    log r7.y, r6.y
    log r7.z, r6.z
    mul r2.xyw, r7.xyzz, c5.z
    exp r6.x, r2.x
    exp r6.y, r2.y
    exp r6.z, r2.w
    mul_sat_pp r2.xyw, r6.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r5
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r4, r1, r0
    mul_pp r1.w, r4.w, c0.w
    texld_pp r2, t2, s1
    mul_pp r0.xyz, r0, r2
    mul_pp r0.xyz, r0, c1.w
    add r2.xyz, r0, r0
    mad r0.xyz, r0, -r3.x, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 104 instruction slots used (8 texture, 96 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyComplexFogBaseNormDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, true, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c7, 2, -1, 3, -2
    def c23, 0, 0, 0, 0
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
    mad r0.xy, r0, c7.x, c7.y
    mad r0.zw, r1.xyxy, c7.x, c7.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c7.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c7.z, c7.ywzw
    mad r1.xyz, r1, c7.x, c7.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c23.x
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
    mov r1.y, c7.y
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
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r0.w, -r2.w, -c7.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r0.w, r2.xyww, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mul_pp r0.xyz, r0, r2
    mul r2.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 103 instruction slots used (8 texture, 95 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddComplexFogBaseNormDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, true, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c7, 2, -1, 3, -2
    def c23, 0, 0, 0, 0
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
    mad r0.xy, r0, c7.x, c7.y
    mad r0.zw, r1.xyxy, c7.x, c7.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c7.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c7.z, c7.ywzw
    mad r1.xyz, r1, c7.x, c7.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c23.x
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
    mov r1.y, c7.y
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
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r0.w, -r2.w, -c7.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r0.w, r2.xyww, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mad_pp r0.xyz, r2, c7.x, r0
    add_pp r0.xyz, r0, c7.y
    mul r2.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 104 instruction slots used (8 texture, 96 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyComplexFogBaseNormDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, true, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c7, 2, -1, 3, -2
    def c23, 0, 0, 0, 0
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
    mad r0.xy, r0, c7.x, c7.y
    mad r0.zw, r1.xyxy, c7.x, c7.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c7.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c7.z, c7.ywzw
    mad r1.xyz, r1, c7.x, c7.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov_pp r0.z, c23.x
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
    mov r1.y, c7.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r0.w, -r2.w, -c7.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mul_pp r2.xyw, r3.xyzz, r4.xyzz
    mul_pp r3.w, r3.w, c0.w
    mul_pp r1.xyz, r1, r2.xyww
    add_pp r1.xyz, r1, r1
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r0.w, r2.xyww, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 104 instruction slots used (8 texture, 96 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyComplexFogBaseNormDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, true, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c7, 2, -1, 3, -2
    def c23, 0, 0, 0, 0
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
    mad r0.xy, r0, c7.x, c7.y
    mad r0.zw, r1.xyxy, c7.x, c7.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c7.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c7.z, c7.ywzw
    mad r1.xyz, r1, c7.x, c7.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c23.x
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
    mov r1.y, c7.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r0.w, -r2.w, -c7.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r0.w, r2.xyww, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2.xyww
    nrm r4.xyz, r2.xyww
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r4, t1, s7
    texld_pp r3, r3, s3
    log r5.x, r4.x
    log r5.y, r4.y
    log r5.z, r4.z
    mul r2.xyw, r5.xyzz, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.w
    mul_sat_pp r2.xyw, r4.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r3
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r0.xyz, r2, r1, r0
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 103 instruction slots used (8 texture, 95 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddComplexFogBaseNormDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, true, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c7, 2, -1, 3, -2
    def c23, 0, 0, 0, 0
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
    mad r0.xy, r0, c7.x, c7.y
    mad r0.zw, r1.xyxy, c7.x, c7.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c7.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c7.z, c7.ywzw
    mad r1.xyz, r1, c7.x, c7.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c23.x
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
    mov r1.y, c7.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r0.w, -r2.w, -c7.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r0.w, r2.xyww, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2.xyww
    nrm r4.xyz, r2.xyww
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r4, t1, s7
    texld_pp r3, r3, s3
    log r5.x, r4.x
    log r5.y, r4.y
    log r5.z, r4.z
    mul r2.xyw, r5.xyzz, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.w
    mul_sat_pp r2.xyw, r4.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r3
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c7.x, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c7.y
    mad_pp r0.xyz, r2, r1, r0
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 104 instruction slots used (8 texture, 96 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyBaseNormDetailNormSpecMapSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mad r0.xy, r0, c2.x, c2.y
    mad r0.zw, r1.xyxy, c2.x, c2.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c2.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c2.z, c2.ywzw
    mad r1.xyz, r1, c2.x, c2.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c3.x
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
    mov r1.y, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r0.w, -r2.w, -c2.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r0.w, r2.xyww, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mul_pp r0.xyz, r0, r2
    add_pp r1.xyz, r0, r0
    mov_pp oC0, r1

// approximately 98 instruction slots used (8 texture, 90 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyBaseNormDetailNormSpecMapSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mad r0.xy, r0, c2.x, c2.y
    mad r0.zw, r1.xyxy, c2.x, c2.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c2.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c2.z, c2.ywzw
    mad r1.xyz, r1, c2.x, c2.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c3.x
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
    mov r1.y, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r0.w, -r2.w, -c2.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r0.w, r2.xyww, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mul_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 97 instruction slots used (8 texture, 89 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddBaseNormDetailNormSpecMapSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mad r0.xy, r0, c2.x, c2.y
    mad r0.zw, r1.xyxy, c2.x, c2.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c2.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c2.z, c2.ywzw
    mad r1.xyz, r1, c2.x, c2.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c3.x
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
    mov r1.y, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r0.w, -r2.w, -c2.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r0.w, r2.xyww, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mad_pp r0.xyz, r2, c2.x, r0
    add_pp r1.xyz, r0, c2.y
    mov_pp oC0, r1

// approximately 98 instruction slots used (8 texture, 90 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyBaseNormDetailNormSpecMapSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mad r0.xy, r0, c2.x, c2.y
    mad r0.zw, r1.xyxy, c2.x, c2.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c2.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c2.z, c2.ywzw
    mad r1.xyz, r1, c2.x, c2.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov_pp r0.z, c3.x
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
    mov r1.y, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r0.w, -r2.w, -c2.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mul_pp r2.xyw, r3.xyzz, r4.xyzz
    mul_pp r3.w, r3.w, c0.w
    mul_pp r1.xyz, r1, r2.xyww
    add_pp r1.xyz, r1, r1
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r0.w, r2.xyww, r0
    mul r4.xyz, r0, r0.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mad_pp r3.xyz, r0, r0.w, r1
    mov_pp oC0, r3

// approximately 98 instruction slots used (8 texture, 90 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyBaseNormDetailNormSpecMapSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mad r0.xy, r0, c2.x, c2.y
    mad r0.zw, r1.xyxy, c2.x, c2.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c2.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c2.z, c2.ywzw
    mad r1.xyz, r1, c2.x, c2.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c3.x
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
    mov r1.y, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r0.w, -r2.w, -c2.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r0.w, r2.xyww, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2.xyww
    nrm r4.xyz, r2.xyww
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r4, t1, s7
    texld_pp r3, r3, s3
    log r5.x, r4.x
    log r5.y, r4.y
    log r5.z, r4.z
    mul r2.xyw, r5.xyzz, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.w
    mul_sat_pp r2.xyw, r4.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r3
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 97 instruction slots used (8 texture, 89 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddBaseNormDetailNormSpecMapSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, true, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mad r0.xy, r0, c2.x, c2.y
    mad r0.zw, r1.xyxy, c2.x, c2.y
    mul r0, r0, c6.yyxx
    mul r0.zw, r0, c6.z
    texld r1, t1, s4
    mul r2.w, r1.w, c2.z
    mov_sat r2.w, r2.w
    mul r0.zw, r0, r2.w
    mad r0.xy, r0, c6.w, -r0.zwzw
    mad_sat r2.xy, r1.w, c2.z, c2.ywzw
    mad r1.xyz, r1, c2.x, c2.y
    mad r0.xy, r2.x, r0, r0.zwzw
    mad r0.xy, r2.y, -r0, r0
    mov r0.z, c3.x
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
    mov r1.y, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r0.w, -r2.w, -c2.y
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r0.w, r2.xyww, r0
    mul r3.xyz, r0, r0.w
    mad_pp r3.xyz, r3, c8.w, -r2.xyww
    nrm r4.xyz, r2.xyww
    dp3 r1.w, r0, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r4, t1, s7
    texld_pp r3, r3, s3
    log r5.x, r4.x
    log r5.y, r4.y
    log r5.z, r4.z
    mul r2.xyw, r5.xyzz, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.w
    mul_sat_pp r2.xyw, r4.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r3
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c2.x, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c2.y
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 98 instruction slots used (8 texture, 90 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyBaseNormDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mov r1.w, c2.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c2.x, c2.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c2.x, c2.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c2.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c2.z, c2.xyyw
    mad r3.xyz, r3, c2.x, c2.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    mad_pp r2.xyz, r7.y, c1, r2
    add_pp r1.w, -r7.w, -c2.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r1.w, r6.w, r7.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    mul_pp r2.w, r8.w, c0.w
    dp3 r5.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r5.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mul_pp r0.xyz, r3, r0
    add_pp r2.xyz, r0, r0
    mov_pp oC0, r2

// approximately 134 instruction slots used (7 texture, 127 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyBaseNormDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mov r1.w, c2.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c2.x, c2.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c2.x, c2.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c2.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c2.z, c2.xyyw
    mad r3.xyz, r3, c2.x, c2.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov_pp r2.z, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    mad_pp r2.xyz, r7.y, c1, r2
    add_pp r1.w, -r7.w, -c2.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r1.w, r6.w, r7.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    mul_pp r2.w, r8.w, c0.w
    dp3 r5.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r5.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mul_pp r2.xyz, r3, r0
    mov_pp oC0, r2

// approximately 133 instruction slots used (7 texture, 126 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddBaseNormDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mov r1.w, c2.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c2.x, c2.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c2.x, c2.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c2.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c2.z, c2.xyyw
    mad r3.xyz, r3, c2.x, c2.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    mad_pp r2.xyz, r7.y, c1, r2
    add_pp r1.w, -r7.w, -c2.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r1.w, r6.w, r7.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    mul_pp r2.w, r8.w, c0.w
    dp3 r5.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r5.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mad_pp r0.xyz, r3, c2.x, r0
    add_pp r2.xyz, r0, c2.y
    mov_pp oC0, r2

// approximately 134 instruction slots used (7 texture, 127 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyBaseNormDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mov r1.w, c2.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c2.x, c2.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c2.x, c2.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c2.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c2.z, c2.xyyw
    mad r3.xyz, r3, c2.x, c2.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mul_sat_pp r1.xyz, r6, r1
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    mov r3.xy, t1.zwzw
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, r3, s2
    texld_pp r0, r0, s3
    mad_pp r2.xyz, r3.y, c1, r2
    add_pp r0.w, -r3.w, -c2.y
    mad_pp r3.xyw, r3.w, c0.xyzz, r0.w
    mul_pp r2.xyz, r2, r3.xyww
    texld_pp r4, t1, s0
    texld_pp r5, t2, s1
    mul_pp r3.xyw, r4.xyzz, r5.xyzz
    mul_pp r4.w, r4.w, c0.w
    mul_pp r2.xyz, r2, r3.xyww
    add_pp r2.xyz, r2, r2
    mad_pp r1.xyz, r1, r3.z, r2
    mul_pp r0.w, r6.w, r3.z
    mul_pp r0.xyz, r6, r0
    mad_pp r4.xyz, r0, r0.w, r1
    mov_pp oC0, r4

// approximately 134 instruction slots used (7 texture, 127 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyBaseNormDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mov r1.w, c2.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c2.x, c2.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c2.x, c2.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c2.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c2.z, c2.xyyw
    mad r3.xyz, r3, c2.x, c2.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov_pp r2.z, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    mad_pp r2.xyz, r5.y, c1, r2
    add_pp r0.w, -r5.w, -c2.y
    mad_pp r3.xyz, r5.w, c0, r0.w
    mul_pp r0.w, r6.w, r5.z
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mul_pp r3.xyz, r3, r4
    mul_pp r4.w, r3.w, c0.w
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r0.xyz, r6, r0
    mad_pp r4.xyz, r0, r0.w, r1
    mov_pp oC0, r4

// approximately 133 instruction slots used (7 texture, 126 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddBaseNormDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mov r1.w, c2.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c2.x, c2.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c2.x, c2.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c2.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c2.z, c2.xyyw
    mad r3.xyz, r3, c2.x, c2.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    mad_pp r2.xyz, r5.y, c1, r2
    add_pp r0.w, -r5.w, -c2.y
    mad_pp r3.xyz, r5.w, c0, r0.w
    mul_pp r0.w, r6.w, r5.z
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mad_pp r3.xyz, r4, c2.x, r3
    mul_pp r4.w, r3.w, c0.w
    add_pp r3.xyz, r3, c2.y
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r0.xyz, r6, r0
    mad_pp r4.xyz, r0, r0.w, r1
    mov_pp oC0, r4

// approximately 134 instruction slots used (7 texture, 127 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyComplexFogBaseNormDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, true, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c23, 2, -1, 3, -2
    def c24, 0, 0, 0, 0
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
    mov r0.xy, c23
    mad r1.w, r0.w, -c12.w, -r0.y
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.zw, r2.xyxy, c23.x, c23.y
    mul r0.zw, r0, c6.y
    mad r2.xy, r3, c23.x, c23.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r2.z, r3.w, c23.z
    mov_sat r2.z, r2.z
    mul r2.xy, r2, r2.z
    mad r0.zw, r0, c6.w, -r2.xyxy
    mad_sat r2.zw, r3.w, c23.z, c23.xyyw
    mad r3.xyz, r3, c23.x, c23.y
    mad r0.zw, r2.z, r0, r2.xyxy
    mad r2.xy, r2.w, -r0.zwzw, r0.zwzw
    mov r2.z, c24.x
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
    max r2.w, r1.w, c24.x
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r0.yzw, r1.w, c17.xxyz
    mul r0.yzw, r4.y, r0
    dp3 r1.w, -r1, r2
    add r1.w, r1.w, r1.w
    mad r1.xyz, r2, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r3.w, r2.w, c24.x
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
    max r3.w, r2.w, c24.x
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
    max r3.w, r2.w, c24.x
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
    mad_pp r1.xyz, r7.y, c4, r1
    add_pp r1.w, -r7.w, -c23.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r1.w, r6.w, r7.z
    mul_pp r1.xyz, r1, r3
    mad_pp r0.yzw, r8.xxyz, r1.xxyz, r0
    mul_pp r3.w, r8.w, c0.w
    dp3 r2.w, r5, r2
    mul r1.xyz, r2, r2.w
    mad_pp r1.xyz, r1, c8.w, -r5
    texld_pp r2, t2, s1
    texld_pp r4, r1, s3
    mul_pp r1.xyz, r6, r4
    mad_pp r0.yzw, r1.xxyz, r1.w, r0
    mul_pp r0.yzw, r2.xxyz, r0
    mul_pp r0.yzw, r0, c1.w
    add r1.xyz, r0.yzww, r0.yzww
    mad r0.xyz, r0.yzww, -r0.x, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 140 instruction slots used (7 texture, 133 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyComplexFogBaseNormDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, true, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c23, 2, -1, 3, -2
    def c24, 0, 0, 0, 0
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
    mov r1.w, c23.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c23.x, c23.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c23.x, c23.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c23.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c23.z, c23.xyyw
    mad r3.xyz, r3, c23.x, c23.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov_pp r2.z, c24.x
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
    max r3.w, r1.w, c24.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c24.x
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
    max r3.w, r1.w, c24.x
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
    max r3.w, r1.w, c24.x
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
    mad_pp r2.xyz, r7.y, c4, r2
    add_pp r1.w, -r7.w, -c23.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r1.w, r6.w, r7.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    mul_pp r2.w, r8.w, c0.w
    dp3 r5.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r5.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mul_pp r0.xyz, r3, r0
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 139 instruction slots used (7 texture, 132 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddComplexFogBaseNormDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, true, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c23, 2, -1, 3, -2
    def c24, 0, 0, 0, 0
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
    mov r1.w, c23.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c23.x, c23.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c23.x, c23.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c23.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c23.z, c23.xyyw
    mad r3.xyz, r3, c23.x, c23.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c24.x
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
    max r3.w, r1.w, c24.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c24.x
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
    max r3.w, r1.w, c24.x
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
    max r3.w, r1.w, c24.x
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
    mad_pp r2.xyz, r7.y, c4, r2
    add_pp r1.w, -r7.w, -c23.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r1.w, r6.w, r7.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    mul_pp r2.w, r8.w, c0.w
    dp3 r5.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r5.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mad_pp r0.xyz, r3, c23.x, r0
    add_pp r0.xyz, r0, c23.y
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 140 instruction slots used (7 texture, 133 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyComplexFogBaseNormDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, true, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c23, 2, -1, 3, -2
    def c24, 0, 0, 0, 0
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
    mov r1.w, c23.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c23.x, c23.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c23.x, c23.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c23.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c23.z, c23.xyyw
    mad r3.xyz, r3, c23.x, c23.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c24.x
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
    max r3.w, r1.w, c24.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c24.x
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
    max r3.w, r1.w, c24.x
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
    max r3.w, r1.w, c24.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mul_sat_pp r1.xyz, r6, r1
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    mov r3.xy, t1.zwzw
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, r3, s2
    texld_pp r0, r0, s3
    mad_pp r2.xyz, r3.y, c4, r2
    add_pp r0.w, -r3.w, -c23.y
    mad_pp r3.xyw, r3.w, c0.xyzz, r0.w
    mul_pp r2.xyz, r2, r3.xyww
    texld_pp r4, t1, s0
    texld_pp r5, t2, s1
    mul_pp r3.xyw, r4.xyzz, r5.xyzz
    mul_pp r4.w, r4.w, c0.w
    mul_pp r2.xyz, r2, r3.xyww
    add_pp r2.xyz, r2, r2
    mad_pp r1.xyz, r1, r3.z, r2
    mul_pp r0.w, r6.w, r3.z
    mul_pp r0.xyz, r6, r0
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r4.xyz, r0, r1
    mov_pp oC0, r4

// approximately 140 instruction slots used (7 texture, 133 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyComplexFogBaseNormDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, true, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c23, 2, -1, 3, -2
    def c24, 0, 0, 0, 0
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
    mov r1.w, c23.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c23.x, c23.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c23.x, c23.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c23.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c23.z, c23.xyyw
    mad r3.xyz, r3, c23.x, c23.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov_pp r2.z, c24.x
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
    max r3.w, r1.w, c24.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c24.x
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
    max r3.w, r1.w, c24.x
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
    max r3.w, r1.w, c24.x
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
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    mad_pp r2.xyz, r5.y, c4, r2
    add_pp r0.w, -r5.w, -c23.y
    mad_pp r3.xyz, r5.w, c0, r0.w
    mul_pp r0.w, r6.w, r5.z
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mul_pp r3.xyz, r3, r4
    mul_pp r4.w, r3.w, c0.w
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r0.xyz, r6, r0
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r4.xyz, r0, r1
    mov_pp oC0, r4

// approximately 139 instruction slots used (7 texture, 132 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddComplexFogBaseNormDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, true, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c23, 2, -1, 3, -2
    def c24, 0, 0, 0, 0
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
    mov r1.w, c23.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c23.x, c23.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c23.x, c23.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c23.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c23.z, c23.xyyw
    mad r3.xyz, r3, c23.x, c23.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c24.x
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
    max r3.w, r1.w, c24.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c24.x
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
    max r3.w, r1.w, c24.x
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
    max r3.w, r1.w, c24.x
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
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    mad_pp r2.xyz, r5.y, c4, r2
    add_pp r0.w, -r5.w, -c23.y
    mad_pp r3.xyz, r5.w, c0, r0.w
    mul_pp r0.w, r6.w, r5.z
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mad_pp r3.xyz, r4, c23.x, r3
    mul_pp r4.w, r3.w, c0.w
    add_pp r3.xyz, r3, c23.y
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r0.xyz, r6, r0
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r4.xyz, r0, r1
    mov_pp oC0, r4

// approximately 140 instruction slots used (7 texture, 133 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyBaseNormDetailNormSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mov r1.w, c2.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c2.x, c2.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c2.x, c2.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c2.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c2.z, c2.xyyw
    mad r3.xyz, r3, c2.x, c2.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    mad_pp r2.xyz, r7.y, c1, r2
    add_pp r1.w, -r7.w, -c2.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r1.w, r6.w, r7.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    mul_pp r2.w, r8.w, c0.w
    dp3 r5.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r5.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mul_pp r0.xyz, r3, r0
    add_pp r2.xyz, r0, r0
    mov_pp oC0, r2

// approximately 134 instruction slots used (7 texture, 127 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyBaseNormDetailNormSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mov r1.w, c2.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c2.x, c2.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c2.x, c2.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c2.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c2.z, c2.xyyw
    mad r3.xyz, r3, c2.x, c2.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov_pp r2.z, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    mad_pp r2.xyz, r7.y, c1, r2
    add_pp r1.w, -r7.w, -c2.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r1.w, r6.w, r7.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    mul_pp r2.w, r8.w, c0.w
    dp3 r5.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r5.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mul_pp r2.xyz, r3, r0
    mov_pp oC0, r2

// approximately 133 instruction slots used (7 texture, 126 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddBaseNormDetailNormSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mov r1.w, c2.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c2.x, c2.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c2.x, c2.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c2.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c2.z, c2.xyyw
    mad r3.xyz, r3, c2.x, c2.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    mad_pp r2.xyz, r7.y, c1, r2
    add_pp r1.w, -r7.w, -c2.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r1.w, r6.w, r7.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    mul_pp r2.w, r8.w, c0.w
    dp3 r5.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r5.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mad_pp r0.xyz, r3, c2.x, r0
    add_pp r2.xyz, r0, c2.y
    mov_pp oC0, r2

// approximately 134 instruction slots used (7 texture, 127 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyBaseNormDetailNormSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mov r1.w, c2.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c2.x, c2.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c2.x, c2.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c2.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c2.z, c2.xyyw
    mad r3.xyz, r3, c2.x, c2.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r7.xyz, r1.w, c21
    mad_pp r1.xyz, r7, r4.y, r1
    mul_sat_pp r1.xyz, r6, r1
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    mov r3.xy, t1.zwzw
    dp3 r1.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, r3, s2
    texld_pp r0, r0, s3
    mad_pp r2.xyz, r3.y, c1, r2
    add_pp r0.w, -r3.w, -c2.y
    mad_pp r3.xyw, r3.w, c0.xyzz, r0.w
    mul_pp r2.xyz, r2, r3.xyww
    texld_pp r4, t1, s0
    texld_pp r5, t2, s1
    mul_pp r3.xyw, r4.xyzz, r5.xyzz
    mul_pp r4.w, r4.w, c0.w
    mul_pp r2.xyz, r2, r3.xyww
    add_pp r2.xyz, r2, r2
    mad_pp r1.xyz, r1, r3.z, r2
    mul_pp r0.w, r6.w, r3.z
    mul_pp r0.xyz, r6, r0
    mad_pp r4.xyz, r0, r0.w, r1
    mov_pp oC0, r4

// approximately 134 instruction slots used (7 texture, 127 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyBaseNormDetailNormSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mov r1.w, c2.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c2.x, c2.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c2.x, c2.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c2.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c2.z, c2.xyyw
    mad r3.xyz, r3, c2.x, c2.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov_pp r2.z, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    mad_pp r2.xyz, r5.y, c1, r2
    add_pp r0.w, -r5.w, -c2.y
    mad_pp r3.xyz, r5.w, c0, r0.w
    mul_pp r0.w, r6.w, r5.z
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mul_pp r3.xyz, r3, r4
    mul_pp r4.w, r3.w, c0.w
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r0.xyz, r6, r0
    mad_pp r4.xyz, r0, r0.w, r1
    mov_pp oC0, r4

// approximately 133 instruction slots used (7 texture, 126 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddBaseNormDetailNormSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, true, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mov r1.w, c2.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c2.x, c2.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c2.x, c2.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c2.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c2.z, c2.xyyw
    mad r3.xyz, r3, c2.x, c2.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    mad_pp r2.xzw, r2.x, r3.xyyz, c22.xyyz
    mad_pp r2.xyz, r2.y, c17, r2.xzww
    mad_pp r2.xyz, r4.x, c19, r2
    mad_pp r2.xyz, r4.y, c21, r2
    mad_pp r2.xyz, r5.y, c1, r2
    add_pp r0.w, -r5.w, -c2.y
    mad_pp r3.xyz, r5.w, c0, r0.w
    mul_pp r0.w, r6.w, r5.z
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mad_pp r3.xyz, r4, c2.x, r3
    mul_pp r4.w, r3.w, c0.w
    add_pp r3.xyz, r3, c2.y
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r0.xyz, r6, r0
    mad_pp r4.xyz, r0, r0.w, r1
    mov_pp oC0, r4

// approximately 134 instruction slots used (7 texture, 127 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyBaseNormDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mov r1.w, c2.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c2.x, c2.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c2.x, c2.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c2.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c2.z, c2.xyyw
    mad r3.xyz, r3, c2.x, c2.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    mad_pp r2.xyz, r7.y, c1, r2
    add_pp r1.w, -r7.w, -c2.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r1.w, r6.w, r7.z
    mul_pp r2.xyz, r2, r3
    dp3 r2.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r2.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r2.w, r3.w, c0.w
    mul_pp r0.xyz, r6, r0
    mad_pp r0.xyz, r0, r1.w, r1
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    add_pp r2.xyz, r0, r0
    mov_pp oC0, r2

// approximately 144 instruction slots used (8 texture, 136 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyBaseNormDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mov r1.w, c2.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c2.x, c2.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c2.x, c2.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c2.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c2.z, c2.xyyw
    mad r3.xyz, r3, c2.x, c2.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov_pp r2.z, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    mad_pp r2.xyz, r7.y, c1, r2
    add_pp r1.w, -r7.w, -c2.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r1.w, r6.w, r7.z
    mul_pp r2.xyz, r2, r3
    dp3 r2.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r2.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r2.w, r3.w, c0.w
    mul_pp r0.xyz, r6, r0
    mad_pp r0.xyz, r0, r1.w, r1
    texld_pp r1, t2, s1
    mul_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 143 instruction slots used (8 texture, 135 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddBaseNormDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mov r1.w, c2.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c2.x, c2.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c2.x, c2.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c2.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c2.z, c2.xyyw
    mad r3.xyz, r3, c2.x, c2.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    mad_pp r2.xyz, r7.y, c1, r2
    add_pp r1.w, -r7.w, -c2.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r1.w, r6.w, r7.z
    mul_pp r2.xyz, r2, r3
    dp3 r2.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r2.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r2.w, r3.w, c0.w
    mul_pp r0.xyz, r6, r0
    mad_pp r0.xyz, r0, r1.w, r1
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c2.x, r0
    add_pp r2.xyz, r0, c2.y
    mov_pp oC0, r2

// approximately 144 instruction slots used (8 texture, 136 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyBaseNormDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mov r1.w, c2.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c2.x, c2.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c2.x, c2.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c2.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c2.z, c2.xyyw
    mad r3.xyz, r3, c2.x, c2.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    mad_pp r2.xyz, r7.y, c1, r2
    add_pp r1.w, -r7.w, -c2.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mul_pp r3.xyz, r3, r4
    mul_pp r4.w, r3.w, c0.w
    mul_pp r2.xyz, r2, r3
    add_pp r2.xyz, r2, r2
    mad_pp r1.xyz, r1, r7.z, r2
    mul_pp r1.w, r6.w, r7.z
    dp3 r5.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r5.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mad_pp r4.xyz, r0, r1.w, r1
    mov_pp oC0, r4

// approximately 144 instruction slots used (8 texture, 136 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyBaseNormDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mov r1.w, c2.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c2.x, c2.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c2.x, c2.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c2.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c2.z, c2.xyyw
    mad r3.xyz, r3, c2.x, c2.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov_pp r2.z, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    mad_pp r2.xyz, r7.y, c1, r2
    add_pp r1.w, -r7.w, -c2.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r1.w, r6.w, r7.z
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mul_pp r3.xyz, r3, r4
    mul_pp r4.w, r3.w, c0.w
    mad_pp r1.xyz, r3, r2, r1
    dp3 r5.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r5.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mad_pp r4.xyz, r0, r1.w, r1
    mov_pp oC0, r4

// approximately 143 instruction slots used (8 texture, 135 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddBaseNormDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mov r1.w, c2.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c2.x, c2.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c2.x, c2.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c2.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c2.z, c2.xyyw
    mad r3.xyz, r3, c2.x, c2.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    mad_pp r2.xyz, r7.y, c1, r2
    add_pp r1.w, -r7.w, -c2.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r1.w, r6.w, r7.z
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mad_pp r3.xyz, r4, c2.x, r3
    mul_pp r4.w, r3.w, c0.w
    add_pp r3.xyz, r3, c2.y
    mad_pp r1.xyz, r3, r2, r1
    dp3 r5.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r5.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mad_pp r4.xyz, r0, r1.w, r1
    mov_pp oC0, r4

// approximately 144 instruction slots used (8 texture, 136 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyComplexFogBaseNormDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, true, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c23, 2, -1, 3, -2
    def c24, 0, 0, 0, 0
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
    mov r0.xy, c23
    mad r1.w, r0.w, -c12.w, -r0.y
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.zw, r2.xyxy, c23.x, c23.y
    mul r0.zw, r0, c6.y
    mad r2.xy, r3, c23.x, c23.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r2.z, r3.w, c23.z
    mov_sat r2.z, r2.z
    mul r2.xy, r2, r2.z
    mad r0.zw, r0, c6.w, -r2.xyxy
    mad_sat r2.zw, r3.w, c23.z, c23.xyyw
    mad r3.xyz, r3, c23.x, c23.y
    mad r0.zw, r2.z, r0, r2.xyxy
    mad r2.xy, r2.w, -r0.zwzw, r0.zwzw
    mov r2.z, c24.x
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
    max r2.w, r1.w, c24.x
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r0.yzw, r1.w, c17.xxyz
    mul r0.yzw, r4.y, r0
    dp3 r1.w, -r1, r2
    add r1.w, r1.w, r1.w
    mad r1.xyz, r2, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r3.w, r2.w, c24.x
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
    max r3.w, r2.w, c24.x
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
    max r3.w, r2.w, c24.x
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
    mad_pp r1.xyz, r7.y, c4, r1
    add_pp r1.w, -r7.w, -c23.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r1.w, r6.w, r7.z
    mul_pp r1.xyz, r1, r3
    dp3 r2.w, r5, r2
    mul r2.xyz, r2, r2.w
    mad_pp r2.xyz, r2, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mad_pp r0.yzw, r3.xxyz, r1.xxyz, r0
    mul_pp r3.w, r3.w, c0.w
    mul_pp r1.xyz, r6, r2
    mad_pp r0.yzw, r1.xxyz, r1.w, r0
    texld_pp r1, t2, s1
    mul_pp r0.yzw, r0, r1.xxyz
    mul_pp r0.yzw, r0, c1.w
    add r1.xyz, r0.yzww, r0.yzww
    mad r0.xyz, r0.yzww, -r0.x, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 150 instruction slots used (8 texture, 142 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyComplexFogBaseNormDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, true, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c23, 2, -1, 3, -2
    def c24, 0, 0, 0, 0
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
    mov r1.w, c23.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c23.x, c23.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c23.x, c23.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c23.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c23.z, c23.xyyw
    mad r3.xyz, r3, c23.x, c23.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov_pp r2.z, c24.x
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
    max r3.w, r1.w, c24.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c24.x
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
    max r3.w, r1.w, c24.x
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
    max r3.w, r1.w, c24.x
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
    mad_pp r2.xyz, r7.y, c4, r2
    add_pp r1.w, -r7.w, -c23.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r1.w, r6.w, r7.z
    mul_pp r2.xyz, r2, r3
    dp3 r2.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r2.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r2.w, r3.w, c0.w
    mul_pp r0.xyz, r6, r0
    mad_pp r0.xyz, r0, r1.w, r1
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 149 instruction slots used (8 texture, 141 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddComplexFogBaseNormDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, true, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c23, 2, -1, 3, -2
    def c24, 0, 0, 0, 0
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
    mov r1.w, c23.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c23.x, c23.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c23.x, c23.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c23.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c23.z, c23.xyyw
    mad r3.xyz, r3, c23.x, c23.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c24.x
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
    max r3.w, r1.w, c24.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c24.x
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
    max r3.w, r1.w, c24.x
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
    max r3.w, r1.w, c24.x
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
    mad_pp r2.xyz, r7.y, c4, r2
    add_pp r1.w, -r7.w, -c23.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r1.w, r6.w, r7.z
    mul_pp r2.xyz, r2, r3
    dp3 r2.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r2.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r2.w, r3.w, c0.w
    mul_pp r0.xyz, r6, r0
    mad_pp r0.xyz, r0, r1.w, r1
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c23.x, r0
    add_pp r0.xyz, r0, c23.y
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 150 instruction slots used (8 texture, 142 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyComplexFogBaseNormDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, true, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c23, 2, -1, 3, -2
    def c24, 0, 0, 0, 0
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
    mov r1.w, c23.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c23.x, c23.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c23.x, c23.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c23.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c23.z, c23.xyyw
    mad r3.xyz, r3, c23.x, c23.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c24.x
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
    max r3.w, r1.w, c24.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c24.x
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
    max r3.w, r1.w, c24.x
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
    max r3.w, r1.w, c24.x
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
    mad_pp r2.xyz, r7.y, c4, r2
    add_pp r1.w, -r7.w, -c23.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mul_pp r3.xyz, r3, r4
    mul_pp r4.w, r3.w, c0.w
    mul_pp r2.xyz, r2, r3
    add_pp r2.xyz, r2, r2
    mad_pp r1.xyz, r1, r7.z, r2
    mul_pp r1.w, r6.w, r7.z
    dp3 r5.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r5.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r4.xyz, r0, r1
    mov_pp oC0, r4

// approximately 150 instruction slots used (8 texture, 142 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyComplexFogBaseNormDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, true, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c23, 2, -1, 3, -2
    def c24, 0, 0, 0, 0
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
    mov r1.w, c23.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c23.x, c23.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c23.x, c23.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c23.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c23.z, c23.xyyw
    mad r3.xyz, r3, c23.x, c23.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov_pp r2.z, c24.x
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
    max r3.w, r1.w, c24.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c24.x
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
    max r3.w, r1.w, c24.x
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
    max r3.w, r1.w, c24.x
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
    mad_pp r2.xyz, r7.y, c4, r2
    add_pp r1.w, -r7.w, -c23.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r1.w, r6.w, r7.z
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mul_pp r3.xyz, r3, r4
    mul_pp r4.w, r3.w, c0.w
    mad_pp r1.xyz, r3, r2, r1
    dp3 r5.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r5.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r4.xyz, r0, r1
    mov_pp oC0, r4

// approximately 149 instruction slots used (8 texture, 141 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddComplexFogBaseNormDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, true, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c23, 2, -1, 3, -2
    def c24, 0, 0, 0, 0
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
    mov r1.w, c23.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c23.x, c23.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c23.x, c23.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c23.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c23.z, c23.xyyw
    mad r3.xyz, r3, c23.x, c23.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c24.x
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
    max r3.w, r1.w, c24.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c24.x
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
    max r3.w, r1.w, c24.x
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
    max r3.w, r1.w, c24.x
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
    mad_pp r2.xyz, r7.y, c4, r2
    add_pp r1.w, -r7.w, -c23.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r1.w, r6.w, r7.z
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mad_pp r3.xyz, r4, c23.x, r3
    mul_pp r4.w, r3.w, c0.w
    add_pp r3.xyz, r3, c23.y
    mad_pp r1.xyz, r3, r2, r1
    dp3 r5.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r5.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r4.xyz, r0, r1
    mov_pp oC0, r4

// approximately 150 instruction slots used (8 texture, 142 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyBaseNormDetailNormSpecMapSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mov r1.w, c2.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c2.x, c2.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c2.x, c2.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c2.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c2.z, c2.xyyw
    mad r3.xyz, r3, c2.x, c2.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    mad_pp r2.xyz, r7.y, c1, r2
    add_pp r1.w, -r7.w, -c2.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r1.w, r6.w, r7.z
    mul_pp r2.xyz, r2, r3
    dp3 r2.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r2.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r2.w, r3.w, c0.w
    mul_pp r0.xyz, r6, r0
    mad_pp r0.xyz, r0, r1.w, r1
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    add_pp r2.xyz, r0, r0
    mov_pp oC0, r2

// approximately 144 instruction slots used (8 texture, 136 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyBaseNormDetailNormSpecMapSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mov r1.w, c2.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c2.x, c2.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c2.x, c2.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c2.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c2.z, c2.xyyw
    mad r3.xyz, r3, c2.x, c2.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov_pp r2.z, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    mad_pp r2.xyz, r7.y, c1, r2
    add_pp r1.w, -r7.w, -c2.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r1.w, r6.w, r7.z
    mul_pp r2.xyz, r2, r3
    dp3 r2.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r2.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r2.w, r3.w, c0.w
    mul_pp r0.xyz, r6, r0
    mad_pp r0.xyz, r0, r1.w, r1
    texld_pp r1, t2, s1
    mul_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 143 instruction slots used (8 texture, 135 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddBaseNormDetailNormSpecMapSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mov r1.w, c2.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c2.x, c2.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c2.x, c2.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c2.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c2.z, c2.xyyw
    mad r3.xyz, r3, c2.x, c2.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    mad_pp r2.xyz, r7.y, c1, r2
    add_pp r1.w, -r7.w, -c2.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r1.w, r6.w, r7.z
    mul_pp r2.xyz, r2, r3
    dp3 r2.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r2.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r2.w, r3.w, c0.w
    mul_pp r0.xyz, r6, r0
    mad_pp r0.xyz, r0, r1.w, r1
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c2.x, r0
    add_pp r2.xyz, r0, c2.y
    mov_pp oC0, r2

// approximately 144 instruction slots used (8 texture, 136 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyBaseNormDetailNormSpecMapSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mov r1.w, c2.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c2.x, c2.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c2.x, c2.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c2.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c2.z, c2.xyyw
    mad r3.xyz, r3, c2.x, c2.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    mad_pp r2.xyz, r7.y, c1, r2
    add_pp r1.w, -r7.w, -c2.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mul_pp r3.xyz, r3, r4
    mul_pp r4.w, r3.w, c0.w
    mul_pp r2.xyz, r2, r3
    add_pp r2.xyz, r2, r2
    mad_pp r1.xyz, r1, r7.z, r2
    mul_pp r1.w, r6.w, r7.z
    dp3 r5.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r5.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mad_pp r4.xyz, r0, r1.w, r1
    mov_pp oC0, r4

// approximately 144 instruction slots used (8 texture, 136 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyBaseNormDetailNormSpecMapSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mov r1.w, c2.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c2.x, c2.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c2.x, c2.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c2.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c2.z, c2.xyyw
    mad r3.xyz, r3, c2.x, c2.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov_pp r2.z, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    mad_pp r2.xyz, r7.y, c1, r2
    add_pp r1.w, -r7.w, -c2.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r1.w, r6.w, r7.z
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mul_pp r3.xyz, r3, r4
    mul_pp r4.w, r3.w, c0.w
    mad_pp r1.xyz, r3, r2, r1
    dp3 r5.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r5.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mad_pp r4.xyz, r0, r1.w, r1
    mov_pp oC0, r4

// approximately 143 instruction slots used (8 texture, 135 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddBaseNormDetailNormSpecMapSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, true, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 3, -2
    def c3, 0, 0, 0, 0
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
    mov r1.w, c2.y
    mad r0.x, r0.w, -c12.w, -r1.w
    mov r2.xy, t6.zwzw
    texld r3, t6, s5
    texld r2, r2, s6
    mad r0.yz, r2.xxyw, c2.x, c2.y
    mul r0.yz, r0, c6.y
    mad r2.xy, r3, c2.x, c2.y
    mul r2.xy, r2, c6.x
    mul r2.xy, r2, c6.z
    texld r3, t1, s4
    mul r0.w, r3.w, c2.z
    mov_sat r0.w, r0.w
    mul r2.xy, r2, r0.w
    mad r0.yz, r0, c6.w, -r2.xxyw
    mad_sat r2.zw, r3.w, c2.z, c2.xyyw
    mad r3.xyz, r3, c2.x, c2.y
    mad r0.yz, r2.z, r0, r2.xxyw
    mad r2.xy, r2.w, -r0.yzzw, r0.yzzw
    mov r2.z, c3.x
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
    max r3.w, r1.w, c3.x
    pow r1.w, r3.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r2.y, r4
    dp3 r1.w, -r1, r0.yzww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.yzww, -r1.w, -r1
    dp3 r3.w, r1, r6
    max r4.w, r3.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    max r3.w, r1.w, c3.x
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
    mad_pp r2.xyz, r7.y, c1, r2
    add_pp r1.w, -r7.w, -c2.y
    mad_pp r3.xyz, r7.w, c0, r1.w
    mul_pp r1.w, r6.w, r7.z
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mad_pp r3.xyz, r4, c2.x, r3
    mul_pp r4.w, r3.w, c0.w
    add_pp r3.xyz, r3, c2.y
    mad_pp r1.xyz, r3, r2, r1
    dp3 r5.w, r5, r0.yzww
    mul r0.xyz, r0.yzww, r5.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r6, r0
    mad_pp r4.xyz, r0, r1.w, r1
    mov_pp oC0, r4

// approximately 144 instruction slots used (8 texture, 136 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    mul_pp r0.xyz, r2, r0
    add_pp r1.xyz, r0, r0
    mov_pp oC0, r1

// approximately 74 instruction slots used (5 texture, 69 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    mul_pp r1.xyz, r2, r0
    mov_pp oC0, r1

// approximately 73 instruction slots used (5 texture, 68 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    mad_pp r0.xyz, r2, c2.x, r0
    add_pp r1.xyz, r0, c2.y
    mov_pp oC0, r1

// approximately 74 instruction slots used (5 texture, 69 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    texld_pp r5, t1, s0
    texld_pp r6, t2, s1
    mul_pp r2.xyw, r5.xyzz, r6.xyzz
    mul_pp r5.w, r5.w, c0.w
    mul_pp r1.xyz, r1, r2.xyww
    add_pp r1.xyz, r1, r1
    nrm r6.xyz, r3
    dp3 r1.w, r0.yzww, r6
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mad_pp r5.xyz, r0, r0.w, r1
    mov_pp oC0, r5

// approximately 74 instruction slots used (5 texture, 69 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 73 instruction slots used (5 texture, 68 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c2.x, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c2.y
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 74 instruction slots used (5 texture, 69 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyComplexFogDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, true, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c5, 2, -1, 1, 0
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
    mov r0.xz, c5
    mad r1.w, r0.w, -c12.w, r0.z
    texld r2, t6, s5
    mad r0.yw, r2.xxzy, c5.x, c5.y
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
    mov r1.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r2.w, r3, r2
    mul r4.xyz, r2, r2.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r1, r1, s2
    texld_pp r4, r4, s3
    mad_pp r0.yzw, r1.y, c4.xxyz, r0
    add_pp r2.w, -r1.w, c5.z
    mad_pp r1.xyw, r1.w, c0.xyzz, r2.w
    mul_pp r0.yzw, r0, r1.xxyw
    nrm r5.xyz, r3
    dp3 r4.w, r2, r5
    mov r2, c10
    mad_pp r2, r4.w, r2, c11
    mul_pp r1.xyw, r2.xyzz, r4.xyzz
    mul_pp r1.z, r1.z, r2.w
    mul_pp r1.xyz, r1.z, r1.xyww
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.yzw, r3.xxyz, r0, r1.xxyz
    mul_pp r1.w, r3.w, c0.w
    mul_pp r0.yzw, r2.xxyz, r0
    mul_pp r0.yzw, r0, c1.w
    add r2.xyz, r0.yzww, r0.yzww
    mad r0.xyz, r0.yzww, -r0.x, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 80 instruction slots used (5 texture, 75 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyComplexFogDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, true, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c5, 2, -1, 1, 0
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
    mov r1.w, c5.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c5.x, c5.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r1.w, -r2.w, c5.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    mul_pp r0.xyz, r2, r0
    mul r2.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 79 instruction slots used (5 texture, 74 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddComplexFogDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, true, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c5, 2, -1, 1, 0
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
    mov r1.w, c5.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c5.x, c5.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r1.w, -r2.w, c5.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    mad_pp r0.xyz, r2, c5.x, r0
    add_pp r0.xyz, r0, c5.y
    mul r2.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 80 instruction slots used (5 texture, 75 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyComplexFogDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, true, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c5, 2, -1, 1, 0
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
    mov r1.w, c5.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c5.x, c5.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r1.w, -r2.w, c5.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    texld_pp r5, t1, s0
    texld_pp r6, t2, s1
    mul_pp r2.xyw, r5.xyzz, r6.xyzz
    mul_pp r5.w, r5.w, c0.w
    mul_pp r1.xyz, r1, r2.xyww
    add_pp r1.xyz, r1, r1
    nrm r6.xyz, r3
    dp3 r1.w, r0.yzww, r6
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r5.xyz, r0, r1
    mov_pp oC0, r5

// approximately 80 instruction slots used (5 texture, 75 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyComplexFogDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, true, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c5, 2, -1, 1, 0
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
    mov r1.w, c5.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c5.x, c5.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r1.w, -r2.w, c5.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r0.xyz, r2, r1, r0
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 79 instruction slots used (5 texture, 74 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddComplexFogDetailNorm
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, true, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c5, 2, -1, 1, 0
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
    mov r1.w, c5.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c5.x, c5.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r1.w, -r2.w, c5.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c5.x, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c5.y
    mad_pp r0.xyz, r2, r1, r0
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 80 instruction slots used (5 texture, 75 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyDetailNormSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    mul_pp r0.xyz, r2, r0
    add_pp r1.xyz, r0, r0
    mov_pp oC0, r1

// approximately 74 instruction slots used (5 texture, 69 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyDetailNormSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    mul_pp r1.xyz, r2, r0
    mov_pp oC0, r1

// approximately 73 instruction slots used (5 texture, 68 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddDetailNormSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t2, s1
    texld_pp r3, t1, s0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    mad_pp r0.xyz, r2, c2.x, r0
    add_pp r1.xyz, r0, c2.y
    mov_pp oC0, r1

// approximately 74 instruction slots used (5 texture, 69 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyDetailNormSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    texld_pp r5, t1, s0
    texld_pp r6, t2, s1
    mul_pp r2.xyw, r5.xyzz, r6.xyzz
    mul_pp r5.w, r5.w, c0.w
    mul_pp r1.xyz, r1, r2.xyww
    add_pp r1.xyz, r1, r1
    nrm r6.xyz, r3
    dp3 r1.w, r0.yzww, r6
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mad_pp r5.xyz, r0, r0.w, r1
    mov_pp oC0, r5

// approximately 74 instruction slots used (5 texture, 69 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyDetailNormSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 73 instruction slots used (5 texture, 68 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddDetailNormSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, false, true, false, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    add r3.xyz, -t0, c8
    dp3 r1.w, r3, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r3
    texld_pp r2, r2, s2
    texld_pp r4, r4, s3
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    nrm r5.xyz, r3
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    mul_pp r0.xyz, r0, r4
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c2.x, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c2.y
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 74 instruction slots used (5 texture, 69 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mul_pp r0.xyz, r0, r2
    add_pp r1.xyz, r0, r0
    mov_pp oC0, r1

// approximately 84 instruction slots used (6 texture, 78 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mul_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 83 instruction slots used (6 texture, 77 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mad_pp r0.xyz, r2, c2.x, r0
    add_pp r1.xyz, r0, c2.y
    mov_pp oC0, r1

// approximately 84 instruction slots used (6 texture, 78 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mul_pp r2.xyw, r3.xyzz, r4.xyzz
    mul_pp r3.w, r3.w, c0.w
    mul_pp r1.xyz, r1, r2.xyww
    add_pp r1.xyz, r1, r1
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mad_pp r3.xyz, r0, r0.w, r1
    mov_pp oC0, r3

// approximately 84 instruction slots used (6 texture, 78 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2.xyww
    nrm r4.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r4, t1, s7
    texld_pp r3, r3, s3
    log r5.x, r4.x
    log r5.y, r4.y
    log r5.z, r4.z
    mul r2.xyw, r5.xyzz, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.w
    mul_sat_pp r2.xyw, r4.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r3
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 83 instruction slots used (6 texture, 77 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2.xyww
    nrm r4.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r4, t1, s7
    texld_pp r3, r3, s3
    log r5.x, r4.x
    log r5.y, r4.y
    log r5.z, r4.z
    mul r2.xyw, r5.xyzz, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.w
    mul_sat_pp r2.xyw, r4.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r3
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c2.x, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c2.y
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 84 instruction slots used (6 texture, 78 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyComplexFogDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, true, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c7, 2, -1, 1, 0
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
    mov r0.xz, c7
    mad r1.w, r0.w, -c12.w, r0.z
    texld r2, t6, s5
    mad r0.yw, r2.xxzy, c7.x, c7.y
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
    mov r1.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r1, r1, s2
    mad_pp r0.yzw, r1.y, c4.xxyz, r0
    add_pp r2.w, -r1.w, c7.z
    mad_pp r1.xyw, r1.w, c0.xyzz, r2.w
    mul_pp r0.yzw, r0, r1.xxyw
    add r1.xyw, -t0.xyzz, c8.xyzz
    dp3 r2.w, r1.xyww, r2
    mul r4.xyz, r2, r2.w
    mad_pp r4.xyz, r4, c8.w, -r1.xyww
    nrm r5.xyz, r1.xyww
    dp3 r4.w, r2, r5
    mov r2, c10
    mad_pp r2, r4.w, r2, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r1.xyw, r6.xyzz, c5.z
    exp r5.x, r1.x
    exp r5.y, r1.y
    exp r5.z, r1.w
    mul_sat_pp r1.xyw, r5.xyzz, c5.w
    mul_pp r1.xyw, r1, r2.xyzz
    mul_pp r4.w, r1.z, r2.w
    mul_pp r1.xyz, r1.xyww, r4
    mul_pp r1.xyz, r4.w, r1
    mad_pp r0.yzw, r3.xxyz, r0, r1.xxyz
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mul_pp r0.yzw, r0, r2.xxyz
    mul_pp r0.yzw, r0, c1.w
    add r2.xyz, r0.yzww, r0.yzww
    mad r0.xyz, r0.yzww, -r0.x, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 90 instruction slots used (6 texture, 84 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyComplexFogDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, true, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c7, 2, -1, 1, 0
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
    mov r1.w, c7.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c7.x, c7.y
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
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r1.w, -r2.w, c7.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mul_pp r0.xyz, r0, r2
    mul r2.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 89 instruction slots used (6 texture, 83 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddComplexFogDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, true, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c7, 2, -1, 1, 0
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
    mov r1.w, c7.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c7.x, c7.y
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
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r1.w, -r2.w, c7.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mad_pp r0.xyz, r2, c7.x, r0
    add_pp r0.xyz, r0, c7.y
    mul r2.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 90 instruction slots used (6 texture, 84 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyComplexFogDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, true, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c7, 2, -1, 1, 0
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
    mov r1.w, c7.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c7.x, c7.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r1.w, -r2.w, c7.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mul_pp r2.xyw, r3.xyzz, r4.xyzz
    mul_pp r3.w, r3.w, c0.w
    mul_pp r1.xyz, r1, r2.xyww
    add_pp r1.xyz, r1, r1
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 90 instruction slots used (6 texture, 84 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyComplexFogDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, true, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c7, 2, -1, 1, 0
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
    mov r1.w, c7.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c7.x, c7.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r1.w, -r2.w, c7.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2.xyww
    nrm r4.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r4, t1, s7
    texld_pp r3, r3, s3
    log r5.x, r4.x
    log r5.y, r4.y
    log r5.z, r4.z
    mul r2.xyw, r5.xyzz, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.w
    mul_sat_pp r2.xyw, r4.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r3
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r0.xyz, r2, r1, r0
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 89 instruction slots used (6 texture, 83 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddComplexFogDetailNormSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, true, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c7, 2, -1, 1, 0
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
    mov r1.w, c7.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c7.x, c7.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c4, r1
    add_pp r1.w, -r2.w, c7.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2.xyww
    nrm r4.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r4, t1, s7
    texld_pp r3, r3, s3
    log r5.x, r4.x
    log r5.y, r4.y
    log r5.z, r4.z
    mul r2.xyw, r5.xyzz, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.w
    mul_sat_pp r2.xyw, r4.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r3
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c7.x, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c7.y
    mad_pp r0.xyz, r2, r1, r0
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 90 instruction slots used (6 texture, 84 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyDetailNormSpecMapSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mul_pp r0.xyz, r0, r2
    add_pp r1.xyz, r0, r0
    mov_pp oC0, r1

// approximately 84 instruction slots used (6 texture, 78 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyDetailNormSpecMapSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mul_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 83 instruction slots used (6 texture, 77 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddDetailNormSpecMapSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r3, t1, s0
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mul_pp r0.xyz, r0.w, r0
    mad_pp r0.xyz, r3, r1, r0
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mad_pp r0.xyz, r2, c2.x, r0
    add_pp r1.xyz, r0, c2.y
    mov_pp oC0, r1

// approximately 84 instruction slots used (6 texture, 78 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyDetailNormSpecMapSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mul_pp r2.xyw, r3.xyzz, r4.xyzz
    mul_pp r3.w, r3.w, c0.w
    mul_pp r1.xyz, r1, r2.xyww
    add_pp r1.xyz, r1, r1
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r4.xyz, r0.yzww, r1.w
    mad_pp r4.xyz, r4, c8.w, -r2.xyww
    nrm r5.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r5
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r5, t1, s7
    texld_pp r4, r4, s3
    log r6.x, r5.x
    log r6.y, r5.y
    log r6.z, r5.z
    mul r2.xyw, r6.xyzz, c5.z
    exp r5.x, r2.x
    exp r5.y, r2.y
    exp r5.z, r2.w
    mul_sat_pp r2.xyw, r5.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r4
    mad_pp r3.xyz, r0, r0.w, r1
    mov_pp oC0, r3

// approximately 84 instruction slots used (6 texture, 78 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyDetailNormSpecMapSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2.xyww
    nrm r4.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r4, t1, s7
    texld_pp r3, r3, s3
    log r5.x, r4.x
    log r5.y, r4.y
    log r5.z, r4.z
    mul r2.xyw, r5.xyzz, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.w
    mul_sat_pp r2.xyw, r4.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r3
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 83 instruction slots used (6 texture, 77 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddDetailNormSpecMapSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, false, true, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r1.w, c2.z
    mad r0.x, r0.w, -c12.w, r1.w
    texld r2, t6, s5
    mad r0.yz, r2.xxyw, c2.x, c2.y
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
    mov r2.xy, t1.zwzw
    texld_pp r2, r2, s2
    mad_pp r1.xyz, r2.y, c1, r1
    add_pp r1.w, -r2.w, c2.z
    mad_pp r2.xyw, r2.w, c0.xyzz, r1.w
    mul_pp r1.xyz, r1, r2.xyww
    add r2.xyw, -t0.xyzz, c8.xyzz
    dp3 r1.w, r2.xyww, r0.yzww
    mul r3.xyz, r0.yzww, r1.w
    mad_pp r3.xyz, r3, c8.w, -r2.xyww
    nrm r4.xyz, r2.xyww
    dp3 r1.w, r0.yzww, r4
    mov r0, c10
    mad_pp r0, r1.w, r0, c11
    texld_pp r4, t1, s7
    texld_pp r3, r3, s3
    log r5.x, r4.x
    log r5.y, r4.y
    log r5.z, r4.z
    mul r2.xyw, r5.xyzz, c5.z
    exp r4.x, r2.x
    exp r4.y, r2.y
    exp r4.z, r2.w
    mul_sat_pp r2.xyw, r4.xyzz, c5.w
    mul_pp r0.xyz, r0, r2.xyww
    mul_pp r0.w, r0.w, r2.z
    mul_pp r0.xyz, r0, r3
    mul_pp r0.xyz, r0.w, r0
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c2.x, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c2.y
    mad_pp r3.xyz, r2, r1, r0
    mov_pp oC0, r3

// approximately 84 instruction slots used (6 texture, 78 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    mul_pp r2.w, r8.w, c0.w
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mul_pp r0.xyz, r3, r0
    add_pp r2.xyz, r0, r0
    mov_pp oC0, r2

// approximately 119 instruction slots used (5 texture, 114 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    mul_pp r2.w, r8.w, c0.w
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mul_pp r2.xyz, r3, r0
    mov_pp oC0, r2

// approximately 118 instruction slots used (5 texture, 113 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    mul_pp r2.w, r8.w, c0.w
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mad_pp r0.xyz, r3, c2.x, r0
    add_pp r2.xyz, r0, c2.y
    mov_pp oC0, r2

// approximately 119 instruction slots used (5 texture, 114 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mov r3.xy, t1.zwzw
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, r3, s2
    texld_pp r0, r0, s3
    mad_pp r2.xyz, r3.y, c1, r2
    add_pp r0.w, -r3.w, c2.z
    mad_pp r3.xyw, r3.w, c0.xyzz, r0.w
    mul_pp r2.xyz, r2, r3.xyww
    texld_pp r5, t1, s0
    texld_pp r6, t2, s1
    mul_pp r3.xyw, r5.xyzz, r6.xyzz
    mul_pp r5.w, r5.w, c0.w
    mul_pp r2.xyz, r2, r3.xyww
    add_pp r2.xyz, r2, r2
    mad_pp r1.xyz, r1, r3.z, r2
    mul_pp r0.w, r4.w, r3.z
    mul_pp r0.xyz, r4, r0
    mad_pp r5.xyz, r0, r0.w, r1
    mov_pp oC0, r5

// approximately 119 instruction slots used (5 texture, 114 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r5.y, c1, r2
    add_pp r0.w, -r5.w, c2.z
    mad_pp r3.xyz, r5.w, c0, r0.w
    mul_pp r0.w, r4.w, r5.z
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r5, t2, s1
    mul_pp r3.xyz, r3, r5
    mul_pp r5.w, r3.w, c0.w
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r0.xyz, r4, r0
    mad_pp r5.xyz, r0, r0.w, r1
    mov_pp oC0, r5

// approximately 118 instruction slots used (5 texture, 113 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    texld_pp r6, t1, s0
    texld_pp r8, t2, s1
    mad_pp r6.xyz, r8, c2.x, r6
    mul_pp r8.w, r6.w, c0.w
    add_pp r6.xyz, r6, c2.y
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r5.y, c1, r2
    add_pp r0.w, -r5.w, c2.z
    mad_pp r3.xyz, r5.w, c0, r0.w
    mul_pp r0.w, r4.w, r5.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r6, r2, r1
    mul_pp r0.xyz, r4, r0
    mad_pp r8.xyz, r0, r0.w, r1
    mov_pp oC0, r8

// approximately 119 instruction slots used (5 texture, 114 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyComplexFogDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, true, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c5, 2, -1, 1, 0
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
    mov r0.xz, c5
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.yw, r3.xxzy, c5.x, c5.y
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
    max r2.w, r1.w, c5.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r0.yzw, r1.w, c17.xxyz
    mul r0.yzw, r4.y, r0
    dp3 r1.w, -r1, r3
    add r1.w, r1.w, r1.w
    mad r1.xyz, r3, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r3.w, r2.w, c5.w
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r0.yzw, r1.xxyz, r4.x, r0
    dp3 r2.w, c18, r3
    add r2.w, r2.w, r2.w
    mad r1.xyz, r3, -r2.w, c18
    dp3 r2.w, r1, r6
    max r3.w, r2.w, c5.w
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
    max r3.w, r2.w, c5.w
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
    mad_pp r2.xyz, r6.y, c4, r2
    add_pp r2.w, -r6.w, c5.z
    mad_pp r4.xyz, r6.w, c0, r2.w
    mul_pp r1.w, r1.w, r6.z
    mul_pp r2.xyz, r2, r4
    mad_pp r0.yzw, r8.xxyz, r2.xxyz, r0
    mul_pp r2.w, r8.w, c0.w
    dp3 r3.w, r5, r3
    mul r3.xyz, r3, r3.w
    mad_pp r3.xyz, r3, c8.w, -r5
    texld_pp r4, t2, s1
    texld_pp r3, r3, s3
    mul_pp r1.xyz, r1, r3
    mad_pp r0.yzw, r1.xxyz, r1.w, r0
    mul_pp r0.yzw, r4.xxyz, r0
    mul_pp r0.yzw, r0, c1.w
    add r1.xyz, r0.yzww, r0.yzww
    mad r0.xyz, r0.yzww, -r0.x, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 125 instruction slots used (5 texture, 120 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyComplexFogDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, true, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c5, 2, -1, 1, 0
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
    mov r0.z, c5.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c5.x, c5.y
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
    max r2.w, r1.w, c5.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c5.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c5.w
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
    max r2.w, r1.w, c5.w
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
    mad_pp r2.xyz, r6.y, c4, r2
    add_pp r1.w, -r6.w, c5.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    mul_pp r2.w, r8.w, c0.w
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mul_pp r0.xyz, r3, r0
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 124 instruction slots used (5 texture, 119 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddComplexFogDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, true, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c5, 2, -1, 1, 0
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
    mov r0.z, c5.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c5.x, c5.y
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
    max r2.w, r1.w, c5.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c5.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c5.w
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
    max r2.w, r1.w, c5.w
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
    mad_pp r2.xyz, r6.y, c4, r2
    add_pp r1.w, -r6.w, c5.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    mul_pp r2.w, r8.w, c0.w
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mad_pp r0.xyz, r3, c5.x, r0
    add_pp r0.xyz, r0, c5.y
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 125 instruction slots used (5 texture, 120 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyComplexFogDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, true, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c5, 2, -1, 1, 0
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
    mov r0.z, c5.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c5.x, c5.y
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
    max r2.w, r1.w, c5.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c5.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c5.w
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
    max r2.w, r1.w, c5.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mov r3.xy, t1.zwzw
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, r3, s2
    texld_pp r0, r0, s3
    mad_pp r2.xyz, r3.y, c4, r2
    add_pp r0.w, -r3.w, c5.z
    mad_pp r3.xyw, r3.w, c0.xyzz, r0.w
    mul_pp r2.xyz, r2, r3.xyww
    texld_pp r5, t1, s0
    texld_pp r6, t2, s1
    mul_pp r3.xyw, r5.xyzz, r6.xyzz
    mul_pp r5.w, r5.w, c0.w
    mul_pp r2.xyz, r2, r3.xyww
    add_pp r2.xyz, r2, r2
    mad_pp r1.xyz, r1, r3.z, r2
    mul_pp r0.w, r4.w, r3.z
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r5.xyz, r0, r1
    mov_pp oC0, r5

// approximately 125 instruction slots used (5 texture, 120 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyComplexFogDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, true, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c5, 2, -1, 1, 0
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
    mov r0.z, c5.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c5.x, c5.y
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
    max r2.w, r1.w, c5.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c5.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c5.w
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
    max r2.w, r1.w, c5.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r5.y, c4, r2
    add_pp r0.w, -r5.w, c5.z
    mad_pp r3.xyz, r5.w, c0, r0.w
    mul_pp r0.w, r4.w, r5.z
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r5, t2, s1
    mul_pp r3.xyz, r3, r5
    mul_pp r5.w, r3.w, c0.w
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r5.xyz, r0, r1
    mov_pp oC0, r5

// approximately 124 instruction slots used (5 texture, 119 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddComplexFogDetailNormSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, true, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c5, 2, -1, 1, 0
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
    mov r0.z, c5.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c5.x, c5.y
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
    max r2.w, r1.w, c5.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c5.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c5.w
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
    max r2.w, r1.w, c5.w
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
    texld_pp r6, t1, s0
    texld_pp r8, t2, s1
    mad_pp r6.xyz, r8, c5.x, r6
    mul_pp r8.w, r6.w, c0.w
    add_pp r6.xyz, r6, c5.y
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r5.y, c4, r2
    add_pp r0.w, -r5.w, c5.z
    mad_pp r3.xyz, r5.w, c0, r0.w
    mul_pp r0.w, r4.w, r5.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r6, r2, r1
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r0.w, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r8.xyz, r0, r1
    mov_pp oC0, r8

// approximately 125 instruction slots used (5 texture, 120 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyDetailNormSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    mul_pp r2.w, r8.w, c0.w
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mul_pp r0.xyz, r3, r0
    add_pp r2.xyz, r0, r0
    mov_pp oC0, r2

// approximately 119 instruction slots used (5 texture, 114 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyDetailNormSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    mul_pp r2.w, r8.w, c0.w
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mul_pp r2.xyz, r3, r0
    mov_pp oC0, r2

// approximately 118 instruction slots used (5 texture, 113 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddDetailNormSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    mul_pp r2.w, r8.w, c0.w
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t2, s1
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mad_pp r0.xyz, r3, c2.x, r0
    add_pp r2.xyz, r0, c2.y
    mov_pp oC0, r2

// approximately 119 instruction slots used (5 texture, 114 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyDetailNormSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r7.y, r1
    mul_sat_pp r1.xyz, r4, r1
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mov r3.xy, t1.zwzw
    dp3 r1.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r1.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, r3, s2
    texld_pp r0, r0, s3
    mad_pp r2.xyz, r3.y, c1, r2
    add_pp r0.w, -r3.w, c2.z
    mad_pp r3.xyw, r3.w, c0.xyzz, r0.w
    mul_pp r2.xyz, r2, r3.xyww
    texld_pp r5, t1, s0
    texld_pp r6, t2, s1
    mul_pp r3.xyw, r5.xyzz, r6.xyzz
    mul_pp r5.w, r5.w, c0.w
    mul_pp r2.xyz, r2, r3.xyww
    add_pp r2.xyz, r2, r2
    mad_pp r1.xyz, r1, r3.z, r2
    mul_pp r0.w, r4.w, r3.z
    mul_pp r0.xyz, r4, r0
    mad_pp r5.xyz, r0, r0.w, r1
    mov_pp oC0, r5

// approximately 119 instruction slots used (5 texture, 114 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyDetailNormSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r5.y, c1, r2
    add_pp r0.w, -r5.w, c2.z
    mad_pp r3.xyz, r5.w, c0, r0.w
    mul_pp r0.w, r4.w, r5.z
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r5, t2, s1
    mul_pp r3.xyz, r3, r5
    mul_pp r5.w, r3.w, c0.w
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r0.xyz, r4, r0
    mad_pp r5.xyz, r0, r0.w, r1
    mov_pp oC0, r5

// approximately 118 instruction slots used (5 texture, 113 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddDetailNormSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, false, true, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    texld_pp r6, t1, s0
    texld_pp r8, t2, s1
    mad_pp r6.xyz, r8, c2.x, r6
    mul_pp r8.w, r6.w, c0.w
    add_pp r6.xyz, r6, c2.y
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r5.y, c1, r2
    add_pp r0.w, -r5.w, c2.z
    mad_pp r3.xyz, r5.w, c0, r0.w
    mul_pp r0.w, r4.w, r5.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r6, r2, r1
    mul_pp r0.xyz, r4, r0
    mad_pp r8.xyz, r0, r0.w, r1
    mov_pp oC0, r8

// approximately 119 instruction slots used (5 texture, 114 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    dp3 r2.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r2.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r2.w, r3.w, c0.w
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    add_pp r2.xyz, r0, r0
    mov_pp oC0, r2

// approximately 129 instruction slots used (6 texture, 123 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    dp3 r2.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r2.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r2.w, r3.w, c0.w
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    texld_pp r1, t2, s1
    mul_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 128 instruction slots used (6 texture, 122 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    dp3 r2.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r2.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r2.w, r3.w, c0.w
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c2.x, r0
    add_pp r2.xyz, r0, c2.y
    mov_pp oC0, r2

// approximately 129 instruction slots used (6 texture, 123 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r7, t2, s1
    mul_pp r3.xyz, r3, r7
    mul_pp r7.w, r3.w, c0.w
    mul_pp r2.xyz, r2, r3
    add_pp r2.xyz, r2, r2
    mad_pp r1.xyz, r1, r6.z, r2
    mul_pp r1.w, r4.w, r6.z
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r7.xyz, r0, r1.w, r1
    mov_pp oC0, r7

// approximately 129 instruction slots used (6 texture, 123 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r6, t2, s1
    mul_pp r3.xyz, r3, r6
    mul_pp r6.w, r3.w, c0.w
    mad_pp r1.xyz, r3, r2, r1
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r6.xyz, r0, r1.w, r1
    mov_pp oC0, r6

// approximately 128 instruction slots used (6 texture, 122 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    texld_pp r8, t1, s0
    texld_pp r9, t2, s1
    mad_pp r8.xyz, r9, c2.x, r8
    mul_pp r9.w, r8.w, c0.w
    add_pp r8.xyz, r8, c2.y
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r9.xyz, r0, r1.w, r1
    mov_pp oC0, r9

// approximately 129 instruction slots used (6 texture, 123 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyComplexFogDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, true, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c23, 2, -1, 1, 0
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
    mov r0.xz, c23
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.yw, r3.xxzy, c23.x, c23.y
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
    max r2.w, r1.w, c23.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r0.yzw, r1.w, c17.xxyz
    mul r0.yzw, r4.y, r0
    dp3 r1.w, -r1, r3
    add r1.w, r1.w, r1.w
    mad r1.xyz, r3, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r3.w, r2.w, c23.w
    pow r2.w, r3.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r0.yzw, r1.xxyz, r4.x, r0
    dp3 r2.w, c18, r3
    add r2.w, r2.w, r2.w
    mad r1.xyz, r3, -r2.w, c18
    dp3 r2.w, r1, r6
    max r3.w, r2.w, c23.w
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
    max r3.w, r2.w, c23.w
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
    mad_pp r2.xyz, r4.x, r2, c22
    mad_pp r2.xyz, r4.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c4, r2
    add_pp r2.w, -r6.w, c23.z
    mad_pp r4.xyz, r6.w, c0, r2.w
    mul_pp r1.w, r1.w, r6.z
    mul_pp r2.xyz, r2, r4
    dp3 r2.w, r5, r3
    mul r3.xyz, r3, r2.w
    mad_pp r3.xyz, r3, c8.w, -r5
    texld_pp r4, t1, s0
    texld_pp r3, r3, s3
    mad_pp r0.yzw, r4.xxyz, r2.xxyz, r0
    mul_pp r2.w, r4.w, c0.w
    mul_pp r1.xyz, r1, r3
    mad_pp r0.yzw, r1.xxyz, r1.w, r0
    texld_pp r1, t2, s1
    mul_pp r0.yzw, r0, r1.xxyz
    mul_pp r0.yzw, r0, c1.w
    add r1.xyz, r0.yzww, r0.yzww
    mad r0.xyz, r0.yzww, -r0.x, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 135 instruction slots used (6 texture, 129 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyComplexFogDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, true, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c23, 2, -1, 1, 0
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
    mov r0.z, c23.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c23.x, c23.y
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
    max r2.w, r1.w, c23.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c23.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c23.w
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
    max r2.w, r1.w, c23.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c4, r2
    add_pp r1.w, -r6.w, c23.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    dp3 r2.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r2.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r2.w, r3.w, c0.w
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 134 instruction slots used (6 texture, 128 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddComplexFogDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, true, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c23, 2, -1, 1, 0
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
    mov r0.z, c23.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c23.x, c23.y
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
    max r2.w, r1.w, c23.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c23.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c23.w
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
    max r2.w, r1.w, c23.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c4, r2
    add_pp r1.w, -r6.w, c23.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    dp3 r2.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r2.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r2.w, r3.w, c0.w
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c23.x, r0
    add_pp r0.xyz, r0, c23.y
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 135 instruction slots used (6 texture, 129 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyComplexFogDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, true, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c23, 2, -1, 1, 0
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
    mov r0.z, c23.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c23.x, c23.y
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
    max r2.w, r1.w, c23.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c23.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c23.w
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
    max r2.w, r1.w, c23.w
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
    mad_pp r2.xyz, r6.y, c4, r2
    add_pp r1.w, -r6.w, c23.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r7, t2, s1
    mul_pp r3.xyz, r3, r7
    mul_pp r7.w, r3.w, c0.w
    mul_pp r2.xyz, r2, r3
    add_pp r2.xyz, r2, r2
    mad_pp r1.xyz, r1, r6.z, r2
    mul_pp r1.w, r4.w, r6.z
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r7.xyz, r0, r1
    mov_pp oC0, r7

// approximately 135 instruction slots used (6 texture, 129 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyComplexFogDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, true, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c23, 2, -1, 1, 0
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
    mov r0.z, c23.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c23.x, c23.y
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
    max r2.w, r1.w, c23.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c23.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c23.w
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
    max r2.w, r1.w, c23.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c4, r2
    add_pp r1.w, -r6.w, c23.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r6, t2, s1
    mul_pp r3.xyz, r3, r6
    mul_pp r6.w, r3.w, c0.w
    mad_pp r1.xyz, r3, r2, r1
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r6.xyz, r0, r1
    mov_pp oC0, r6

// approximately 134 instruction slots used (6 texture, 128 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddComplexFogDetailNormSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, true, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c23, 2, -1, 1, 0
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
    mov r0.z, c23.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c23.x, c23.y
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
    max r2.w, r1.w, c23.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c23.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c23.w
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
    max r2.w, r1.w, c23.w
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
    texld_pp r8, t1, s0
    texld_pp r9, t2, s1
    mad_pp r8.xyz, r9, c23.x, r8
    mul_pp r9.w, r8.w, c0.w
    add_pp r8.xyz, r8, c23.y
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c4, r2
    add_pp r1.w, -r6.w, c23.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r9.xyz, r0, r1
    mov_pp oC0, r9

// approximately 135 instruction slots used (6 texture, 129 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyDetailNormSpecMapSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    dp3 r2.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r2.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r2.w, r3.w, c0.w
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    texld_pp r1, t2, s1
    mul_pp r0.xyz, r0, r1
    add_pp r2.xyz, r0, r0
    mov_pp oC0, r2

// approximately 129 instruction slots used (6 texture, 123 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyDetailNormSpecMapSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    dp3 r2.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r2.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r2.w, r3.w, c0.w
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    texld_pp r1, t2, s1
    mul_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 128 instruction slots used (6 texture, 122 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddDetailNormSpecMapSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    dp3 r2.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r2.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r3, t1, s0
    texld_pp r0, r0, s3
    mad_pp r1.xyz, r3, r2, r1
    mul_pp r2.w, r3.w, c0.w
    mul_pp r0.xyz, r4, r0
    mad_pp r0.xyz, r0, r1.w, r1
    texld_pp r1, t2, s1
    mad_pp r0.xyz, r1, c2.x, r0
    add_pp r2.xyz, r0, c2.y
    mov_pp oC0, r2

// approximately 129 instruction slots used (6 texture, 123 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyDetailNormSpecMapSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r7, t2, s1
    mul_pp r3.xyz, r3, r7
    mul_pp r7.w, r3.w, c0.w
    mul_pp r2.xyz, r2, r3
    add_pp r2.xyz, r2, r2
    mad_pp r1.xyz, r1, r6.z, r2
    mul_pp r1.w, r4.w, r6.z
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r7.xyz, r0, r1.w, r1
    mov_pp oC0, r7

// approximately 129 instruction slots used (6 texture, 123 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyDetailNormSpecMapSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    texld_pp r3, t1, s0
    texld_pp r6, t2, s1
    mul_pp r3.xyz, r3, r6
    mul_pp r6.w, r3.w, c0.w
    mad_pp r1.xyz, r3, r2, r1
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r6.xyz, r0, r1.w, r1
    mov_pp oC0, r6

// approximately 128 instruction slots used (6 texture, 122 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddDetailNormSpecMapSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, false, true, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 2, -1, 1, 0
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
    mov r0.z, c2.z
    mad r2.w, r2.w, -c12.w, r0.z
    texld r3, t6, s5
    mad r0.xy, r3, c2.x, c2.y
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
    max r2.w, r1.w, c2.w
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r4.xyz, r1.w, c17
    mul r4.xyz, r3.y, r4
    dp3 r1.w, -r1, r0.xyww
    add r1.w, r1.w, r1.w
    mad r1.xyz, r0.xyww, -r1.w, -r1
    dp3 r2.w, r1, r6
    max r4.w, r2.w, c2.w
    pow r2.w, r4.w, c7.z
    mul r2.w, r2.w, c7.w
    mul r1.xyz, r2.w, c14
    mad_pp r1.xyz, r1, r3.x, r4
    dp3 r1.w, c18, r0.xyww
    add r1.w, r1.w, r1.w
    mad r4.xyz, r0.xyww, -r1.w, c18
    dp3 r1.w, r4, r6
    max r2.w, r1.w, c2.w
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
    max r2.w, r1.w, c2.w
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
    texld_pp r8, t1, s0
    texld_pp r9, t2, s1
    mad_pp r8.xyz, r9, c2.x, r8
    mul_pp r9.w, r8.w, c0.w
    add_pp r8.xyz, r8, c2.y
    mad_pp r2.xyz, r3.x, r2, c22
    mad_pp r2.xyz, r3.y, c17, r2
    mad_pp r2.xyz, r7.x, c19, r2
    mad_pp r2.xyz, r7.y, c21, r2
    mad_pp r2.xyz, r6.y, c1, r2
    add_pp r1.w, -r6.w, c2.z
    mad_pp r3.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r4.w, r6.z
    mul_pp r2.xyz, r2, r3
    mad_pp r1.xyz, r8, r2, r1
    dp3 r4.w, r5, r0.xyww
    mul r0.xyz, r0.xyww, r4.w
    mad_pp r0.xyz, r0, c8.w, -r5
    texld_pp r0, r0, s3
    mul_pp r0.xyz, r4, r0
    mad_pp r9.xyz, r0, r1.w, r1
    mov_pp oC0, r9

// approximately 129 instruction slots used (6 texture, 123 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplySpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mad r1.w, r0.w, -r1.w, c2.x
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
    mad r2.w, r1.w, -r2.w, c2.x
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
    mov r1.xy, t1.zwzw
    texld_pp r2, t1, s7
    texld_pp r1, r1, s2
    mad_pp r0.xyz, r1.y, c1, r0
    add_pp r0.w, -r1.w, c2.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    log r3.x, r2.x
    log r3.y, r2.y
    log r3.z, r2.z
    mul r1.xyw, r3.xyzz, c5.z
    exp r2.x, r1.x
    exp r2.y, r1.y
    exp r2.z, r1.w
    mul_sat_pp r1.xyw, r2.xyzz, c5.w
    add r2.xyz, -t0, c8
    nrm r3.xyz, r2
    dp3 r0.w, t3, r3
    mov r3, c10
    mad_pp r3, r0.w, r3, c11
    mul_pp r1.xyw, r1, r3.xyzz
    mul_pp r0.w, r1.z, r3.w
    dp3 r2.w, r2, t3
    mul r3.xyz, r2.w, t3
    mad_pp r2.xyz, r3, c8.w, -r2
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mul_pp r1.xyz, r1.xyww, r2
    mul_pp r1.xyz, r0.w, r1
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mul_pp r0.xyz, r0, r2
    add_pp r1.xyz, r0, r0
    mov_pp oC0, r1

// approximately 78 instruction slots used (5 texture, 73 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplySpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mad r1.w, r0.w, -r1.w, c2.x
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
    mad r2.w, r1.w, -r2.w, c2.x
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
    mov r1.xy, t1.zwzw
	
    texld_pp r2, t1, s7
	
    texld_pp r1, r1, s2
    mad_pp r0.xyz, r1.y, c1, r0
    add_pp r0.w, -r1.w, c2.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
	
    log r3.x, r2.x
    log r3.y, r2.y
    log r3.z, r2.z
    mul r1.xyw, r3.xyzz, c5.z
    exp r2.x, r1.x
    exp r2.y, r1.y
    exp r2.z, r1.w
    mul_sat_pp r1.xyw, r2.xyzz, c5.w
	
    add r2.xyz, -t0, c8
    nrm r3.xyz, r2
    dp3 r0.w, t3, r3
	
    mov r3, c10
    mad_pp r3, r0.w, r3, c11	
    mul_pp r1.xyw, r1, r3.xyzz
	
    mul_pp r0.w, r1.z, r3.w
    dp3 r2.w, r2, t3
    mul r3.xyz, r2.w, t3
    mad_pp r2.xyz, r3, c8.w, -r2
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mul_pp r1.xyz, r1.xyww, r2
    mul_pp r1.xyz, r0.w, r1
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mul_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 77 instruction slots used (5 texture, 72 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 2, -1, 0
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
    mov_pp r1.w, c2.x
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
    mov r1.xy, t1.zwzw
    texld_pp r2, t1, s7
    texld_pp r1, r1, s2
    mad_pp r0.xyz, r1.y, c1, r0
    add_pp r0.w, -r1.w, c2.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    log r3.x, r2.x
    log r3.y, r2.y
    log r3.z, r2.z
    mul r1.xyw, r3.xyzz, c5.z
    exp r2.x, r1.x
    exp r2.y, r1.y
    exp r2.z, r1.w
    mul_sat_pp r1.xyw, r2.xyzz, c5.w
    add r2.xyz, -t0, c8
    nrm r3.xyz, r2
    dp3 r0.w, t3, r3
    mov r3, c10
    mad_pp r3, r0.w, r3, c11
    mul_pp r1.xyw, r1, r3.xyzz
    mul_pp r0.w, r1.z, r3.w
    dp3 r2.w, r2, t3
    mul r3.xyz, r2.w, t3
    mad_pp r2.xyz, r3, c8.w, -r2
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mul_pp r1.xyz, r1.xyww, r2
    mul_pp r1.xyz, r0.w, r1
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mad_pp r0.xyz, r2, c2.y, r0
    add_pp r1.xyz, r0, c2.z
    mov_pp oC0, r1

// approximately 77 instruction slots used (5 texture, 72 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplySpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mad r1.w, r0.w, -r1.w, c2.x
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
    mad r2.w, r1.w, -r2.w, c2.x
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
    mov r1.xy, t1.zwzw
    texld_pp r2, t1, s7
    texld_pp r1, r1, s2
    mad_pp r0.xyz, r1.y, c1, r0
    add_pp r0.w, -r1.w, c2.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mul_pp r1.xyw, r3.xyzz, r4.xyzz
    mul_pp r3.w, r3.w, c0.w
    mul_pp r0.xyz, r0, r1.xyww
    add_pp r0.xyz, r0, r0
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r1.xyw, r4.xyzz, c5.z
    exp r2.x, r1.x
    exp r2.y, r1.y
    exp r2.z, r1.w
    mul_sat_pp r1.xyw, r2.xyzz, c5.w
    add r2.xyz, -t0, c8
    nrm r4.xyz, r2
    dp3 r0.w, t3, r4
    mov r4, c10
    mad_pp r4, r0.w, r4, c11
    mul_pp r1.xyw, r1, r4.xyzz
    mul_pp r0.w, r1.z, r4.w
    dp3 r2.w, r2, t3
    mul r4.xyz, r2.w, t3
    mad_pp r2.xyz, r4, c8.w, -r2
    texld_pp r2, r2, s3
    mul_pp r1.xyz, r1.xyww, r2
    mad_pp r3.xyz, r1, r0.w, r0
    mov_pp oC0, r3

// approximately 78 instruction slots used (5 texture, 73 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplySpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mad r1.w, r0.w, -r1.w, c2.x
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
    mad r2.w, r1.w, -r2.w, c2.x
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
    mov r1.xy, t1.zwzw
    texld_pp r2, t1, s7
    texld_pp r1, r1, s2
    mad_pp r0.xyz, r1.y, c1, r0
    add_pp r0.w, -r1.w, c2.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    log r3.x, r2.x
    log r3.y, r2.y
    log r3.z, r2.z
    mul r1.xyw, r3.xyzz, c5.z
    exp r2.x, r1.x
    exp r2.y, r1.y
    exp r2.z, r1.w
    mul_sat_pp r1.xyw, r2.xyzz, c5.w
    add r2.xyz, -t0, c8
    nrm r3.xyz, r2
    dp3 r0.w, t3, r3
    mov r3, c10
    mad_pp r3, r0.w, r3, c11
    mul_pp r1.xyw, r1, r3.xyzz
    mul_pp r0.w, r1.z, r3.w
    dp3 r2.w, r2, t3
    mul r3.xyz, r2.w, t3
    mad_pp r2.xyz, r3, c8.w, -r2
    texld_pp r2, r2, s3
    mul_pp r1.xyz, r1.xyww, r2
    mul_pp r1.xyz, r0.w, r1
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r3.xyz, r2, r0, r1
    mov_pp oC0, r3

// approximately 77 instruction slots used (5 texture, 72 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 2, -1, 0
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
    mov_pp r1.w, c2.x
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
    mov r1.xy, t1.zwzw
    texld_pp r2, t1, s7
    texld_pp r1, r1, s2
    mad_pp r0.xyz, r1.y, c1, r0
    add_pp r0.w, -r1.w, c2.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    log r3.x, r2.x
    log r3.y, r2.y
    log r3.z, r2.z
    mul r1.xyw, r3.xyzz, c5.z
    exp r2.x, r1.x
    exp r2.y, r1.y
    exp r2.z, r1.w
    mul_sat_pp r1.xyw, r2.xyzz, c5.w
    add r2.xyz, -t0, c8
    nrm r3.xyz, r2
    dp3 r0.w, t3, r3
    mov r3, c10
    mad_pp r3, r0.w, r3, c11
    mul_pp r1.xyw, r1, r3.xyzz
    mul_pp r0.w, r1.z, r3.w
    dp3 r2.w, r2, t3
    mul r3.xyz, r2.w, t3
    mad_pp r2.xyz, r3, c8.w, -r2
    texld_pp r2, r2, s3
    mul_pp r1.xyz, r1.xyww, r2
    mul_pp r1.xyz, r0.w, r1
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c2.y, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c2.z
    mad_pp r3.xyz, r2, r0, r1
    mov_pp oC0, r3

// approximately 77 instruction slots used (5 texture, 72 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyComplexFogSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, true, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 1, 2, 0, 0
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
    mov r0.xy, c6
    mad r1.w, r0.w, -c12.w, r0.x
    dp3_sat r0.z, t3, r1
    dp3 r0.w, r1, -c13
    mov r2.w, c13.w
    mad_sat r0.w, r0.w, r2.w, c14.w
    mul r0.z, r0.z, r1.w
    mul r1.x, r0.w, r0.z
    mov r2.w, c15.w
    add r2.xyz, -t0, c15
    dp4 r2.w, r2, r2
    nrm r3.xyz, r2
    mad r3.w, r2.w, -c15.w, r0.x
    dp3_sat r0.x, t3, r3
    dp3 r0.z, r3, -c16
    mov r0.w, c16.w
    mad_sat r0.z, r0.z, r0.w, c17.w
    mul r0.x, r0.x, r3.w
    mul r1.y, r0.z, r0.x
    max r0.xz, r1.xyyw, t2.z
    min r1.xy, t2.w, r0.xzzw
    mov r2.xyz, c14
    mad_pp r0.xzw, r1.x, r2.xyyz, c22.xyyz
    mad_pp r0.xzw, r1.y, c17.xyyz, r0
    dp3 r1.x, -c18, t3
    mul r1.y, r1.x, c9.z
    max r2.x, r1.x, r1.y
    dp3 r2.y, -c20, t3
    max r1.xy, r2, t2.z
    min r2.xy, t2.w, r1
    mad_pp r0.xzw, r2.x, c19.xyyz, r0
    mad_pp r0.xzw, r2.y, c21.xyyz, r0
    mov r1.xy, t1.zwzw
    texld_pp r2, t1, s7
    texld_pp r1, r1, s2
    mad_pp r0.xzw, r1.y, c4.xyyz, r0
    add_pp r2.w, -r1.w, c6.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r2.w
    mul_pp r0.xzw, r0, r1.xyyw
    log r3.x, r2.x
    log r3.y, r2.y
    log r3.z, r2.z
    mul r1.xyw, r3.xyzz, c5.z
    exp r2.x, r1.x
    exp r2.y, r1.y
    exp r2.z, r1.w
    mul_sat_pp r1.xyw, r2.xyzz, c5.w
    add r2.xyz, -t0, c8
    nrm r3.xyz, r2
    dp3 r2.w, t3, r3
    mov r3, c10
    mad_pp r3, r2.w, r3, c11
    mul_pp r1.xyw, r1, r3.xyzz
    mul_pp r2.w, r1.z, r3.w
    dp3 r1.z, r2, t3
    mul r3.xyz, r1.z, t3
    mad_pp r2.xyz, r3, c8.w, -r2
    texld_pp r3, t1, s0
    texld_pp r4, r2, s3
    mul_pp r1.xyz, r1.xyww, r4
    mul_pp r1.xyz, r2.w, r1
    mad_pp r0.xzw, r3.xyyz, r0, r1.xyyz
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mul_pp r0.xzw, r0, r2.xyyz
    mul_pp r0.xzw, r0, c1.w
    add r2.xyz, r0.xzww, r0.xzww
    mad r0.xyz, r0.xzww, -r0.y, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 83 instruction slots used (5 texture, 78 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyComplexFogSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, true, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 1, 0, 0, 0
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
    mad r1.w, r0.w, -r1.w, c6.x
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
    mad r2.w, r1.w, -r2.w, c6.x
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
    mov r1.xy, t1.zwzw
    texld_pp r2, t1, s7
    texld_pp r1, r1, s2
    mad_pp r0.xyz, r1.y, c4, r0
    add_pp r0.w, -r1.w, c6.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    log r3.x, r2.x
    log r3.y, r2.y
    log r3.z, r2.z
    mul r1.xyw, r3.xyzz, c5.z
    exp r2.x, r1.x
    exp r2.y, r1.y
    exp r2.z, r1.w
    mul_sat_pp r1.xyw, r2.xyzz, c5.w
    add r2.xyz, -t0, c8
    nrm r3.xyz, r2
    dp3 r0.w, t3, r3
    mov r3, c10
    mad_pp r3, r0.w, r3, c11
    mul_pp r1.xyw, r1, r3.xyzz
    mul_pp r0.w, r1.z, r3.w
    dp3 r2.w, r2, t3
    mul r3.xyz, r2.w, t3
    mad_pp r2.xyz, r3, c8.w, -r2
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mul_pp r1.xyz, r1.xyww, r2
    mul_pp r1.xyz, r0.w, r1
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mul_pp r0.xyz, r0, r2
    mul r2.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 83 instruction slots used (5 texture, 78 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddComplexFogSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, true, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 1, 2, -1, 0
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
    mov_pp r1.w, c6.x
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
    mov r1.xy, t1.zwzw
    texld_pp r2, t1, s7
    texld_pp r1, r1, s2
    mad_pp r0.xyz, r1.y, c4, r0
    add_pp r0.w, -r1.w, c6.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    log r3.x, r2.x
    log r3.y, r2.y
    log r3.z, r2.z
    mul r1.xyw, r3.xyzz, c5.z
    exp r2.x, r1.x
    exp r2.y, r1.y
    exp r2.z, r1.w
    mul_sat_pp r1.xyw, r2.xyzz, c5.w
    add r2.xyz, -t0, c8
    nrm r3.xyz, r2
    dp3 r0.w, t3, r3
    mov r3, c10
    mad_pp r3, r0.w, r3, c11
    mul_pp r1.xyw, r1, r3.xyzz
    mul_pp r0.w, r1.z, r3.w
    dp3 r2.w, r2, t3
    mul r3.xyz, r2.w, t3
    mad_pp r2.xyz, r3, c8.w, -r2
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mul_pp r1.xyz, r1.xyww, r2
    mul_pp r1.xyz, r0.w, r1
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mad_pp r0.xyz, r2, c6.y, r0
    add_pp r0.xyz, r0, c6.z
    mul r2.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 83 instruction slots used (5 texture, 78 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyComplexFogSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, true, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 1, 0, 0, 0
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
    mad r1.w, r0.w, -r1.w, c6.x
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
    mad r2.w, r1.w, -r2.w, c6.x
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
    mov r1.xy, t1.zwzw
    texld_pp r2, t1, s7
    texld_pp r1, r1, s2
    mad_pp r0.xyz, r1.y, c4, r0
    add_pp r0.w, -r1.w, c6.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mul_pp r1.xyw, r3.xyzz, r4.xyzz
    mul_pp r3.w, r3.w, c0.w
    mul_pp r0.xyz, r0, r1.xyww
    add_pp r0.xyz, r0, r0
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r1.xyw, r4.xyzz, c5.z
    exp r2.x, r1.x
    exp r2.y, r1.y
    exp r2.z, r1.w
    mul_sat_pp r1.xyw, r2.xyzz, c5.w
    add r2.xyz, -t0, c8
    nrm r4.xyz, r2
    dp3 r0.w, t3, r4
    mov r4, c10
    mad_pp r4, r0.w, r4, c11
    mul_pp r1.xyw, r1, r4.xyzz
    mul_pp r0.w, r1.z, r4.w
    dp3 r2.w, r2, t3
    mul r4.xyz, r2.w, t3
    mad_pp r2.xyz, r4, c8.w, -r2
    texld_pp r2, r2, s3
    mul_pp r1.xyz, r1.xyww, r2
    mad_pp r0.xyz, r1, r0.w, r0
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 84 instruction slots used (5 texture, 79 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyComplexFogSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, true, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 1, 0, 0, 0
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
    mad r1.w, r0.w, -r1.w, c6.x
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
    mad r2.w, r1.w, -r2.w, c6.x
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
    mov r1.xy, t1.zwzw
    texld_pp r2, t1, s7
    texld_pp r1, r1, s2
    mad_pp r0.xyz, r1.y, c4, r0
    add_pp r0.w, -r1.w, c6.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    log r3.x, r2.x
    log r3.y, r2.y
    log r3.z, r2.z
    mul r1.xyw, r3.xyzz, c5.z
    exp r2.x, r1.x
    exp r2.y, r1.y
    exp r2.z, r1.w
    mul_sat_pp r1.xyw, r2.xyzz, c5.w
    add r2.xyz, -t0, c8
    nrm r3.xyz, r2
    dp3 r0.w, t3, r3
    mov r3, c10
    mad_pp r3, r0.w, r3, c11
    mul_pp r1.xyw, r1, r3.xyzz
    mul_pp r0.w, r1.z, r3.w
    dp3 r2.w, r2, t3
    mul r3.xyz, r2.w, t3
    mad_pp r2.xyz, r3, c8.w, -r2
    texld_pp r2, r2, s3
    mul_pp r1.xyz, r1.xyww, r2
    mul_pp r1.xyz, r0.w, r1
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r0.xyz, r2, r0, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 83 instruction slots used (5 texture, 78 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddComplexFogSpecMap
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, true, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 1, 2, -1, 0
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
    mov_pp r1.w, c6.x
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
    mov r1.xy, t1.zwzw
    texld_pp r2, t1, s7
    texld_pp r1, r1, s2
    mad_pp r0.xyz, r1.y, c4, r0
    add_pp r0.w, -r1.w, c6.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    log r3.x, r2.x
    log r3.y, r2.y
    log r3.z, r2.z
    mul r1.xyw, r3.xyzz, c5.z
    exp r2.x, r1.x
    exp r2.y, r1.y
    exp r2.z, r1.w
    mul_sat_pp r1.xyw, r2.xyzz, c5.w
    add r2.xyz, -t0, c8
    nrm r3.xyz, r2
    dp3 r0.w, t3, r3
    mov r3, c10
    mad_pp r3, r0.w, r3, c11
    mul_pp r1.xyw, r1, r3.xyzz
    mul_pp r0.w, r1.z, r3.w
    dp3 r2.w, r2, t3
    mul r3.xyz, r2.w, t3
    mad_pp r2.xyz, r3, c8.w, -r2
    texld_pp r2, r2, s3
    mul_pp r1.xyz, r1.xyww, r2
    mul_pp r1.xyz, r0.w, r1
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c6.y, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c6.z
    mad_pp r0.xyz, r2, r0, r1
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 83 instruction slots used (5 texture, 78 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplySpecMapSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mad r1.w, r0.w, -r1.w, c2.x
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
    mad r2.w, r1.w, -r2.w, c2.x
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
    mov r1.xy, t1.zwzw
    texld_pp r2, t1, s7
    texld_pp r1, r1, s2
    mad_pp r0.xyz, r1.y, c1, r0
    add_pp r0.w, -r1.w, c2.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    log r3.x, r2.x
    log r3.y, r2.y
    log r3.z, r2.z
    mul r1.xyw, r3.xyzz, c5.z
    exp r2.x, r1.x
    exp r2.y, r1.y
    exp r2.z, r1.w
    mul_sat_pp r1.xyw, r2.xyzz, c5.w
    add r2.xyz, -t0, c8
    nrm r3.xyz, r2
    dp3 r0.w, t3, r3
    mov r3, c10
    mad_pp r3, r0.w, r3, c11
    mul_pp r1.xyw, r1, r3.xyzz
    mul_pp r0.w, r1.z, r3.w
    dp3 r2.w, r2, t3
    mul r3.xyz, r2.w, t3
    mad_pp r2.xyz, r3, c8.w, -r2
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mul_pp r1.xyz, r1.xyww, r2
    mul_pp r1.xyz, r0.w, r1
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mul_pp r0.xyz, r0, r2
    add_pp r1.xyz, r0, r0
    mov_pp oC0, r1

// approximately 78 instruction slots used (5 texture, 73 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplySpecMapSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mad r1.w, r0.w, -r1.w, c2.x
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
    mad r2.w, r1.w, -r2.w, c2.x
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
    mov r1.xy, t1.zwzw
    texld_pp r2, t1, s7
    texld_pp r1, r1, s2
    mad_pp r0.xyz, r1.y, c1, r0
    add_pp r0.w, -r1.w, c2.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    log r3.x, r2.x
    log r3.y, r2.y
    log r3.z, r2.z
    mul r1.xyw, r3.xyzz, c5.z
    exp r2.x, r1.x
    exp r2.y, r1.y
    exp r2.z, r1.w
    mul_sat_pp r1.xyw, r2.xyzz, c5.w
    add r2.xyz, -t0, c8
    nrm r3.xyz, r2
    dp3 r0.w, t3, r3
    mov r3, c10
    mad_pp r3, r0.w, r3, c11
    mul_pp r1.xyw, r1, r3.xyzz
    mul_pp r0.w, r1.z, r3.w
    dp3 r2.w, r2, t3
    mul r3.xyz, r2.w, t3
    mad_pp r2.xyz, r3, c8.w, -r2
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mul_pp r1.xyz, r1.xyww, r2
    mul_pp r1.xyz, r0.w, r1
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mul_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 77 instruction slots used (5 texture, 72 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddSpecMapSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 2, -1, 0
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
    mov_pp r1.w, c2.x
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
    mov r1.xy, t1.zwzw
    texld_pp r2, t1, s7
    texld_pp r1, r1, s2
    mad_pp r0.xyz, r1.y, c1, r0
    add_pp r0.w, -r1.w, c2.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    log r3.x, r2.x
    log r3.y, r2.y
    log r3.z, r2.z
    mul r1.xyw, r3.xyzz, c5.z
    exp r2.x, r1.x
    exp r2.y, r1.y
    exp r2.z, r1.w
    mul_sat_pp r1.xyw, r2.xyzz, c5.w
    add r2.xyz, -t0, c8
    nrm r3.xyz, r2
    dp3 r0.w, t3, r3
    mov r3, c10
    mad_pp r3, r0.w, r3, c11
    mul_pp r1.xyw, r1, r3.xyzz
    mul_pp r0.w, r1.z, r3.w
    dp3 r2.w, r2, t3
    mul r3.xyz, r2.w, t3
    mad_pp r2.xyz, r3, c8.w, -r2
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mul_pp r1.xyz, r1.xyww, r2
    mul_pp r1.xyz, r0.w, r1
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.w, r3.w, c0.w
    texld_pp r2, t2, s1
    mad_pp r0.xyz, r2, c2.y, r0
    add_pp r1.xyz, r0, c2.z
    mov_pp oC0, r1

// approximately 77 instruction slots used (5 texture, 72 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplySpecMapSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mad r1.w, r0.w, -r1.w, c2.x
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
    mad r2.w, r1.w, -r2.w, c2.x
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
    mov r1.xy, t1.zwzw
    texld_pp r2, t1, s7
    texld_pp r1, r1, s2
    mad_pp r0.xyz, r1.y, c1, r0
    add_pp r0.w, -r1.w, c2.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    texld_pp r3, t1, s0
    texld_pp r4, t2, s1
    mul_pp r1.xyw, r3.xyzz, r4.xyzz
    mul_pp r3.w, r3.w, c0.w
    mul_pp r0.xyz, r0, r1.xyww
    add_pp r0.xyz, r0, r0
    log r4.x, r2.x
    log r4.y, r2.y
    log r4.z, r2.z
    mul r1.xyw, r4.xyzz, c5.z
    exp r2.x, r1.x
    exp r2.y, r1.y
    exp r2.z, r1.w
    mul_sat_pp r1.xyw, r2.xyzz, c5.w
    add r2.xyz, -t0, c8
    nrm r4.xyz, r2
    dp3 r0.w, t3, r4
    mov r4, c10
    mad_pp r4, r0.w, r4, c11
    mul_pp r1.xyw, r1, r4.xyzz
    mul_pp r0.w, r1.z, r4.w
    dp3 r2.w, r2, t3
    mul r4.xyz, r2.w, t3
    mad_pp r2.xyz, r4, c8.w, -r2
    texld_pp r2, r2, s3
    mul_pp r1.xyz, r1.xyww, r2
    mad_pp r3.xyz, r1, r0.w, r0
    mov_pp oC0, r3

// approximately 78 instruction slots used (5 texture, 73 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplySpecMapSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mad r1.w, r0.w, -r1.w, c2.x
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
    mad r2.w, r1.w, -r2.w, c2.x
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
    mov r1.xy, t1.zwzw
    texld_pp r2, t1, s7
    texld_pp r1, r1, s2
    mad_pp r0.xyz, r1.y, c1, r0
    add_pp r0.w, -r1.w, c2.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    log r3.x, r2.x
    log r3.y, r2.y
    log r3.z, r2.z
    mul r1.xyw, r3.xyzz, c5.z
    exp r2.x, r1.x
    exp r2.y, r1.y
    exp r2.z, r1.w
    mul_sat_pp r1.xyw, r2.xyzz, c5.w
    add r2.xyz, -t0, c8
    nrm r3.xyz, r2
    dp3 r0.w, t3, r3
    mov r3, c10
    mad_pp r3, r0.w, r3, c11
    mul_pp r1.xyw, r1, r3.xyzz
    mul_pp r0.w, r1.z, r3.w
    dp3 r2.w, r2, t3
    mul r3.xyz, r2.w, t3
    mad_pp r2.xyz, r3, c8.w, -r2
    texld_pp r2, r2, s3
    mul_pp r1.xyz, r1.xyww, r2
    mul_pp r1.xyz, r0.w, r1
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r3.xyz, r2, r0, r1
    mov_pp oC0, r3

// approximately 77 instruction slots used (5 texture, 72 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddSpecMapSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, false, false, true, false);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 2, -1, 0
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
    mov_pp r1.w, c2.x
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
    mov r1.xy, t1.zwzw
    texld_pp r2, t1, s7
    texld_pp r1, r1, s2
    mad_pp r0.xyz, r1.y, c1, r0
    add_pp r0.w, -r1.w, c2.x
    mad_pp r1.xyw, r1.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r1.xyww
    log r3.x, r2.x
    log r3.y, r2.y
    log r3.z, r2.z
    mul r1.xyw, r3.xyzz, c5.z
    exp r2.x, r1.x
    exp r2.y, r1.y
    exp r2.z, r1.w
    mul_sat_pp r1.xyw, r2.xyzz, c5.w
    add r2.xyz, -t0, c8
    nrm r3.xyz, r2
    dp3 r0.w, t3, r3
    mov r3, c10
    mad_pp r3, r0.w, r3, c11
    mul_pp r1.xyw, r1, r3.xyzz
    mul_pp r0.w, r1.z, r3.w
    dp3 r2.w, r2, t3
    mul r3.xyz, r2.w, t3
    mad_pp r2.xyz, r3, c8.w, -r2
    texld_pp r2, r2, s3
    mul_pp r1.xyz, r1.xyww, r2
    mul_pp r1.xyz, r0.w, r1
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c2.y, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c2.z
    mad_pp r3.xyz, r2, r0, r1
    mov_pp oC0, r3

// approximately 77 instruction slots used (5 texture, 72 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplySpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mov r1.w, c2.x
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    mad_pp r0.xyz, r6.y, c1, r0
    add_pp r0.w, -r6.w, c2.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    dp3 r1.w, r4, t3
    mul r2.xyz, r1.w, t3
    mad_pp r2.xyz, r2, c8.w, -r4
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.w, r3.w, c0.w
    mul_pp r2.xyz, r5, r2
    mad_pp r0.xyz, r2, r0.w, r0
    texld_pp r2, t2, s1
    mul_pp r0.xyz, r0, r2
    add_pp r1.xyz, r0, r0
    mov_pp oC0, r1

// approximately 122 instruction slots used (5 texture, 117 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplySpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mov r1.w, c2.x
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    mad_pp r0.xyz, r6.y, c1, r0
    add_pp r0.w, -r6.w, c2.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    dp3 r1.w, r4, t3
    mul r2.xyz, r1.w, t3
    mad_pp r2.xyz, r2, c8.w, -r4
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.w, r3.w, c0.w
    mul_pp r2.xyz, r5, r2
    mad_pp r0.xyz, r2, r0.w, r0
    texld_pp r2, t2, s1
    mul_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 121 instruction slots used (5 texture, 116 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 2, -1
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
    mov r1.w, c2.x
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    mad_pp r0.xyz, r6.y, c1, r0
    add_pp r0.w, -r6.w, c2.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    dp3 r1.w, r4, t3
    mul r2.xyz, r1.w, t3
    mad_pp r2.xyz, r2, c8.w, -r4
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.w, r3.w, c0.w
    mul_pp r2.xyz, r5, r2
    mad_pp r0.xyz, r2, r0.w, r0
    texld_pp r2, t2, s1
    mad_pp r0.xyz, r2, c2.z, r0
    add_pp r1.xyz, r0, c2.w
    mov_pp oC0, r1

// approximately 122 instruction slots used (5 texture, 117 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplySpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mov r1.w, c2.x
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    mad_pp r0.xyz, r6.y, c1, r0
    add_pp r0.w, -r6.w, c2.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.xyz, r0, r2
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mul_pp r0.xyz, r0, r2
    add_pp r0.xyz, r0, r0
    mad_pp r0.xyz, r1, r6.z, r0
    mul_pp r0.w, r5.w, r6.z
    dp3 r4.w, r4, t3
    mul r1.xyz, r4.w, t3
    mad_pp r1.xyz, r1, c8.w, -r4
    texld_pp r1, r1, s3
    mul_pp r1.xyz, r5, r1
    mad_pp r3.xyz, r1, r0.w, r0
    mov_pp oC0, r3

// approximately 122 instruction slots used (5 texture, 117 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplySpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mov r1.w, c2.x
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    mad_pp r0.xyz, r6.y, c1, r0
    add_pp r0.w, -r6.w, c2.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r0.xyz, r2, r0, r1
    dp3 r4.w, r4, t3
    mul r1.xyz, r4.w, t3
    mad_pp r1.xyz, r1, c8.w, -r4
    texld_pp r1, r1, s3
    mul_pp r1.xyz, r5, r1
    mad_pp r3.xyz, r1, r0.w, r0
    mov_pp oC0, r3

// approximately 121 instruction slots used (5 texture, 116 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 2, -1
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
    mov r1.w, c2.x
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    mad_pp r0.xyz, r6.y, c1, r0
    add_pp r0.w, -r6.w, c2.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c2.z, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c2.w
    mad_pp r0.xyz, r2, r0, r1
    dp3 r4.w, r4, t3
    mul r1.xyz, r4.w, t3
    mad_pp r1.xyz, r1, c8.w, -r4
    texld_pp r1, r1, s3
    mul_pp r1.xyz, r5, r1
    mad_pp r3.xyz, r1, r0.w, r0
    mov_pp oC0, r3

// approximately 122 instruction slots used (5 texture, 117 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyComplexFogSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, true, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 1, 0, 2, 0
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
    mov r0.xz, c6
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
    max r2.w, r1.w, c6.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r0.xyw, r1.w, c17.xyzz
    mul r0.xyw, r3.y, r0
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r4.w, r2.w, c6.y
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
    max r4.w, r2.w, c6.y
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
    max r4.w, r2.w, c6.y
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
    mad_pp r1.xyz, r6.y, c4, r1
    add_pp r1.w, -r6.w, c6.x
    mad_pp r2.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r5.w, r6.z
    mul_pp r1.xyz, r1, r2
    dp3 r4.w, r4, t3
    mul r2.xyz, r4.w, t3
    mad_pp r2.xyz, r2, c8.w, -r4
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mad_pp r0.xyw, r3.xyzz, r1.xyzz, r0
    mul_pp r3.w, r3.w, c0.w
    mul_pp r1.xyz, r5, r2
    mad_pp r0.xyw, r1.xyzz, r1.w, r0
    texld_pp r1, t2, s1
    mul_pp r0.xyw, r0, r1.xyzz
    mul_pp r0.xyw, r0, c1.w
    add r1.xyz, r0.xyww, r0.xyww
    mad r0.xyz, r0.xyww, -r0.z, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 128 instruction slots used (5 texture, 123 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyComplexFogSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, true, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 1, 0, 0, 0
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
    mov r1.w, c6.x
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
    max r2.w, r1.w, c6.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c6.y
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
    max r2.w, r1.w, c6.y
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
    max r2.w, r1.w, c6.y
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
    mad_pp r0.xyz, r6.y, c4, r0
    add_pp r0.w, -r6.w, c6.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    dp3 r1.w, r4, t3
    mul r2.xyz, r1.w, t3
    mad_pp r2.xyz, r2, c8.w, -r4
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.w, r3.w, c0.w
    mul_pp r2.xyz, r5, r2
    mad_pp r0.xyz, r2, r0.w, r0
    texld_pp r2, t2, s1
    mul_pp r0.xyz, r0, r2
    mul r2.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 127 instruction slots used (5 texture, 122 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddComplexFogSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, true, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 1, 0, 2, -1
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
    mov r1.w, c6.x
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
    max r2.w, r1.w, c6.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c6.y
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
    max r2.w, r1.w, c6.y
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
    max r2.w, r1.w, c6.y
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
    mad_pp r0.xyz, r6.y, c4, r0
    add_pp r0.w, -r6.w, c6.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    dp3 r1.w, r4, t3
    mul r2.xyz, r1.w, t3
    mad_pp r2.xyz, r2, c8.w, -r4
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.w, r3.w, c0.w
    mul_pp r2.xyz, r5, r2
    mad_pp r0.xyz, r2, r0.w, r0
    texld_pp r2, t2, s1
    mad_pp r0.xyz, r2, c6.z, r0
    add_pp r0.xyz, r0, c6.w
    mul r2.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 128 instruction slots used (5 texture, 123 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyComplexFogSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, true, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 1, 0, 0, 0
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
    mov r1.w, c6.x
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
    max r2.w, r1.w, c6.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c6.y
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
    max r2.w, r1.w, c6.y
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
    max r2.w, r1.w, c6.y
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
    mad_pp r0.xyz, r6.y, c4, r0
    add_pp r0.w, -r6.w, c6.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.xyz, r0, r2
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mul_pp r0.xyz, r0, r2
    add_pp r0.xyz, r0, r0
    mad_pp r0.xyz, r1, r6.z, r0
    mul_pp r0.w, r5.w, r6.z
    dp3 r4.w, r4, t3
    mul r1.xyz, r4.w, t3
    mad_pp r1.xyz, r1, c8.w, -r4
    texld_pp r1, r1, s3
    mul_pp r1.xyz, r5, r1
    mad_pp r0.xyz, r1, r0.w, r0
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 128 instruction slots used (5 texture, 123 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyComplexFogSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, true, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 1, 0, 0, 0
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
    mov r1.w, c6.x
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
    max r2.w, r1.w, c6.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c6.y
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
    max r2.w, r1.w, c6.y
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
    max r2.w, r1.w, c6.y
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
    mad_pp r0.xyz, r6.y, c4, r0
    add_pp r0.w, -r6.w, c6.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r0.xyz, r2, r0, r1
    dp3 r4.w, r4, t3
    mul r1.xyz, r4.w, t3
    mad_pp r1.xyz, r1, c8.w, -r4
    texld_pp r1, r1, s3
    mul_pp r1.xyz, r5, r1
    mad_pp r0.xyz, r1, r0.w, r0
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 127 instruction slots used (5 texture, 122 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddComplexFogSpecMapSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, true, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c6, 1, 0, 2, -1
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
    mov r1.w, c6.x
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
    max r2.w, r1.w, c6.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c6.y
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
    max r2.w, r1.w, c6.y
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
    max r2.w, r1.w, c6.y
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
    mad_pp r0.xyz, r6.y, c4, r0
    add_pp r0.w, -r6.w, c6.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c6.z, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c6.w
    mad_pp r0.xyz, r2, r0, r1
    dp3 r4.w, r4, t3
    mul r1.xyz, r4.w, t3
    mad_pp r1.xyz, r1, c8.w, -r4
    texld_pp r1, r1, s3
    mul_pp r1.xyz, r5, r1
    mad_pp r0.xyz, r1, r0.w, r0
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 128 instruction slots used (5 texture, 123 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplySpecMapSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mov r1.w, c2.x
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    mad_pp r0.xyz, r6.y, c1, r0
    add_pp r0.w, -r6.w, c2.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    dp3 r1.w, r4, t3
    mul r2.xyz, r1.w, t3
    mad_pp r2.xyz, r2, c8.w, -r4
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.w, r3.w, c0.w
    mul_pp r2.xyz, r5, r2
    mad_pp r0.xyz, r2, r0.w, r0
    texld_pp r2, t2, s1
    mul_pp r0.xyz, r0, r2
    add_pp r1.xyz, r0, r0
    mov_pp oC0, r1

// approximately 122 instruction slots used (5 texture, 117 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplySpecMapSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mov r1.w, c2.x
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    mad_pp r0.xyz, r6.y, c1, r0
    add_pp r0.w, -r6.w, c2.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    dp3 r1.w, r4, t3
    mul r2.xyz, r1.w, t3
    mad_pp r2.xyz, r2, c8.w, -r4
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.w, r3.w, c0.w
    mul_pp r2.xyz, r5, r2
    mad_pp r0.xyz, r2, r0.w, r0
    texld_pp r2, t2, s1
    mul_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 121 instruction slots used (5 texture, 116 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddSpecMapSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 2, -1
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
    mov r1.w, c2.x
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    mad_pp r0.xyz, r6.y, c1, r0
    add_pp r0.w, -r6.w, c2.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    dp3 r1.w, r4, t3
    mul r2.xyz, r1.w, t3
    mad_pp r2.xyz, r2, c8.w, -r4
    texld_pp r3, t1, s0
    texld_pp r2, r2, s3
    mad_pp r0.xyz, r3, r0, r1
    mul_pp r1.w, r3.w, c0.w
    mul_pp r2.xyz, r5, r2
    mad_pp r0.xyz, r2, r0.w, r0
    texld_pp r2, t2, s1
    mad_pp r0.xyz, r2, c2.z, r0
    add_pp r1.xyz, r0, c2.w
    mov_pp oC0, r1

// approximately 122 instruction slots used (5 texture, 117 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplySpecMapSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mov r1.w, c2.x
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    mad_pp r0.xyz, r6.y, c1, r0
    add_pp r0.w, -r6.w, c2.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.xyz, r0, r2
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mul_pp r0.xyz, r0, r2
    add_pp r0.xyz, r0, r0
    mad_pp r0.xyz, r1, r6.z, r0
    mul_pp r0.w, r5.w, r6.z
    dp3 r4.w, r4, t3
    mul r1.xyz, r4.w, t3
    mad_pp r1.xyz, r1, c8.w, -r4
    texld_pp r1, r1, s3
    mul_pp r1.xyz, r5, r1
    mad_pp r3.xyz, r1, r0.w, r0
    mov_pp oC0, r3

// approximately 122 instruction slots used (5 texture, 117 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplySpecMapSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mov r1.w, c2.x
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    mad_pp r0.xyz, r6.y, c1, r0
    add_pp r0.w, -r6.w, c2.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r0.xyz, r2, r0, r1
    dp3 r4.w, r4, t3
    mul r1.xyz, r4.w, t3
    mad_pp r1.xyz, r1, c8.w, -r4
    texld_pp r1, r1, s3
    mul_pp r1.xyz, r5, r1
    mad_pp r3.xyz, r1, r0.w, r0
    mov_pp oC0, r3

// approximately 121 instruction slots used (5 texture, 116 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddSpecMapSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, false, false, true, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 2, -1
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
    mov r1.w, c2.x
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    mad_pp r0.xyz, r6.y, c1, r0
    add_pp r0.w, -r6.w, c2.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c2.z, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c2.w
    mad_pp r0.xyz, r2, r0, r1
    dp3 r4.w, r4, t3
    mul r1.xyz, r4.w, t3
    mad_pp r1.xyz, r1, c8.w, -r4
    texld_pp r1, r1, s3
    mul_pp r1.xyz, r5, r1
    mad_pp r3.xyz, r1, r0.w, r0
    mov_pp oC0, r3

// approximately 122 instruction slots used (5 texture, 117 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplySpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mov r1.w, c2.x
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    mad_pp r0.xyz, r6.y, c1, r0
    add_pp r0.w, -r6.w, c2.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    mad_pp r0.xyz, r7, r0, r1
    mul_pp r1.w, r7.w, c0.w
    dp3 r4.w, r4, t3
    mul r2.xyz, r4.w, t3
    mad_pp r2.xyz, r2, c8.w, -r4
    texld_pp r3, t2, s1
    texld_pp r2, r2, s3
    mul_pp r2.xyz, r5, r2
    mad_pp r0.xyz, r2, r0.w, r0
    mul_pp r0.xyz, r3, r0
    add_pp r1.xyz, r0, r0
    mov_pp oC0, r1

// approximately 112 instruction slots used (4 texture, 108 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplySpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mov r1.w, c2.x
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    mad_pp r0.xyz, r6.y, c1, r0
    add_pp r0.w, -r6.w, c2.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    mad_pp r0.xyz, r7, r0, r1
    mul_pp r1.w, r7.w, c0.w
    dp3 r4.w, r4, t3
    mul r2.xyz, r4.w, t3
    mad_pp r2.xyz, r2, c8.w, -r4
    texld_pp r3, t2, s1
    texld_pp r2, r2, s3
    mul_pp r2.xyz, r5, r2
    mad_pp r0.xyz, r2, r0.w, r0
    mul_pp r1.xyz, r3, r0
    mov_pp oC0, r1

// approximately 111 instruction slots used (4 texture, 107 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 2, -1
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
    mov r1.w, c2.x
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    mad_pp r0.xyz, r6.y, c1, r0
    add_pp r0.w, -r6.w, c2.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    mad_pp r0.xyz, r7, r0, r1
    mul_pp r1.w, r7.w, c0.w
    dp3 r4.w, r4, t3
    mul r2.xyz, r4.w, t3
    mad_pp r2.xyz, r2, c8.w, -r4
    texld_pp r3, t2, s1
    texld_pp r2, r2, s3
    mul_pp r2.xyz, r5, r2
    mad_pp r0.xyz, r2, r0.w, r0
    mad_pp r0.xyz, r3, c2.z, r0
    add_pp r1.xyz, r0, c2.w
    mov_pp oC0, r1

// approximately 112 instruction slots used (4 texture, 108 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplySpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mov r1.w, c2.x
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mul_sat_pp r1.xyz, r5, r1
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    mov r2.xy, t1.zwzw
    dp3 r0.w, r4, t3
    mul r3.xyz, r0.w, t3
    mad_pp r3.xyz, r3, c8.w, -r4
    texld_pp r2, r2, s2
    texld_pp r3, r3, s3
    mad_pp r0.xyz, r2.y, c1, r0
    add_pp r0.w, -r2.w, c2.x
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r2.xyww
    texld_pp r4, t1, s0
    texld_pp r6, t2, s1
    mul_pp r2.xyw, r4.xyzz, r6.xyzz
    mul_pp r4.w, r4.w, c0.w
    mul_pp r0.xyz, r0, r2.xyww
    add_pp r0.xyz, r0, r0
    mad_pp r0.xyz, r1, r2.z, r0
    mul_pp r0.w, r5.w, r2.z
    mul_pp r1.xyz, r5, r3
    mad_pp r4.xyz, r1, r0.w, r0
    mov_pp oC0, r4

// approximately 112 instruction slots used (4 texture, 108 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplySpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mov r1.w, c2.x
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    mad_pp r0.xyz, r6.y, c1, r0
    add_pp r0.w, -r6.w, c2.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r0.xyz, r2, r0, r1
    mul_pp r1.xyz, r5, r4
    mad_pp r3.xyz, r1, r0.w, r0
    mov_pp oC0, r3

// approximately 111 instruction slots used (4 texture, 107 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 2, -1
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
    mov r1.w, c2.x
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    mad_pp r0.xyz, r6.y, c1, r0
    add_pp r0.w, -r6.w, c2.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c2.z, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c2.w
    mad_pp r0.xyz, r2, r0, r1
    mul_pp r1.xyz, r5, r4
    mad_pp r3.xyz, r1, r0.w, r0
    mov_pp oC0, r3

// approximately 112 instruction slots used (4 texture, 108 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplyComplexFogSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, true, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c5, 1, 0, 2, 0
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
    mov r0.xz, c5
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
    max r2.w, r1.w, c5.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r0.xyw, r1.w, c17.xyzz
    mul r0.xyw, r3.y, r0
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r4.w, r2.w, c5.y
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
    max r4.w, r2.w, c5.y
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
    max r4.w, r2.w, c5.y
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
    mad_pp r1.xyz, r6.y, c4, r1
    add_pp r1.w, -r6.w, c5.x
    mad_pp r2.xyz, r6.w, c0, r1.w
    mul_pp r1.w, r5.w, r6.z
    mul_pp r1.xyz, r1, r2
    mad_pp r0.xyw, r7.xyzz, r1.xyzz, r0
    mul_pp r2.w, r7.w, c0.w
    dp3 r4.w, r4, t3
    mul r1.xyz, r4.w, t3
    mad_pp r1.xyz, r1, c8.w, -r4
    texld_pp r3, t2, s1
    texld_pp r4, r1, s3
    mul_pp r1.xyz, r5, r4
    mad_pp r0.xyw, r1.xyzz, r1.w, r0
    mul_pp r0.xyw, r3.xyzz, r0
    mul_pp r0.xyw, r0, c1.w
    add r1.xyz, r0.xyww, r0.xyww
    mad r0.xyz, r0.xyww, -r0.z, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r2.xyz, r0, r1
    mov_pp oC0, r2

// approximately 118 instruction slots used (4 texture, 114 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplyComplexFogSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, true, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c5, 1, 0, 0, 0
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
    mov r1.w, c5.x
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
    max r2.w, r1.w, c5.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c5.y
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
    max r2.w, r1.w, c5.y
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
    max r2.w, r1.w, c5.y
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
    mad_pp r0.xyz, r6.y, c4, r0
    add_pp r0.w, -r6.w, c5.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    mad_pp r0.xyz, r7, r0, r1
    mul_pp r1.w, r7.w, c0.w
    dp3 r4.w, r4, t3
    mul r2.xyz, r4.w, t3
    mad_pp r2.xyz, r2, c8.w, -r4
    texld_pp r3, t2, s1
    texld_pp r2, r2, s3
    mul_pp r2.xyz, r5, r2
    mad_pp r0.xyz, r2, r0.w, r0
    mul_pp r0.xyz, r3, r0
    mul r2.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 117 instruction slots used (4 texture, 113 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddComplexFogSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, true, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c5, 1, 0, 2, -1
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
    mov r1.w, c5.x
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
    max r2.w, r1.w, c5.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c5.y
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
    max r2.w, r1.w, c5.y
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
    max r2.w, r1.w, c5.y
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
    mad_pp r0.xyz, r6.y, c4, r0
    add_pp r0.w, -r6.w, c5.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    mad_pp r0.xyz, r7, r0, r1
    mul_pp r1.w, r7.w, c0.w
    dp3 r4.w, r4, t3
    mul r2.xyz, r4.w, t3
    mad_pp r2.xyz, r2, c8.w, -r4
    texld_pp r3, t2, s1
    texld_pp r2, r2, s3
    mul_pp r2.xyz, r5, r2
    mad_pp r0.xyz, r2, r0.w, r0
    mad_pp r0.xyz, r3, c5.z, r0
    add_pp r0.xyz, r0, c5.w
    mul r2.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r2
    mov r2.xyz, c2
    mad_sat_pp r2.xyz, t2.z, -r2, c3
    add_pp r1.xyz, r0, r2
    mov_pp oC0, r1

// approximately 118 instruction slots used (4 texture, 114 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplyComplexFogSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, true, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c5, 1, 0, 0, 0
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
    mov r1.w, c5.x
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
    max r2.w, r1.w, c5.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c5.y
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
    max r2.w, r1.w, c5.y
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
    max r2.w, r1.w, c5.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mul_sat_pp r1.xyz, r5, r1
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    mov r2.xy, t1.zwzw
    dp3 r0.w, r4, t3
    mul r3.xyz, r0.w, t3
    mad_pp r3.xyz, r3, c8.w, -r4
    texld_pp r2, r2, s2
    texld_pp r3, r3, s3
    mad_pp r0.xyz, r2.y, c4, r0
    add_pp r0.w, -r2.w, c5.x
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r2.xyww
    texld_pp r4, t1, s0
    texld_pp r6, t2, s1
    mul_pp r2.xyw, r4.xyzz, r6.xyzz
    mul_pp r4.w, r4.w, c0.w
    mul_pp r0.xyz, r0, r2.xyww
    add_pp r0.xyz, r0, r0
    mad_pp r0.xyz, r1, r2.z, r0
    mul_pp r0.w, r5.w, r2.z
    mul_pp r1.xyz, r5, r3
    mad_pp r0.xyz, r1, r0.w, r0
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r4.xyz, r0, r1
    mov_pp oC0, r4

// approximately 118 instruction slots used (4 texture, 114 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplyComplexFogSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, true, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c5, 1, 0, 0, 0
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
    mov r1.w, c5.x
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
    max r2.w, r1.w, c5.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c5.y
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
    max r2.w, r1.w, c5.y
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
    max r2.w, r1.w, c5.y
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
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    mad_pp r0.xyz, r6.y, c4, r0
    add_pp r0.w, -r6.w, c5.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r0.xyz, r2, r0, r1
    mul_pp r1.xyz, r5, r4
    mad_pp r0.xyz, r1, r0.w, r0
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 117 instruction slots used (4 texture, 113 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddComplexFogSpecLighting
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, true, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c5, 1, 0, 2, -1
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
    mov r1.w, c5.x
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
    max r2.w, r1.w, c5.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c5.y
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
    max r2.w, r1.w, c5.y
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
    max r2.w, r1.w, c5.y
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
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    mad_pp r0.xyz, r6.y, c4, r0
    add_pp r0.w, -r6.w, c5.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c5.z, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c5.w
    mad_pp r0.xyz, r2, r0, r1
    mul_pp r1.xyz, r5, r4
    mad_pp r0.xyz, r1, r0.w, r0
    mul r1.xyz, r0, c1.w
    mad r0.xyz, r0, -c1.w, c1
    mad r0.xyz, t2.z, r0, r1
    mov r1.xyz, c2
    mad_sat_pp r1.xyz, t2.z, -r1, c3
    add_pp r3.xyz, r0, r1
    mov_pp oC0, r3

// approximately 118 instruction slots used (4 texture, 114 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedMultiplySpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, true, false, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mov r1.w, c2.x
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    mad_pp r0.xyz, r6.y, c1, r0
    add_pp r0.w, -r6.w, c2.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    mad_pp r0.xyz, r7, r0, r1
    mul_pp r1.w, r7.w, c0.w
    dp3 r4.w, r4, t3
    mul r2.xyz, r4.w, t3
    mad_pp r2.xyz, r2, c8.w, -r4
    texld_pp r3, t2, s1
    texld_pp r2, r2, s3
    mul_pp r2.xyz, r5, r2
    mad_pp r0.xyz, r2, r0.w, r0
    mul_pp r0.xyz, r3, r0
    add_pp r1.xyz, r0, r0
    mov_pp oC0, r1

// approximately 112 instruction slots used (4 texture, 108 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionMultiplySpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, true, false, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mov r1.w, c2.x
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    mad_pp r0.xyz, r6.y, c1, r0
    add_pp r0.w, -r6.w, c2.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    mad_pp r0.xyz, r7, r0, r1
    mul_pp r1.w, r7.w, c0.w
    dp3 r4.w, r4, t3
    mul r2.xyz, r4.w, t3
    mad_pp r2.xyz, r2, c8.w, -r4
    texld_pp r3, t2, s1
    texld_pp r2, r2, s3
    mul_pp r2.xyz, r5, r2
    mad_pp r0.xyz, r2, r0.w, r0
    mul_pp r1.xyz, r3, r0
    mov_pp oC0, r1

// approximately 111 instruction slots used (4 texture, 107 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailBeforeReflectionBiasedAddSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, true, false, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 2, -1
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
    mov r1.w, c2.x
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    mad_pp r0.xyz, r6.y, c1, r0
    add_pp r0.w, -r6.w, c2.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    mad_pp r0.xyz, r7, r0, r1
    mul_pp r1.w, r7.w, c0.w
    dp3 r4.w, r4, t3
    mul r2.xyz, r4.w, t3
    mad_pp r2.xyz, r2, c8.w, -r4
    texld_pp r3, t2, s1
    texld_pp r2, r2, s3
    mul_pp r2.xyz, r5, r2
    mad_pp r0.xyz, r2, r0.w, r0
    mad_pp r0.xyz, r3, c2.z, r0
    add_pp r1.xyz, r0, c2.w
    mov_pp oC0, r1

// approximately 112 instruction slots used (4 texture, 108 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedMultiplySpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_multiply, false, false, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mov r1.w, c2.x
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r6.xyz, r1.w, c21
    mad_pp r1.xyz, r6, r3.y, r1
    mul_sat_pp r1.xyz, r5, r1
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    mov r2.xy, t1.zwzw
    dp3 r0.w, r4, t3
    mul r3.xyz, r0.w, t3
    mad_pp r3.xyz, r3, c8.w, -r4
    texld_pp r2, r2, s2
    texld_pp r3, r3, s3
    mad_pp r0.xyz, r2.y, c1, r0
    add_pp r0.w, -r2.w, c2.x
    mad_pp r2.xyw, r2.w, c0.xyzz, r0.w
    mul_pp r0.xyz, r0, r2.xyww
    texld_pp r4, t1, s0
    texld_pp r6, t2, s1
    mul_pp r2.xyw, r4.xyzz, r6.xyzz
    mul_pp r4.w, r4.w, c0.w
    mul_pp r0.xyz, r0, r2.xyww
    add_pp r0.xyz, r0, r0
    mad_pp r0.xyz, r1, r2.z, r0
    mul_pp r0.w, r5.w, r2.z
    mul_pp r1.xyz, r5, r3
    mad_pp r4.xyz, r1, r0.w, r0
    mov_pp oC0, r4

// approximately 112 instruction slots used (4 texture, 108 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionMultiplySpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_multiply, false, false, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 0, 0
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
    mov r1.w, c2.x
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    mad_pp r0.xyz, r6.y, c1, r0
    add_pp r0.w, -r6.w, c2.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mul_pp r2.xyz, r2, r3
    mul_pp r3.w, r2.w, c0.w
    mad_pp r0.xyz, r2, r0, r1
    mul_pp r1.xyz, r5, r4
    mad_pp r3.xyz, r1, r0.w, r0
    mov_pp oC0, r3

// approximately 111 instruction slots used (4 texture, 107 arithmetic)
		};
#endif
	}
}

Technique NoMaskDetailAfterReflectionBiasedAddSpecLightingSelfIllumination
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT ModelNoMask(detail_function_biased_add, false, false, false, false, false, false, true);
#else
		PixelShader = asm
		{
    ps_2_x
    def c2, 1, 0, 2, -1
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
    mov r1.w, c2.x
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
    max r2.w, r1.w, c2.y
    pow r1.w, r2.w, c7.z
    mul r1.w, r1.w, c7.w
    mul r3.xyz, r1.w, c17
    mul r3.xyz, r0.y, r3
    dp3 r1.w, -r1, t3
    add r1.w, r1.w, r1.w
    mad r1.xyz, t3, -r1.w, -r1
    dp3 r2.w, r1, r5
    max r3.w, r2.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    max r2.w, r1.w, c2.y
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
    mad_pp r0.xzw, r0.x, r2.xyyz, c22.xyyz
    mad_pp r0.xyz, r0.y, c17, r0.xzww
    mad_pp r0.xyz, r3.x, c19, r0
    mad_pp r0.xyz, r3.y, c21, r0
    mad_pp r0.xyz, r6.y, c1, r0
    add_pp r0.w, -r6.w, c2.x
    mad_pp r2.xyz, r6.w, c0, r0.w
    mul_pp r0.w, r5.w, r6.z
    mul_pp r0.xyz, r0, r2
    texld_pp r2, t1, s0
    texld_pp r3, t2, s1
    mad_pp r2.xyz, r3, c2.z, r2
    mul_pp r3.w, r2.w, c0.w
    add_pp r2.xyz, r2, c2.w
    mad_pp r0.xyz, r2, r0, r1
    mul_pp r1.xyz, r5, r4
    mad_pp r3.xyz, r1, r0.w, r0
    mov_pp oC0, r3

// approximately 112 instruction slots used (4 texture, 108 arithmetic)
		};
#endif
	}
}