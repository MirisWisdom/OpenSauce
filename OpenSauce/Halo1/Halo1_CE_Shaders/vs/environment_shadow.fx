#define VARS_VIEWPROJ
#define VARS_SHADOW
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float4 D0 : COLOR0;
	float2 T0 : TEXCOORD0;
	float2 T1 : TEXCOORD1;
};

// PASS: matches asm
VS_OUTPUT main(float4 v4 : TEXCOORD0, float4 v0 : POSITION0, float3 v1 : NORMAL0, float3 v2 : BINORMAL0, float3 v3 : TANGENT0)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	half4 a0, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(V_POSITION, c_world_view_projection);	
	
	// (4) output texcoords ------------------------------------------------------------------
	o.T0.x = dot(V_POSITION, c_shadow_xform_x);	
	o.T0.y = dot(V_POSITION, c_shadow_xform_y);	
	o.T1.x = dot(V_POSITION, c_shadow_fade_forward);	
	o.T1.y = dot(V_POSITION, c_shadow_fade_reverse);	
	
	// (1) smooth shadows --------------------------------------------------------------------
	o.D0.w = dot(-c_shadow_axis.rgb, V_NORMAL.rgb);	

	return o;
}

Technique environment_shadow
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}