#include "include/util/interpolation.fx"

#define USE_ASM

sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);
sampler TexS2 : register(s[2]);
sampler TexS3 : register(s[3]);

float4 c_specular_brightness : register(c[0]);
float4 c_view_perpendicular_color : register(c[1]);
float4 c_view_parallel_color : register(c[2]);
float4 c_multiplier : register(c[3]);

//FAIL : r5 * c_multiplier.z in the asm version isn't coming out the same in the HLSL version even though r5 is the same
half4 P0(half4 v0 : COLOR0, half4 v1 : COLOR1, half2 Tex0 : TEXCOORD0, half3 Tex1 : TEXCOORD1, half3 Tex2 : TEXCOORD2, half3 Tex3 : TEXCOORD3) : COLOR
{
	float4 r0, r1, r2, r2_bias, r3, r4, r5, r6;

	r0 = tex2D(TexS0, Tex0);
	r1 = tex3D(TexS1, Tex1);
	r2 = texCUBE(TexS2, Tex2);
	r3 = texCUBE(TexS3, Tex3);
	
	r0.xyz = (r0.xyz - 0.5f) * 2;
	r2_bias.xyz = r2.xyz - 0.5f;
	r2.xyz = (r2.xyz - 0.5f) * 2;
	r3.xyz = (r3.xyz - 0.5f) * 2;

	r1.w = saturate(r3.z * 8);
	r5.xyz = saturate(dot(r0.xyz, r2.xyz));
	r2.w = saturate(r2.z * 8);
	r4.xyz = saturate(((r0.xyz * r5.xyz) - r2_bias.xyz) * 2);	
	r4.xyz = saturate(dot(r4.xyz, r3.xyz));
	r5.xyz = Lerp(r5.z, c_view_perpendicular_color.xyz, c_view_parallel_color.xyz);
	r2.w = r1.w * r2.w;
	r2.xyz = r2.w * c_specular_brightness.xyz;
	r0.xyz = dot(r1.xyz, c_specular_brightness.xyz);
		
	r4.w = r4.z * r4.z;
	r5.xyz = r5.xyz * r1.xyz;
	r4.w = r4.w * r4.w;	
	r5.xyz = saturate(r5 * c_multiplier.z);
	
	r4.xyz = r0.xyz * c_multiplier.z;	
	r4.w = r4.w * r4.w;
	r5.xyz = r5.xyz * r2.xyz;
	r4 = saturate(dot(r4, r4.w));
	r5.xyz = r5.xyz * r4.w;	
	r0.xyz = r5.xyz * (1.0f - v1.w);
	r0.w = r4.z;

	return r0;	
}

Technique EnvironmentSpecularLightmapBumped
{
	Pass P0
	{
	#ifndef USE_ASM
		PixelShader = compile TGT P0();
	#else		
		pixelshader = asm
		{
    ps_1_4
    texld r0, t0
    texld r1, t1
    texld r2, t2
    texld r3, t3
    mov_x8_sat r1.w, r3_bx2.z
  + dp3_sat r5.xyz, r0_bx2, r2_bx2
    mov_x8_sat r2.w, r2_bx2.z
  + mad_x2_sat r4.xyz, r0_bx2, r5, -r2_bias
    dp3_sat r4.xyz, r4, r3_bx2
    lrp r5.xyz, r5.z, c1, c2
  + mul r2.w, r1.w, r2.w
    mul r2.xyz, r2.w, c0
    dp3 r0.xyz, r1, c0
    phase
    texld r1, t1
    mul r4.w, r4.z, r4.z
  + mul r5.xyz, r5, r1
    mul r4.w, r4.w, r4.w
  + mul r5.xyz, r5, c3.z
    mul r4.xyz, r0, c3.z
    mul r4.w, r4.w, r4.w
  + mul r5.xyz, r5, r2
    dp3_sat r4, r4, r4.w
    mul r5.xyz, r5, r4.w
    mul r0.xyz, r5, 1-v1.w
  + mov r0.w, r4.z

// approximately 18 instruction slots used (5 texture, 13 arithmetic)
		};
	#endif
	}
}
