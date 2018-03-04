sampler TexS0 : register(s[0]);

half4 P0(half2 Tex0 : TEXCOORD0) : COLOR
{
	half4 r0;

	half4 t0 = tex2D(TexS0, Tex0);

	r0 = t0;
	return r0;
}

Technique TransparentGeneric
{
    Pass P0
	{
		PixelShader = compile TGT P0();
 	}
}