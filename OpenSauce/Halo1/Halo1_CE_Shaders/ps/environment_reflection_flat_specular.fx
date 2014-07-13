#include "include/util/interpolation.fx"

sampler TexSampler0 : register(s[0]);
sampler TexSampler1 : register(s[1]);
sampler TexSampler2 : register(s[2]);
sampler TexSampler3 : register(s[3]);

float4 c_view_perpendicular_color : register(c[1]);
float4 c_view_parallel_color : register(c[2]);

half4 PS_EnvironmentReflectionFlatSpecular(
    half4 Diff : COLOR0,
    half2 Tex0  : TEXCOORD0,
    half4 Tex1  : TEXCOORD1,
    half4 Tex2  : TEXCOORD2,
    half4 Tex3  : TEXCOORD3) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5;
	
	r0 = tex2D(TexSampler0, Tex0);
	r1 = texCUBE(TexSampler1, Tex1);
	r2 = texCUBE(TexSampler2, Tex2);
	r3 = texCUBE(TexSampler3, Tex3);
	
	// combiner 0
	r4 = saturate(((2 * r1) - 1) * ((2 * r0) - 1));
	r5	= r3 * r3;
	
	// combiner 1
	r4 = saturate(r4 * r4);
	r5 = r5 * r5;
	
	// combiner 2
	r5 = r5 * r5;
	
	// combiner 3
	r4	= saturate(Lerp(r4.z, c_view_parallel_color, c_view_perpendicular_color));
	r4.rgb = Lerp(r4, r3, r5);
	
	// combiner 4
	r0.rgb = r4.w * r4;
	r0.w = 0;
	
	return r0;
}

Technique EnvironmentReflectionFlatSpecular
{
    Pass P0
	{
		PixelShader = compile TGT PS_EnvironmentReflectionFlatSpecular();
 	}
}
