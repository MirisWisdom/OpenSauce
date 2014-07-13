#include "include/util/interpolation.fx"

sampler TexSampler0 : register(s[0]);
sampler TexSampler1 : register(s[1]);
sampler TexSampler2 : register(s[2]);
sampler TexSampler3 : register(s[3]);

float4 c_view_perpendicular_color : register(c[1]);
float4 c_view_parallel_color : register(c[2]);

half4 PS_EnvironmentReflectionMirrorFlat(
    half4 Diff : COLOR0,
    half4 Tex0  : TEXCOORD0,
    half4 Tex1  : TEXCOORD1,
    half4 Tex2  : TEXCOORD2,
    half4 Tex3  : TEXCOORD3) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5;
	
	r0 = tex2D(TexSampler0, Tex0);
	r1 = texCUBE(TexSampler1, Tex1);
	r2 = texCUBE(TexSampler2, Tex2);
	r3 = tex2D(TexSampler3, Tex3.xy / Tex3.w);
	
	// combiner 0
	r4 = saturate(dot(((2 * r1) - 1), ((2 * r0) - 1)));
	r5 = r3 * r3;
	
	// combiner 1
	r4 = saturate(r4 * r4);
	r5 = r5 * r5;
	
	// combiner 2
	r5 = r5 * r5;
	
	// combiner 3
	r4 = saturate(Lerp(r4.z, c_view_perpendicular_color, c_view_parallel_color));
	r4.rgb = Lerp(r4, r3, r5);
	
	// combiner 4
	r0.rgb = r4.w * r4;
	r0.w = 1;
	
	return r0;
}

Technique EnvironmentReflectionMirrorFlat
{
    Pass P0
	{
#ifdef pc   
		ColorOp[0]	= Disable;
		AlphaOp[0]	= Disable;
#endif		
		PixelShader = compile TGT PS_EnvironmentReflectionMirrorFlat();
 	}
}
