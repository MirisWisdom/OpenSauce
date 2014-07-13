#define VARS_SCREENPROJ
#define VARS_TINT
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float4 D0 : COLOR0;
	float4 D1 : COLOR1;
	float2 T0 : TEXCOORD0;
};

// PASS: matches asm
VS_OUTPUT main(float4 v9 : COLOR0, float4 v0 : POSITION0, float4 v4 : TEXCOORD0)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	half4 a0, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(V_POSITION, c_screenproj);
	
	// (1) output texcoords ------------------------------------------------------------------
	o.T0.xy = V_TEXCOORD;	
	
	// (2) output tint color -----------------------------------------------------------------
	o.D0.xyzw = V_COLOR;	
	o.D1.w = c_tint_factor;	

	return o;
}

Technique lens_flare
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}