#define USE_ASM

float4 c_tint_and_intensity : register(c[0]);

sampler TexS0 : register(s[0]);

// PASS
float4 P0(float2 Tex0 : TEXCOORD0, float4 v0 : COLOR0) : COLOR
{
	half4 r0 = 0;
	
	float4 t0 = tex2D(TexS0, Tex0);

	r0.xyz = t0 * c_tint_and_intensity;
	r0.xyz = (r0.xyz * v0.w) + (1 - v0.w);
	r0.w = v0.w;

	return r0;
}

Technique TransparentGlassTint
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
			mul r0.xyz, t0, c0
			mad r0.xyz, r0, v0.w, 1-v0.w
		  + mov r0.w, v0.w

		// approximately 3 instruction slots used (1 texture, 2 arithmetic)
		};
#endif
	}
}
