#define VARS_VIEWPROJ
#define VARS_TEXSCALE
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float2 T0 : TEXCOORD0;
	float3 T1 : TEXCOORD1;
	float3 T2 : TEXCOORD2;
	float4 T3 : TEXCOORD3;
};

// PASS: matches asm
VS_OUTPUT main(float4 v4 : TEXCOORD0, float4 v0 : POSITION0, float3 v1 : NORMAL0, float3 v2 : BINORMAL0, float3 v3 : TANGENT0)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	half4 a0, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;
	
	// (5) output homogeneous point ----------------------------------------------------------
	R_POSITION = mul(V_POSITION, c_world_view_projection);
	o.Pos = R_POSITION;	
	
	// (7) output texcoords ------------------------------------------------------------------
	// (NOTE - since glass is always planar, we could get rid of the normalization cube map.
	//  more generally, we could switch to a "planar" version of this vertex shader if the
	//  type is FLAT and the geometry is planar, -or- if the type is MIRROR since we assume the
	//  geometry is planar)
	R_TEMP0.x = dot(V_TEXCOORD, c_base_map_xform_x);	// base map
	R_TEMP0.y = dot(V_TEXCOORD, c_base_map_xform_y);	
	o.T0.xy = (R_TEMP0.xy) * (c_bump_scale);	// bump map
	o.T1.x = dot(c_eye_forward.rgb, V_TANGENT.rgb);	// non-local viewer eye vector in tangent space
	o.T1.y = dot(c_eye_forward.rgb, V_BINORMAL.rgb);	// (dot with T0 to get fresnel term)
	o.T1.z = dot(c_eye_forward.rgb, V_NORMAL.rgb);	
	o.T2.xyz = V_NORMAL;	// normal in worldspace
	
	// (6) mirror ----------------------------------------------------------------------------
	R_POSITION = R_POSITION.xyww + R_POSITION.w;	// range-compress texcoords
	o.T3.xy = R_POSITION;	
	o.T3.zw = R_POSITION.w;	// perspective projection
	
	return o;
}

Technique environment_reflection_mirror
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}