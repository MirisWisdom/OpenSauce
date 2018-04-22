sampler TexS0 : register(s0);

// PASS
half4 P0(half4 v0 : COLOR0, half4 v1 : COLOR1, half2 Tex0 : TEXCOORD0) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 t0 = tex2D(TexS0, Tex0);

	r0 = (t0) * (t0);
	r1 = (r0) * (r0);
	t0 = (v0) * (t0);
	r0 = (r0) * (r1);
	r1 = (1-v0) * (v1.a);
	r0.rgb = (r0.rgb * v1.rgb) + t0;
	r0.a = t0.a;
	
	return r0;
};

Technique WidgetSpritet
{
	Pass P0
	{
		ColorOp[0]			= Disable;
		AlphaOp[0]			= Disable;
		
		PixelShader = compile TGT P0();
	} 
}