#include "include/util/interpolation.fx"

#define USE_ASM

float4 c_perpendicular_tint_color : register(c0);
float4 c_parallel_tint_color : register(c1);

sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);

// PASS
half4 PS_AlphaModulatesReflection(half2 Tex0 : TEXCOORD0, half3 Tex1 : TEXCOORD1) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	// c0 - perpendicular tint color
	// c1 - parallel tint color

	half4 t0 = tex2D(TexS0, Tex0);
	half4 t1 = texCUBE(TexS1, Tex1);

	r0.w = Lerp(t1.z, c_perpendicular_tint_color, c_parallel_tint_color);
	r0 = r0.w * t0.w;
	
	return r0;
};

// PASS
half4 PS_ColorModulatesBackground(half2 Tex0 : TEXCOORD0, half3 Tex1 : TEXCOORD1, half fog : TEXCOORD2) : COLOR
{
	return tex2D(TexS0, Tex0);
}

Technique TransparentWaterOpacityAlphaModulated
{
    Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT PS_AlphaModulatesReflection();
#else
		pixelshader = asm
		{
			ps_1_1
			tex t0
			tex t1
			lrp r0.w, t1.z, c0, c1
			mul r0, r0.w, t0.w

		// approximately 4 instruction slots used (2 texture, 2 arithmetic)
		};
#endif
 	}
}

Technique TransparentWaterOpacityColorModulated
{
    Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT PS_ColorModulatesBackground();
#else
		pixelshader = asm
		{
			ps_1_1
			tex t0
			mov r0, t0

		// approximately 2 instruction slots used (1 texture, 1 arithmetic)
		};
#endif
 	}
}