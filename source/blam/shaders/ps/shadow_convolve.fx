sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);
sampler TexS2 : register(s[2]);
sampler TexS3 : register(s[3]);

// PASS
half4 P0(half2 Tex0 : TEXCOORD0, half2 Tex1 : TEXCOORD1, half2 Tex2 : TEXCOORD2, half2 Tex3 : TEXCOORD3) : COLOR
{
   half4 r0, r1, r2, r3, r4, r5, r6;

   half4 c7 = half4(0.5,0.5,0.5,0.5);

   half4 t0 = tex2D(TexS0, Tex0);
   half4 t1 = tex2D(TexS1, Tex1);
   half4 t2 = tex2D(TexS2, Tex2);
   half4 t3 = tex2D(TexS3, Tex3);

   r0.rgb = t2.rgb * c7;
   r0.a = t0.b * c7;
   r0.rgb = ((t3.rgb * c7.rgb) + r0) / 2;
   r0.a = ((t1.b * c7) + r0.a) / 2;
   r0 = r0 + r0.a;
   
   return r0;
}

Technique ShadowConvolve
{
	Pass P0
	{
		PixelShader = compile TGT P0();
	}
}
