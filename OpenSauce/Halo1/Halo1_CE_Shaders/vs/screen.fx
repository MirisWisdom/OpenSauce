#define VARS_SCREENPROJ
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"

//#define USE_ASM

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float4 D0 : COLOR0;
	float2 T0 : TEXCOORD0;
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
	
	// (1) output texcoord -------------------------------------------------------------------
	o.T0.xy = (V_TEXCOORD.xy) * (c_texture_scale);	

	return o;
}

Technique screen
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
			mul oT0.xy, v4, c17

			// approximately 6 instruction slots used
		};
#endif
	}
}