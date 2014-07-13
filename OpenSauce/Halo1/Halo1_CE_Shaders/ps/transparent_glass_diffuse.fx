#define USE_ASM

sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);
sampler TexS2 : register(s[2]);

// PASS-ish, had issues, think its fixed but need to chack mor thoroughly
half4 PS_TransparentGlassDiffuseModel(half4 v0 : COLOR0, half2 Tex0 : TEXCOORD0, half2 Tex1 : TEXCOORD1) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 t0 = tex2D(TexS0, Tex0);
	half4 t1 = tex2D(TexS1, Tex1);

	r0.xyz = saturate(t0 * t1 * 2.0f);
	r0.w = saturate(t0.w * t1.w);
	r0.xyz = saturate(r0.xyz * v0.xyz);
	r0.w = saturate(r0.w * v0.w);
	
	return r0;
}

// UNKNOWN
half4 PS_TransparentGlassDiffuseEnvironment(half4 v0 : COLOR0, half2 Tex0 : TEXCOORD0, half2 Tex1 : TEXCOORD1, half2 Tex2 : TEXCOORD2) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 t0 = tex2D(TexS0, Tex0);
	half4 t1 = tex2D(TexS1, Tex1);
	half4 t2 = tex2D(TexS2, Tex2);
	
	r0.xyz = saturate((t0 * t1) * 2);
	r0.w = saturate(t0.w * t1.w);
	r1 = saturate(t2 + v0);
	r0.xyz = saturate(r0 * r1);
	r0.w = saturate(r0.w * v0.w);
	
	return r0;
}

Technique TransparentGlassDiffuseModel
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT PS_TransparentGlassDiffuseModel();
#else
		pixelshader = asm
		{
			ps_1_1
			tex t0
			tex t1
			mul_x2_sat r0.xyz, t0, t1
		  + mul_sat r0.w, t0, t1
			mul_sat r0.xyz, r0, v0
		  + mul_sat r0.w, r0.w, v0.w

		// approximately 4 instruction slots used (2 texture, 2 arithmetic)
		};
#endif
	}
}

Technique TransparentGlassDiffuseEnvironment
{
	Pass P0 
	{
#ifndef USE_ASM
		PixelShader = compile TGT PS_TransparentGlassDiffuseEnvironment();
#else
		pixelshader = asm
		{
			ps_1_1
			tex t0
			tex t1
			tex t2
			mul_x2_sat r0.xyz, t0, t1
		  + mul_sat r0.w, t0, t1
			add_sat r1, t2, v0
			mul_sat r0.xyz, r0, r1
		  + mul_sat r0.w, r0.w, v0.w

		// approximately 6 instruction slots used (3 texture, 3 arithmetic)
		};
#endif
	}
}