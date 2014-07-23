#define VARS_SCREENPROJ
#define VARS_SCREENPROJ2
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"

#define USE_ASM

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float4 D0 : COLOR0;
	float2 T0 : TEXCOORD0;
	float2 T1 : TEXCOORD1;
	float2 T2 : TEXCOORD2;
};

// PASS: matches asm
VS_OUTPUT main(float4 v9 : COLOR0, float4 v0 : POSITION0, float4 v4 : TEXCOORD0)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	half4 a0, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;	
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(V_POSITION, c_screenproj);
	
	// (1) output color ----------------------------------------------------------------------
	o.D0.xyzw = V_COLOR;	
	
	// (5) texture coordinate 0 --------------------------------------------------------------
	R_TEMP0.xy = (c_screen_mask_0_1.xx) * (V_POSITION.xy);	// generate the texture coordinate
	R_TEMP0.xy = (V_TEXCOORD.xy) * (c_screen_mask_0_1.yy) + R_TEMP0.xy;	
	R_TEMP0.xy = (R_TEMP0.xy) * (c_screen_scales_0_1.xy);	// multiply by the scale
	R_TEMP0.xy = R_TEMP0.xy + c_screen_mask_2_off_0.zw;	// subtract the offset
	o.T0.xy = (R_TEMP0.xy) * (c_texture_scale.xy);	// multiply in the texture scale and store
	
	// (5) texture coordinate 1 --------------------------------------------------------------
	R_TEMP0.xy = (c_screen_mask_0_1.zz) * (V_POSITION.xy);	// generate the texture coordinate
	R_TEMP0.xy = (V_TEXCOORD.xy) * (c_screen_mask_0_1.ww) + R_TEMP0.xy;	
	R_TEMP0.xy = (R_TEMP0.xy) * (c_screen_scales_0_1.zw);	// multiply by the scale
	R_TEMP0.xy = R_TEMP0.xy + c_screen_off_1_2.xy;	// subtract the offset
	o.T1.xy = (R_TEMP0.xy) * (c_screen_texture_scales_1_2.xy);	// multiply in the scale and store
	
	// (5) texture coordinate 2 --------------------------------------------------------------
	R_TEMP0.xy = (c_screen_mask_2_off_0.xx) * (V_POSITION.xy);	// generate the texture coordinate
	R_TEMP0.xy = (V_TEXCOORD.xy) * (c_screen_mask_2_off_0.yy) + R_TEMP0.xy;	
	R_TEMP0.xy = (R_TEMP0.xy) * (c_screen_scales_2.xy);	// multiply by the scale
	R_TEMP0.xy = R_TEMP0.xy + c_screen_off_1_2.zw;	// subtract the offset
	o.T2.xy = (R_TEMP0.xy) * (c_screen_texture_scales_1_2.zw);	// multiply in the scale and store

	return o;
}

Technique screen2
{
	Pass P0
	{
#ifndef USE_ASM
		VertexShader = compile TGT main();
#else
		vertexshader = asm
		{
			vs_1_1
			dcl_position v0
			dcl_texcoord v4
			dcl_color v9
			dp4 oPos.x, v0, c13
			dp4 oPos.y, v0, c14
			dp4 oPos.z, v0, c15
			dp4 oPos.w, v0, c16
			mov oD0, v9
			mul r10.xy, c19.x, v0.xyyy
			mad r10.xy, v4, c19.y, r10.xyyy
			mul r10.xy, r10.xyyy, c22.xyyy
			add r10.xy, r10.xyyy, c20.zwww
			mul oT0.xy, r10.xyyy, c17.xyyy
			mul r10.xy, c19.z, v0.xyyy
			mad r10.xy, v4, c19.w, r10.xyyy
			mul r10.xy, r10.xyyy, c22.zwww
			add r10.xy, r10.xyyy, c21.xyyy
			mul oT1.xy, r10.xyyy, c18.xyyy
			mul r10.xy, c20.x, v0.xyyy
			mad r10.xy, v4, c20.y, r10.xyyy
			mul r10.xy, r10.xyyy, c23.xyyy
			add r10.xy, r10.xyyy, c21.zwww
			mul oT2.xy, r10.xyyy, c18.zwww

			// approximately 20 instruction slots used
		};
#endif
	}
}