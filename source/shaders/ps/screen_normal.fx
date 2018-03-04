float4 c0 : register(c0);

sampler TexS0 : register(s[0]);

// PASS
half4 P0(half4 v0 : COLOR0, half2 Tex0 : TEXCOORD0) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 t0 = tex2D(TexS0, Tex0);

	t0 = t0 * c0;
	r0.rgb = t0.rgb * v0;
	r0.a = t0.a;
	
	return r0;
}

Technique ScreenNormal
{
    Pass P0
	{
#ifdef pc
		ColorOp[0]	= Disable;
		AlphaOp[0]	= Disable;
#endif
 		PixelShader = compile TGT P0();
 	}
}
