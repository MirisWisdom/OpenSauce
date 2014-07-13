#include "include/util/interpolation.fx"

#define USE_ASM

sampler TexSampler0 : register(s[0]);
sampler TexSampler1 : register(s[1]);
sampler TexSampler2 : register(s[2]);
sampler TexSampler3 : register(s[3]);

float4 c_eye_forward : register(c[0]);
float4 c_view_perpendicular_color : register(c[1]);
float4 c_view_parallel_color : register(c[2]);

// PASS
half4 PS_GlassReflectionMirror(
    half4 Diff : COLOR0,
    half4 Tex2  : TEXCOORD2,
    half4 Tex3  : TEXCOORD3) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5;
	
	r2 = texCUBE(TexSampler2, Tex2);
	r3 = tex2D(TexSampler3, Tex3.xy / Tex3.w);

	r2.xyz = 2 * (r2.xyz - 0.5);

	r4 = dot(r2.xyz, c_eye_forward.xyz);
	r5 = r3 * r3;
	r4 = saturate(r4 * r4);
	r5 = r5 * r5;
	r5 = r5 * r5;
	r4 = saturate(Lerp(r4.z, c_view_perpendicular_color, c_view_parallel_color));
	r4.xyz = Lerp(r4, r5, r3);
	r4.xyz = r4 * r4.w;
	r0.xyz = r4 * Diff.w;
	r0.w = r4.w;

	return r0;
}

// PASS
half4 PS_GlassReflectionMirrorSpecularMask(
    half4 Diff : COLOR0,
    half4 Tex0  : TEXCOORD0,
    half4 Tex1  : TEXCOORD1,
    half4 Tex2  : TEXCOORD2,
    half4 Tex3  : TEXCOORD3) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5;
	
	r0 = tex2D(TexSampler0, Tex0);
	r1 = texCUBE(TexSampler1, Tex1);
	r2 = texCUBE(TexSampler2, Tex2);
	r3 = tex2D(TexSampler3, Tex3);
	
	r2.xyz = 2 * (r2.xyz - 0.5);
	
	r4 = dot(r2.xyz, c_eye_forward);
	r5 = r3 * r3;
	r4 = saturate(r4 * r4);
	r5 = r5 * r5;
	r5 = r5 * r5;
	r4 = saturate(Lerp(r4.z, c_view_perpendicular_color, c_view_parallel_color));
	r4.xyz = Lerp(r4, r5, r3);
	r4.xyz = r4 * r4.w;
	r0.xyz = r4 * r0;
	r0.w = r4.w;

	return r0;
}

Technique GlassReflectionMirror
{
	Pass P0
	{
#ifndef USE_ASM
	#ifdef pc   
		ColorOp[0]	= Disable;
		AlphaOp[0]	= Disable;
	#endif		
		PixelShader	= compile TGT PS_GlassReflectionMirror();
#else
		pixelshader = asm
		{
			ps_1_4
			def c3, 1, 1, 1, 1
			texld r2, t2
			texld r3, t3_dw.xyww
			dp3 r4, r2_bx2, c0
			mul r5, r3, r3
			mul_sat r4, r4, r4
			mul r5, r5, r5
			mul r5, r5, r5
			phase
			lrp_sat r4, r4.z, c1, c2
			lrp r4.xyz, r4, r5, r3
			mul r4.xyz, r4, r4.w
			mul r0.xyz, r4, v0.w
		  + mov r0.w, r4.w

		// approximately 11 instruction slots used (2 texture, 9 arithmetic)
		};
#endif
	}
}

Technique GlassReflectionMirrorSpecularMask
{
	Pass P0
	{
#ifndef USE_ASM
	#ifdef pc   
		ColorOp[0]	= Disable;
		AlphaOp[0]	= Disable;
	#endif		
		PixelShader	= compile TGT PS_GlassReflectionMirrorSpecularMask();
#else		
		pixelshader = asm
		{
			ps_1_4
			def c3, 1, 1, 1, 1
			texld r0, t0
			texld r1, t1
			texld r2, t2
			texld r3, t3_dw.xyww
			dp3 r4, r2_bx2, c0
			mul r5, r3, r3
			mul_sat r4, r4, r4
			mul r5, r5, r5
			mul r5, r5, r5
			phase
			lrp_sat r4, r4.z, c1, c2
			lrp r4.xyz, r4, r5, r3
			mul r4.xyz, r4, r4.w
			mul r0.xyz, r4, r0
		  + mov r0.w, r4.w

		// approximately 13 instruction slots used (4 texture, 9 arithmetic)
		};
#endif
	}
}