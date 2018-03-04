sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);

float4 c0 : register(c[0]);

// PASS
half4 PS_PassThrough_1(
	half2 Tex0  : TEXCOORD0
) : COLOR
{
	half4 t0, r0;
	
	t0 = tex2D(TexS0, Tex0);
	
	r0.xyz = t0.xyz;
	r0.w = t0.z;
	
	return r0;
}

// PASS
half4 PS_PassThrough_2(
    half2 Tex0  : TEXCOORD0
) : COLOR
{
	half4 t0, r0;
	
	t0 = tex2D(TexS0, Tex0);
	
	r0.xyz = t0.xyz;
	r0.w = 0;
	
	return r0;
}

// PASS
half4 PS_PassThrough_non_convolved(
    half2 Tex0  : TEXCOORD0,
    half2 Tex1  : TEXCOORD1
) : COLOR
{
	half4 t0, t1, r0;
	
	t0 = tex2D(TexS0, Tex0);
	t1 = tex2D(TexS1, Tex1);
	
	r0.xyz = t1.xyz;
	r0.w = t0.z;
	
	return r0;
}

Technique PassThrough_1
{
	Pass P0
	{
		PixelShader = compile TGT PS_PassThrough_1();
	}
}

Technique PassThrough_2
{
	Pass P0
	{
		PixelShader = compile TGT PS_PassThrough_2();
	}
}

Technique PassThrough_non_convolved
{
	Pass P0
	{
		PixelShader = compile TGT PS_PassThrough_non_convolved();
	}
}