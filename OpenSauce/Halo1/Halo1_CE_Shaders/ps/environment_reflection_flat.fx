#include "include/util/interpolation.fx"

sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);
sampler TexS2 : register(s[2]);
sampler TexS3 : register(s[3]);

float4 c_eye_forward : register(c[0]);
float4 c_view_perpendicular_color : register(c[1]);
float4 c_view_parallel_color : register(c[2]);

half4 P0(half2 Tex0 : TEXCOORD0, half3 Tex1 : TEXCOORD1, half3 Tex2 : TEXCOORD2, half3 Tex3 : TEXCOORD3) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 t0 = tex2D(TexS0, Tex0);
	half4 t1 = texCUBE(TexS1, Tex1);// dot product (type==bumped), normal vector (type!=bumped)
	half4 t2 = texCUBE(TexS2, Tex2);// diffuse reflection cube map
	half4 t3 = texCUBE(TexS3, Tex3);// specular reflection cube map

	r0 = saturate(dot(2 * (t0 - 0.5).rgb, 2 * (t1 - 0.5).rgb));
	r1.rgb = t3.rgb * t3;
	r0.rgb = r0.rgb * r0;
	r1.rgb = r1.rgb * r1;
	r1.a = saturate(r0.b);
	r0 = saturate(Lerp(r1.a, c_view_perpendicular_color, c_view_parallel_color));
	r0.rgb = Lerp(r0, t3, r1) * 0.5f;
	
	return r0;
};

Technique EnvironmentReflectionFlat
{
    Pass P0
	{
		PixelShader = compile TGT P0();
 	}
}
