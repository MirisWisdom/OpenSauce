float4 c_lightmap_brightness : register(c[1]);

sampler TexS0 : register(s[0]);

half4 P0(half2 Tex0 : TEXCOORD0) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 c0 = half4(0.501960814, 0.690196097, 0.313725501, 0.0);
	
	half4 t0 = tex2D(TexS0, Tex0);
	r0 = saturate(dot(t0, c0));
	r0.w = (1.0f - r0.z) * c_lightmap_brightness.w + r0.z;
	
	return r0;
}

Technique EnvironmentReflectionLightmapMask
{
    Pass P0
	{
		PixelShader = compile TGT P0();
 	}
}
