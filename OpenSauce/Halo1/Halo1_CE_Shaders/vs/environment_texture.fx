#define VARS_VIEWPROJ
#define VARS_TEXSCALE
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float2 T0 : TEXCOORD0;
	float2 T1 : TEXCOORD1;
	float2 T2 : TEXCOORD2;
	float2 T3 : TEXCOORD3;
};

// PASS: matches asm
VS_OUTPUT main(float4 v4 : TEXCOORD0, float4 v0 : POSITION0, float3 v1 : NORMAL0, float3 v2 : BINORMAL0, float3 v3 : TANGENT0)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	half4 a0, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(V_POSITION, c_world_view_projection);
	
	// (7) output texcoords ------------------------------------------------------------------
	R_TEMP0.x = dot(V_TEXCOORD, c_base_map_xform_x);	// base map
	R_TEMP0.y = dot(V_TEXCOORD, c_base_map_xform_y);	
	o.T0.xy = R_TEMP0;	
	o.T1.xy = (R_TEMP0.xy) * (c_primary_detail_scale);	
	o.T2.xy = (R_TEMP0.xy) * (c_secondary_detail_scale);	
	o.T3.x = (R_TEMP0.x) * (c_micro_detail_scale_x);	
	o.T3.y = (R_TEMP0.y) * (c_micro_detail_scale_y);	

	return o;
}

Technique environment_texture
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}