sampler TexS0 : register(s[0]);
sampler TexS2 : register(s[2]);
sampler TexS3 : register(s[3]);

// PASS
half4 P0(
    half2 Tex0  : TEXCOORD0,
    half2 Tex2  : TEXCOORD2,
    half2 Tex3  : TEXCOORD3
) : COLOR
{
	half4 t0, t2, t3, r0, r1;
	
	t0 = tex2D(TexS0, Tex0);
	t2 = tex2D(TexS2, Tex2);
	t3 = tex2D(TexS3, Tex3);

	t0 -= 0.5f;
	t2 -= 0.5f;
	t3 -= 0.5f;
	
    r0 = (t0 + t0) * 0.5f;
    r1 = (t2 + t3) * 0.5f;
	r0 = (r0 + r1) * 0.5f;
	r0 += 0.5;
	
	return r0;
}

Technique ScreenEffectNonVideoNoConvolutionMask
{
	Pass P0
	{
		PixelShader = compile TGT P0();
	}
}