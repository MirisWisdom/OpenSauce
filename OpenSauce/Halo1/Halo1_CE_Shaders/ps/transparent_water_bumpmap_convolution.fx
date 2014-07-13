#include "include/util/interpolation.fx"

#define USE_ASM

float4 c0 : register(c0);
float4 c1 : register(c1);
float4 c2 : register(c2);
float4 c3 : register(c3);
   
sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);
sampler TexS2 : register(s[2]);
sampler TexS3 : register(s[3]);

// PASS
half4 P0(half2 Tex0 : TEXCOORD0, half2 Tex1 : TEXCOORD1, half2 Tex2 : TEXCOORD2, half2 Tex3 : TEXCOORD3) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 t0 = 2.0f * (tex2D(TexS0, Tex0) - 0.5f);
	half4 t1 = 2.0f * (tex2D(TexS1, Tex1) - 0.5f);
	half4 t2 = 2.0f * (tex2D(TexS2, Tex2) - 0.5f);
	half4 t3 = 2.0f * (tex2D(TexS3, Tex3) - 0.5f);

	r0 = Lerp(c0.w, t1, t0);
	r1 = Lerp(c1.w, t3, t2);
	r0 = Lerp(c2.w, r1, r0);
	r0 = (r0 * 0.5f) + 0.5f;
	r0 = Lerp(c3.w, c3, r0);
	
	return r0;
}

Technique TransparentWaterBumpmapConvolution
{
    Pass P0
	{
#ifndef USE_ASM
 		PixelShader = compile TGT P0();
#else
		pixelshader = asm
		{
			ps_1_1
			def c7, 0.5, 0.5, 0.5, 0.5
			tex t0
			tex t1
			tex t2
			tex t3
			lrp r0, c0.w, t1_bx2, t0_bx2
			lrp r1, c1.w, t3_bx2, t2_bx2
			lrp r0, c2.w, r1, r0
			mad r0, r0, c7, c7
			lrp r0, c3.w, c3, r0

		// approximately 9 instruction slots used (4 texture, 5 arithmetic)
		};
#endif
 	}
}