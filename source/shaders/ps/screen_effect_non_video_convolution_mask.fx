#include "include/util/interpolation.fx"

sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);
sampler TexS2 : register(s[2]);
sampler TexS3 : register(s[3]);

float4 c0 : register(c[0]);
float4 c1 : register(c[1]);
float4 c2 : register(c[2]);

// PASS
half4 P0(
    half2 Tex0  : TEXCOORD0,
    half2 Tex1  : TEXCOORD1,
    half2 Tex2  : TEXCOORD2,
    half2 Tex3  : TEXCOORD3
) : COLOR
{
	half4 t0, t1, t2, t3, r0, r1;
	
	t0 = tex2D(TexS0, Tex0);
	t1 = tex2D(TexS1, Tex1);
	t2 = tex2D(TexS2, Tex2);
	t3 = tex2D(TexS3, Tex3);

    r0.xyz = (t2.xyz - 0.5f) + (t1.xyz - 0.5f);
	r0.w = t0.z * c1.w;
    r1.xyz = r0.xyz * c0.w;
	r1.w = t0.z * c2.w;
	r0.xyz = ((t3.xyz - 0.5f) * c0.w) + r1.xyz;
    r0.xyz += 0.5;
    r0.xyz = Lerp(t0.w, r0, t1);
	r0.w = t0.z;
	
	return r0;
}

Technique ScreenEffectNonVideoConvolutionMask
{
	Pass P0
	{
		PixelShader = compile TGT P0();
	}
}