#include "include/util/interpolation.fx"

sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);
sampler TexS2 : register(s[2]);
sampler TexS3 : register(s[3]);

float4 c_specular_brightness : register(c[0]);
float4 c_view_perpendicular_color : register(c[1]);
float4 c_view_parallel_color : register(c[2]);
float4 c_multiplier : register(c[3]);

half4 P0(half2 Tex0 : TEXCOORD0, half2 Tex1 : TEXCOORD1, half3 Tex2 : TEXCOORD2, half3 Tex3 : TEXCOORD3) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 c4 = half4(0.5019607843137254,0.5019607843137254,0.5019607843137254,0.5019607843137254);

	r0 = tex2D(TexS0, Tex0);
	r1 = tex2D(TexS1, Tex1);
	r2 = texCUBE(TexS2, Tex2);
	r3 = texCUBE(TexS3, Tex3);

	r1.a = saturate((2 * (r3.b-0.5)) * 8);
	r5.rgb = saturate(dot(2 * (r0-0.5).rgb, 2 * (r2-0.5).rgb));
	r2.a = saturate((2 * (r2.b - 0.5)) * 8);
	r4.rgb = saturate(((2 * (r0 - 0.5).rgb) * (r5.rgb) + (-r2+0.5)) * 2);
	r4.rgb = saturate(dot(r4.rgb, 2 * (r3-0.5).rgb));
	r5.rgb = Lerp(r5.b, c_view_perpendicular_color.rgb, c_view_parallel_color.rgb);
	r2.a = (r1.a) * (r2.a);
	r2.rgb = (r2.a) * c_specular_brightness.rgb;
	r0.rgb = saturate(dot(r1.rgb, c_specular_brightness.rgb));
	r1 = tex2D(TexS1, Tex1);
	r4.a = (r4.b) * (r4.b);
	r5.rgb = (r5.rgb) * (r1);
	r4.a = (r4.a) * (r4.a);
	r5.rgb = (r5.rgb) * (c_multiplier.z);
	r4.a = (r4.a) * (r4.a);
	r5.rgb = (r5.rgb) * (r2);
	r4 = saturate(dot(r4.rgb, r4.a));
	r5.rgb = (r5.rgb) * (r4.a);
	r0.rgb = (r5.rgb) * (c4);
	r0.a = r4.b;


	return r0;
}

Technique EnvironmentSpecularLightmapBumped
{
    Pass P0
	{
		PixelShader = compile TGT P0();
 	}
}
