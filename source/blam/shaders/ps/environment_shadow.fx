float4 c0 : register(c[0]);

sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);

// PASS
half4 P0(half4 v0 : COLOR0, half2 Tex0 : TEXCOORD0, half2 Tex1 : TEXCOORD1) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	// c0 = 1-local_shadow_color
	half4 c1 = half4(1.0,1.0,1.0,0.0);
	half4 c2 = half4(1.0,0.0,-1.0,0.0);

	half4 t0 = tex2D(TexS0, Tex0);// shadow
	half4 t1 = tex2D(TexS1, Tex1);// fade

	r0.rgb = saturate((1 - t1.rgb) * v0.a);
	r1.rgb = saturate(t0.rgb * c0.rgb);
	r0.rgb = saturate((r0.rgb * (1 - r1.rgb)) + (1 - r0));
	r1.rgb = saturate(dot(1 - r0.rgb, c1.rgb));
	r0.rgb = c1.rgb - r0.rgb;
	r0.a = r1.b;
	
	return r0;
}

Technique EnvironmentShadow
{
    Pass P0
	{
		PixelShader = compile TGT P0();
 	}
}
