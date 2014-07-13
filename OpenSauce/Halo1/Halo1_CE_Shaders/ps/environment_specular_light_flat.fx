#include "include/util/interpolation.fx"

#define USE_ASM

sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);
sampler TexS2 : register(s[2]);
sampler TexS3 : register(s[3]);

float4 c_view_perpendicular_color : register(c[1]);
float4 c_view_parallel_color : register(c[2]);
float4 c_multiplier : register(c[3]);

//FAIL : r5 * c_multiplier.z in the asm version isn't coming out the same in the HLSL version even though r5 is the same
half4 P0(half4 v0 : COLOR0, half4 v1 : COLOR1, half2 Tex0 : TEXCOORD0, half3 Tex1 : TEXCOORD1, half3 Tex2 : TEXCOORD2, half3 Tex3 : TEXCOORD3) : COLOR
{
	float4 r0, r1, r2, r3, r4, r5, r6;


	half4 c0 = half4(0,0,1,0);

	r1 = texCUBE(TexS1, Tex1);
	r2 = texCUBE(TexS2, Tex2);
	r3 = texCUBE(TexS3, Tex3);

	r1.a = saturate((2 * (r3.b-0.5)) * 8);
	r0.rgb = saturate(dot(c0.rgb, 2 * (r2-0.5).rgb));
	return r0;
	r2.a = saturate((2 * (r2.b-0.5)) * 8);
	r4.rgb = saturate(((c0.rgb) * (r0.rgb) + (-r2+0.5)) * 2);
	r4.rgb = saturate(dot(r4.rgb, 2 * (r3-0.5).rgb));
	r1.rgb = (c0.rgb) * (r1);
	r0.rgb = Lerp(r0.b, c_view_perpendicular_color.rgb, c_view_parallel_color.rgb);
	r2.a = (r1.a) * (r2.a);
	r2.rgb = (r2.a) * (c0);
	r1 = texCUBE(TexS1, Tex1);
	r4.a = (r4.b) * (r4.b);
	r0.rgb = (r0.rgb) * (r1);
	r4.a = (r4.a) * (r4.a);
	r0.rgb = (r0.rgb) * (c_multiplier.z);
	r4.a = (r4.a) * (r4.a);
	r0.rgb = (r0.rgb) * (r2);
	r4 = saturate(dot(r4.rgb, r4.a));
	r0.rgb = (r0.rgb) * (r4.a);
	r0.rgb = (r0.rgb) * (1-v1.a);
	r0.a = r4.b;


	return r0;
}

Technique EnvironmentSpecularLightFlat
{
	Pass P0
	{
#ifndef USE_ASM
		PixelShader = compile TGT P0();
#else
		pixelshader = asm
		{
    ps_1_4
    def c0, 0, 0, 1, 0
    texld r1, t1
    texld r2, t2
    texld r3, t3
    mov_x8_sat r1.w, r3_bx2.z
  + dp3_sat r0.xyz, c0, r2_bx2
    mov_x8_sat r2.w, r2_bx2.z
  + mad_x2_sat r4.xyz, c0, r0, -r2_bias
    dp3_sat r4.xyz, r4, r3_bx2
    mul r1.xyz, c0, r1
    lrp r0.xyz, r0.z, c1, c2
  + mul r2.w, r1.w, r2.w
    mul r2.xyz, r2.w, c0
    phase
    texld r1, t1
    mul r4.w, r4.z, r4.z
  + mul r0.xyz, r0, r1
    mul r4.w, r4.w, r4.w
  + mul r0.xyz, r0, c3.z
    mul r4.w, r4.w, r4.w
  + mul r0.xyz, r0, r2
    dp3_sat r4, r4, r4.w
    mul r0.xyz, r0, r4.w
    mul r0.xyz, r0, 1-v1.w
  + mov r0.w, r4.z

// approximately 16 instruction slots used (4 texture, 12 arithmetic)
		};
#endif
	}
}