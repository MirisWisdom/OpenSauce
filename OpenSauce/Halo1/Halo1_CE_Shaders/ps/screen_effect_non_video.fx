sampler TexS0 : register(s[0]);

// PASS
half4 PS_ScreenEffectNonVideoPass0(half2 Tex0 : TEXCOORD0) : COLOR
{
	half4 r0;
	
	r0.xyz = tex2D(TexS0, Tex0).xyz;
	r0.w = r0.z;
	
	return r0;
}

// PASS
half4 PS_ScreenEffectNonVideoPass1(half2 Tex0 : TEXCOORD0) : COLOR
{
	half4 r0;
	
	r0.xyz = tex2D(TexS0, Tex0).xyz;
	r0.w = 0;
	
	return r0;
}

Technique ScreenEffectNonVideoPass0
{
	Pass P0
	{
		Pixelshader = compile TGT PS_ScreenEffectNonVideoPass0();
	}
}

Technique ScreenEffectNonVideoPass1
{
	Pass P0
	{
		Pixelshader = compile TGT PS_ScreenEffectNonVideoPass1();
	}
}