float4 c_effect_intensity : register(c[0]);

sampler TexS0 : register(s0);
sampler TexS2 : register(s2);

#define USE_ASM

// PASS
half4 PS_TintEdgeDensity(half4 v0 : COLOR0, half3 Tex0 : TEXCOORD0, half3 Tex1 : TEXCOORD1, half3 Tex2 : TEXCOORD2) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 t0 = 2 * (texCUBE(TexS0, Tex0) - 0.5);
	half2 uv1;
	uv1.x = dot(t0.rgb, Tex1.xyz);
	uv1.y = dot(t0.rgb, Tex2.xyz);
	
	half4 t2 = tex2D(TexS2, uv1);
	
	r0.rgb = lerp(v0, 1.0f, t0.w);
	r0.rgb = lerp(r0, 1.0f, v0.w);
	r0.rgb = r0.rgb * t2;
	r0.a = c_effect_intensity.a;

	return r0;
};

// PASS
half4 PS_NoEdgeTint(half4 v0 : COLOR0, half3 Tex0 : TEXCOORD0, half3 Tex1 : TEXCOORD1, half3 Tex2 : TEXCOORD2) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 t0 = 2 * (texCUBE(TexS0, Tex0) - 0.5);
	half2 uv1;
	uv1.x = dot(t0.rgb, Tex1.xyz);
	uv1.y = dot(t0.rgb, Tex2.xyz);
	
	half4 t2 = tex2D(TexS2, uv1);

	r0.rgb = v0.a * v0.rgb + 1.0f;
	r0.rgb = r0.rgb * t2;
	r0.a = c_effect_intensity.a;

	return r0;
};

Technique TintEdgeDensity
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT PS_TintEdgeDensity();
#else
		pixelshader = asm
		{
			ps_1_1
			def c7, 1, 1, 1, 1
			tex t0
			texm3x2pad t1, t0_bx2
			texm3x2tex t2, t0_bx2
			lrp r0.xyz, t0.w, v0, c7
			lrp r0.xyz, v0.w, r0, c7
			mul r0.xyz, r0, t2
		  + mov r0.w, c0.w

		// approximately 6 instruction slots used (3 texture, 3 arithmetic)
		};
#endif
	}
}

Technique NoEdgeTint
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT PS_NoEdgeTint();
#else
		pixelshader = asm
		{
			ps_1_1
			def c7, 1, 1, 1, 1
			tex t0
			texm3x2pad t1, t0_bx2
			texm3x2tex t2, t0_bx2
			mad r0.xyz, v0.w, v0, c7
			mul r0.xyz, r0, t2
		  + mov r0.w, c0.w

		// approximately 5 instruction slots used (3 texture, 2 arithmetic)
		};
#endif
	}
}
