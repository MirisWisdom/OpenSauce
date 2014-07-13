#define VARS_VIEWPROJ
#define VARS_TEXSCALE
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float2 T0 : TEXCOORD0;
	float4 T1 : TEXCOORD1;
	float4 T2 : TEXCOORD2;
	float4 T3 : TEXCOORD3;
};

// PASS: matches asm
VS_OUTPUT main(float4 v4 : TEXCOORD0, float4 v0 : POSITION0, float3 v1 : NORMAL0, float3 v2 : BINORMAL0, float3 v3 : TANGENT0)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	half4 a0, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;
	
	// (1) eye vector ------------------------------------------------------------------------
	R_EYE_VECTOR.xyz = -V_POSITION.xyz + c_eye_position;
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(V_POSITION, c_world_view_projection);	
	
	// (15) output texcoords -----------------------------------------------------------------
	R_TEMP0.x = dot(V_TEXCOORD, c_base_map_xform_x);	// base map
	R_TEMP0.y = dot(V_TEXCOORD, c_base_map_xform_y);	
	o.T0.xy = (R_TEMP0.xy) * (c_bump_scale);	// bump map
	o.T1.x = V_TANGENT.x;	
	o.T1.y = V_BINORMAL.x;	
	o.T1.z = V_NORMAL.x;	
	o.T2.x = V_TANGENT.y;	
	o.T2.y = V_BINORMAL.y;	
	o.T2.z = V_NORMAL.y;	
	o.T3.x = V_TANGENT.z;	
	o.T3.y = V_BINORMAL.z;	
	o.T3.z = V_NORMAL.z;	
	o.T1.w = R_EYE_VECTOR.x;	
	o.T2.w = R_EYE_VECTOR.y;	
	o.T3.w = R_EYE_VECTOR.z;	

	return o;
}

Technique environment_reflection_bumped
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}