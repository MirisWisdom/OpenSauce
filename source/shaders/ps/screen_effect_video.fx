sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);
sampler TexS2 : register(s[2]);

float4 c0 : register(c[0]);
float4 c1 : register(c[1]);

// PASS
half4 P0(
    half2 Tex0  : TEXCOORD0,
    half2 Tex1  : TEXCOORD1,
    half2 Tex2  : TEXCOORD2
) : COLOR
{
	half4 t0, t1, t2, r0, r1;

	t0 = tex2D(TexS0, Tex0);
	t1 = tex2D(TexS1, Tex1);
	t2 = tex2D(TexS2, Tex2);

	r0 = (c0.w * t2) + c1.w;
	r0.xyz *= t1.xyz;
	r1 = t0 * t0;
	r1 *= r1;
	t0 *= c0;
	r0.xyz = (r1 * (1 - r0)) + t0;
	
	return r0;
}

Technique ScreenEffectVideo
{
	Pass P0
	{
		PixelShader = compile TGT P0();
	}
}