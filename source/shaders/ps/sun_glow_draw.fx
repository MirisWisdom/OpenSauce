sampler TexS0 : register(s[0]);

// PASS
half4 P0(half4 v0 : COLOR0, half2 Tex0 : TEXCOORD0) : COLOR
{
   half4 r0, r1, r2, r3, r4, r5, r6;

   half4 c0 = half4(0.6901960784313725,0.6901960784313725,0.5019607843137254,0.0);
   half4 c1 = half4(1.0,1.0,1.0,0.0);
   
   half4 t0 = tex2D(TexS0, Tex0);
   
   r0.a = saturate(c1.b - (2 * (t0.b - 0.5)));
   r0.rgb = (r0.a * c0.rgb) + (1 - r0.a);
   r0.rgb = r0.rgb * t0.rgb;
   r0.a = v0.a;
   
   return r0;
}

Technique SunGlowDraw
{
	Pass P0
	{
		PixelShader = compile TGT P0();
	}
}