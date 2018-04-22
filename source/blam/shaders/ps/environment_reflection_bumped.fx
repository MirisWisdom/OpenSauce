#include "include/util/reflection.fx"
#include "include/util/bump_map.fx"
#include "include/util/interpolation.fx"

sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);
sampler TexS2 : register(s[2]);
sampler TexS3 : register(s[3]);

float4 c_eye_forward : register(c[0]);
float4 c_view_perpendicular_color : register(c[1]);
float4 c_view_parallel_color : register(c[2]);

half4 P0(half2 Tex0 : TEXCOORD0, half4 Tex1 : TEXCOORD1, half4 Tex2 : TEXCOORD2, half4 Tex3 : TEXCOORD3) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;
	
	float3 reflect_vec = GetBumpReflect(TexS0, Tex0, Tex1, Tex2, Tex3);
	
	half4 t3 = texCUBE(TexS3, reflect_vec);
	r1 = t3 * t3;
	r1 = (r1 * r1) * 0.5f;
	r1.xyz = r1 * r1;
	r1.w = saturate(c_eye_forward.w);
	r0 = saturate(Lerp(r1.w, c_view_perpendicular_color, c_view_parallel_color));
	r0.xyz = Lerp(r0.w, t3, r1);
	
	return r0;
};

Technique EnvironmentReflectionBumped
{
   Pass P0
   {
      PixelShader = compile TGT P0();
   }
}
