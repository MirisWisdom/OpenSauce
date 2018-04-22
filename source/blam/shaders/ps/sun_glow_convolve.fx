float4 c0 : register(c[0]);

sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);
sampler TexS2 : register(s[2]);
sampler TexS3 : register(s[3]);

// PASS
half4 PS_SunGlowCopySource(half2 Tex0 : TEXCOORD0, half2 Tex1 : TEXCOORD1) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 c0 = half4(0.0,0.0,0.0,0.75);
	half4 t0 = tex2D(TexS0, Tex0);
	half4 t1 = tex2D(TexS1, Tex1);

	r0.a = t0.a;
	r0.a = lerp(c0.b, r0.a, step(0.5, r0.a));
	r0.rgb = r0.a;
	r0.a = (t1.a * c0.a) * 4;
	
	return r0;
}

// PASS
half4 PS_SunGlowConvolve(half2 Tex0 : TEXCOORD0, half2 Tex1 : TEXCOORD1, half2 Tex2 : TEXCOORD2, half2 Tex3 : TEXCOORD3) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 t0 = tex2D(TexS0, Tex0);
	half4 t1 = tex2D(TexS1, Tex1);
	half4 t2 = tex2D(TexS2, Tex2);
	half4 t3 = tex2D(TexS3, Tex3);

	r0.rgb = (t2.rgb + t3) / 2;
	r0.a = (t0.b + t1.b) / 2;
	r0 = r0 * c0.a;
	r0 = r0 + r0.a;
	
	return r0;
}

Technique SunGlowCopySource
{
	// copy source
	Pass P0  
	{
		PixelShader = compile TGT PS_SunGlowCopySource();
	}
}

Technique SunGlowConvolve
{
	// convolve
	Pass P0  
	{
		PixelShader = compile TGT PS_SunGlowConvolve();
	}
}