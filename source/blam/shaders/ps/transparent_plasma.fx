#include "include/util/interpolation.fx"

sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);

// FAIL
half4 P0(half4 v0 : COLOR0, half3 Tex0 : TEXCOORD0, half3 Tex1 : TEXCOORD1) : COLOR
{
	float4 r0, r1, r2, r3, r4, r5, r6;

	float4 c6 = float4(0.5, 0.5, 0.5, 0.5);
	float4 c7 = float4(0, 0, 0, 0);

	half4 t0 = tex3D(TexS0, Tex0);
	half4 t1 = tex3D(TexS1, Tex1);

    r0.xyz = t1.w + -(t0 - 0.5).w;
	r0.a   = t0.z + -(t1 - 0.5).z;
    r0.xyz = t1 * c7;
	r0.w = (r0.w > 0.5 ? r0.z : r0.w);	
	r0.w = (r0.w * r0.w) * 4;
	r1.rgb = r0.w * r0.w;
	r1.w = (r0.w > 0.5 ? r0.w : r0.z);
	r1.rgb = r0.w * v0;
	r1.a = (2 * (r1.w - 0.5)) * (2 * (r1.w - 0.5));
	r1.rgb = (r0.w * c6) + r1;
	r0.a = r0.w * r0.w;
	r0.rgb = r0.w * r1;	
	r0.a = v0.w;
	
	return r0;
}

Technique TransparentPlasma
{
    Pass P0
	{
		//PixelShader = compile TGT P0();
		pixelshader = asm
		{
    ps_1_1
    def c6, 0.5, 0.5, 0.5, 0.5
    def c7, 0, 0, 0, 0
    tex t0
    tex t1
    add r0.xyz, t1.w, -t0_bias.w
  + add r0.w, t0.z, -t1_bias.z
    mul r0.xyz, t1, c7
  + cnd r0.w, r0.w, r0.z, r0.w  
    mul_x4 r0.w, r0.w, r0.w
    mul r1.xyz, r0.w, r0.w
  + cnd r1.w, r0.w, r0.w, r0.z	
    mul r1.xyz, r0.w, v0
  + mul r1.w, r1_bx2.w, r1_bx2.w
    mad r1.xyz, r0.w, c6, r1
  + mul r0.w, r0.w, r0.w
    mul r0.xyz, r0.w, r1
  + mov r0.w, v0.w

// approximately 9 instruction slots used (2 texture, 7 arithmetic)
		};
 	}
}