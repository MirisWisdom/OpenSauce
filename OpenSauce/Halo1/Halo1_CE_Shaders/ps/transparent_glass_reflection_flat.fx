#include "include/util/interpolation.fx"

#define USE_ASM

float4 c_eye_forward : register(c[0]);
float4 c_view_perpendicular_color : register(c[1]);
float4 c_view_parallel_color : register(c[2]);

sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);
sampler TexS3 : register(s[3]);

// PASS
half4 PS_GlassReflectionFlat(half4 v0 : COLOR0, half2 Tex0 : TEXCOORD0, half3 Tex1 : TEXCOORD1, half3 Tex2 : TEXCOORD2, half3 Tex3 : TEXCOORD3) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 t0 = tex2D(TexS0, Tex0);
	half4 t1 = texCUBE(TexS1, Tex1);
	half4 t3 = texCUBE(TexS3, Tex3);

	r0 = dot(2 * (t1.rgb - 0.5), 2 * (t0.rgb - 0.5));
	r1.xyz = saturate(t3 * t3);
	r1.w = r0.z;
	r1 = saturate(r1 * r1);
	r1 = saturate(r1 * r1);
	r0 = Lerp(r1.w, c_view_perpendicular_color, c_view_parallel_color);
	r0.xyz = Lerp(r0, t3, r1);
	r1.xyz = saturate(r0.w * v0.w);
	r0.xyz = saturate(r0 * r1);
	
	return r0;
}

// PASS
half4 PS_GlassReflectionFlatSpecularMask(half4 v0 : COLOR0, half2 Tex0 : TEXCOORD0, half3 Tex1 : TEXCOORD1, half3 Tex2 : TEXCOORD2, half3 Tex3 : TEXCOORD3) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 t0 = tex2D(TexS0, Tex0);
	half4 t1 = texCUBE(TexS1, Tex1);
	half4 t3 = texCUBE(TexS3, Tex3);

	r0 = dot(2 * (t1.rgb - 0.5), 2 * (t0.rgb - 0.5));
	r1.xyz = saturate(t3 * t3);
	r1.w = r0.z;
	r1 = saturate(r1 * r1);
	r1 = saturate(r1 * r1);
	r0 = Lerp(r1.w, c_view_perpendicular_color, c_view_parallel_color);
	r0.xyz = Lerp(r0, t3, r1);
	r1.xyz = saturate(r0.w * t0);
	r0.xyz = saturate(r0 * r1);
	
	return r0;
}

Technique GlassReflectionFlat
{
    Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT PS_GlassReflectionFlat();
#else
		pixelshader = asm
		{
			ps_1_1
			tex t0
			tex t1
			tex t3
			dp3_sat r0, t1_bx2, t0_bx2
			mul_sat r1.xyz, t3, t3
		  + mov r1.w, r0.z
			mul_sat r1, r1, r1
			mul_sat r1, r1, r1
			lrp r0, r1.w, c1, c2
			lrp r0.xyz, r0, t3, r1
			mul_sat r1.xyz, r0.w, v0.w
			mul_sat r0.xyz, r0, r1

		// approximately 11 instruction slots used (3 texture, 8 arithmetic)
		};
#endif
 	}
}

Technique GlassReflectionFlatSpecularMask
{
    Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT PS_GlassReflectionFlatSpecularMask();
#else
		pixelshader = asm
		{
			ps_1_1
			tex t0
			tex t1
			tex t3
			dp3_sat r0, t1_bx2, t0_bx2
			mul_sat r1.xyz, t3, t3
		  + mov r1.w, r0.z
			mul_sat r1, r1, r1
			mul_sat r1, r1, r1
			lrp r0, r1.w, c1, c2
			lrp r0.xyz, r0, t3, r1
			mul_sat r1.xyz, r0.w, t0
			mul_sat r0.xyz, r0, r1

		// approximately 11 instruction slots used (3 texture, 8 arithmetic)
		};
#endif
 	}
}