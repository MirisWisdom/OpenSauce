#include "include/util/reflection.fx"
#include "include/util/bump_map.fx"
#include "include/util/interpolation.fx"

#define USE_ASM

float4 c0 : register(c0);
   
sampler TexS0 : register(s[0]);
sampler TexS3 : register(s[3]);

// PASS
half4 P0(half2 Tex0 : TEXCOORD0, half4 Tex1 : TEXCOORD1, half4 Tex2 : TEXCOORD2, half4 Tex3 : TEXCOORD3, half fog : FOG) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	float3 reflect_vec = GetBumpReflect(TexS0, Tex0, Tex1, Tex2, Tex3);
		
	half4 t3 = texCUBE(TexS3, reflect_vec);
	r0 = t3 * t3;
	r0 *= r0;
	r0 *= r0;	
	r0 = Lerp(c0, t3, r0);

	return r0;
}

Technique TransparentWaterReflection
{
    Pass P0
	{
#ifndef USE_ASM
 		PixelShader = compile TGT P0();
#else
		pixelshader = asm
		{
			ps_1_1
			tex t0
			texm3x3pad t1, t0_bx2
			texm3x3pad t2, t0_bx2
			texm3x3vspec t3, t0_bx2
			mul r0, t3, t3
			mul r0, r0, r0
			mul r0, r0, r0
			lrp r0, c0, t3, r0

		// approximately 8 instruction slots used (4 texture, 4 arithmetic)
		};
#endif
 	}
}