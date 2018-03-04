#define USE_ASM

sampler TexSampler0 : register(s[0]);
sampler TexSampler1 : register(s[1]);
sampler TexSampler2 : register(s[2]);
sampler TexSampler3 : register(s[3]);

//TODO: check these constants are correct
float4 c_view_perpendicular_color : register(c[0]);
float4 c_view_parallel_color : register(c[1]);
float4 c_group_intensity : register(c[2]);

void CalculateEyeDirAndNormal(half2 Tex0, half4 Tex1, half4 Tex2, half4 Tex3, out half3 E, out half3 N)
{
	E = normalize(half3(Tex1.w, Tex2.w, Tex3.w));

	half3 bump_color = 2*tex2D(TexSampler0, Tex0)-1;
	N = normalize(half3(dot(Tex1, bump_color), dot(Tex2, bump_color), dot(Tex3, bump_color)));
}

half4 PS_GlassReflectionBumped(
    half4 Diff : COLOR0,
    half2 Tex0  : TEXCOORD0,
    half4 Tex1  : TEXCOORD1,
    half4 Tex2  : TEXCOORD2,
    half4 Tex3  : TEXCOORD3) : COLOR
{
	half3 E, N;
	CalculateEyeDirAndNormal(Tex0, Tex1, Tex2, Tex3, E, N);
	half3 R = normalize(2*dot(N, E)*N - E);

	half3 reflection_color = texCUBE( TexSampler3, R );
	half3 specular_color = pow(reflection_color, 8 );

	half3 diffuse_reflection = texCUBE( TexSampler2, N );
	diffuse_reflection = pow(diffuse_reflection, 2);

	half specular_mask = Diff.a * c_group_intensity;

	half attenuation = lerp( c_view_parallel_color.a, c_view_perpendicular_color.a, diffuse_reflection.b );

	half3 tint_color = lerp( c_view_parallel_color, c_view_perpendicular_color, diffuse_reflection.b );

	half3 tinted_reflection = lerp( specular_color, reflection_color, tint_color );

	half3 final_color = tinted_reflection * ( attenuation * specular_mask );

	return half4(final_color, 1.0);
}

Technique TransparentGlassReflectionBumped
{
	Pass P0
	{
#ifndef USE_ASM
	#ifdef pc   
		ColorOp[0]	= Disable;
		AlphaOp[0]	= Disable;
	#endif		
		PixelShader	= compile TGT PS_GlassReflectionBumped();
#else
		pixelshader = asm
		{
			ps_2_0
			def c4, 0, 1, 0, 8
			def c5, 2, -1, 0, 0
			dcl v0
			dcl_pp t0.xy
			dcl_pp t1
			dcl_pp t2
			dcl_pp t3
			dcl_2d s0
			dcl_2d s1
			dcl_volume s2
			texld r0, t0, s0
			mad_pp r2.xyz, c5.x, r0, c5.y
			dp3_pp r9.x, t1, r2
			dp3_pp r9.y, t2, r2
			dp3_pp r9.z, t3, r2
			mov_pp r4.x, t1.w
			mov_pp r4.y, t2.w
			mov_pp r4.z, t3.w
			dp3_pp r9.w, r9, r4
			add_pp r9.w, r9.w, r9.w
			mad_pp r1.xyz, r9, r9.w, -r4
			dp3 r1.w, r1, r1
			rsq r1.w, r1.w
			mul_pp r8.xyz, r1, r1.w
			texld_pp r3, r8, s2
			texld_pp r10, t2, s1
			log_pp r5.x, r3.x
			log_pp r5.y, r3.y
			log_pp r5.z, r3.z
			mul r0.xyz, r5, c4.w
			exp_pp r7.x, r0.x
			exp_pp r7.y, r0.y
			exp_pp r7.z, r0.z
			add_pp r4.xyz, r3, -r7
			mad_pp r9.xyz, c5.x, r10, c5.y
			dp3_pp r4.w, r9, c0
			log_pp r4.w, r4.w
			add_pp r4.w, r4.w, r4.w
			exp_pp r7.w, r4.w
			mov_pp r6.xyz, -c2
			add_pp r1.xyz, r6, c1
			mad_pp r3.xyz, r1, r7.w, c2
			mad_pp r7.xyz, r4, r3, r7
			mov_pp r5.w, -c2.w
			add_pp r0.w, r5.w, c1.w
			mad_pp r7.w, r0.w, r7.w, c2.w
			mul_pp r2.w, v0.w, c3.x
			mul_pp r7.w, r7.w, r2.w
			mul_pp r11.xyz, r7, r7.w
			mov_pp r11.w, c4.y
			mov_pp oC0, r11

		// approximately 41 instruction slots used (3 texture, 38 arithmetic)
		};
#endif
	}
}
