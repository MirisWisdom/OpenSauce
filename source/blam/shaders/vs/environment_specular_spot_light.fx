#define VARS_VIEWPROJ
#define VARS_TEXSCALE
#define VARS_POINTLIGHT
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float4 D0 : COLOR0;
	float4 D1 : COLOR1;
	float2 T0 : TEXCOORD0;
	float3 T1 : TEXCOORD1;
	float3 T2 : TEXCOORD2;
	float3 T3 : TEXCOORD3;
};

// PASS: mathces asm
VS_OUTPUT main(float4 v4 : TEXCOORD0, float4 v0 : POSITION0, float3 v1 : NORMAL0, float3 v2 : BINORMAL0, float3 v3 : TANGENT0)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	half4 a0, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;
	
	// (1) light vector ----------------------------------------------------------------------
	R_LIGHT_VECTOR.xyz = c_light_position.xyz + -V_POSITION;	
	R_LIGHT_VECTOR.w = V_ZERO;	
	
	// (1) eye vector ------------------------------------------------------------------------
	R_EYE_VECTOR.xyz = c_eye_position.xyz + -V_POSITION;	
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(V_POSITION, c_world_view_projection);
	
	// (12) output texcoords -----------------------------------------------------------------
	R_TEMP0.x = dot(V_TEXCOORD, c_base_map_xform_x);	// base map
	R_TEMP0.y = dot(V_TEXCOORD, c_base_map_xform_y);	
	o.T0.xy = (R_TEMP0.xy) * (c_bump_scale);	// bump map
	o.T1.x = dot(R_LIGHT_VECTOR.rgb, c_light_gel_xform_x.rgb);	
	o.T1.y = dot(R_LIGHT_VECTOR.rgb, c_light_gel_xform_y.rgb);	
	o.T1.z = dot(R_LIGHT_VECTOR.rgb, c_light_gel_xform_z.rgb);	
	o.T2.x = dot(R_EYE_VECTOR.rgb, V_TANGENT.rgb);	
	o.T2.y = dot(R_EYE_VECTOR.rgb, V_BINORMAL.rgb);	
	o.T2.z = dot(R_EYE_VECTOR.rgb, V_NORMAL.rgb);	
	o.T3.x = dot(R_LIGHT_VECTOR.rgb, V_TANGENT.rgb);	
	o.T3.y = dot(R_LIGHT_VECTOR.rgb, V_BINORMAL.rgb);	
	o.T3.z = dot(R_LIGHT_VECTOR.rgb, V_NORMAL.rgb);	
	
	// (1) output distance attenuation -------------------------------------------------------
	o.D1.w = dot(-R_LIGHT_VECTOR, c_light_gel_xform_w);	
	
	// (1) make d3d happy by filling all of D1------------------------------------------------
	o.D1.xyz = V_ZERO;	
	

	return o;
}

Technique environment_specular_spot_light
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}