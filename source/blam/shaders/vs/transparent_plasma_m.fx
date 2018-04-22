#define VARS_VIEWPROJ
#define VARS_FOG
#define VARS_TEXSCALE
#define VARS_TEXANIM
#define VARS_NODEMATRIX
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"
#include "include/fog.fx"
#include "include/model/position.fx"


float4x3 c_bone_matrices : register(c[29]);
float4 c[19] : register(c[0]);

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float4 D0 : COLOR0;
	float3 T0 : TEXCOORD0;
	float3 T1 : TEXCOORD1;
};

// PASS
VS_OUTPUT main(float2 v6 : BLENDWEIGHT0, float4 v0 : POSITION0, float3 v1 : NORMAL0, float3 v2 : BINORMAL0, float3 v3 : TANGENT0, float4 v4 : TEXCOORD0, float2 v5 : BLENDINDICES0)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	half4 a0, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;
	
	// (4) transform position ----------------------------------------------------------------
	float4x3 WorldMatrix;
	R_POSITION.xyz = GetPosition(V_POSITION,
		V_NODE_INDICES,
		V_NODE_WEIGHTS,
		WorldMatrix,
		false);
	R_POSITION.w = V_ONE;	// necessary because we can't use DPH
	
	// (6) transform normal ------------------------------------------------------------------
	R_NORMAL.xyz = normalize(mul(V_NORMAL.xyz, (float3x3)WorldMatrix));
	
	// (1) eye vector ------------------------------------------------------------------------
	R_EYE_VECTOR.xyz = -R_POSITION.xyz + c_eye_position;	
	
	// (1) offset position -------------------------------------------------------------------
	R_POSITION.xyz = R_NORMAL.xyz * (c_texture0_xform_x.z) + R_POSITION;	
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(R_POSITION, c_world_view_projection);	
	
	// (6) output texcoords ------------------------------------------------------------------
	o.T0.x = dot(V_POSITION, c_texture0_xform_x);	
	o.T0.y = dot(V_POSITION, c_texture0_xform_y);	
	o.T0.z = dot(V_POSITION, c_texture1_xform_x);	
	o.T1.x = dot(V_POSITION, c_texture1_xform_y);	
	o.T1.y = dot(V_POSITION, c_texture2_xform_x);	
	o.T1.z = dot(V_POSITION, c_texture2_xform_y);	
	
	// (6) output reflection tint color ------------------------------------------------------
	R_EYE_VECTOR.w = dot(R_EYE_VECTOR.rgb, R_EYE_VECTOR.rgb);	// E.E
	R_EYE_VECTOR.w = rsqrt(R_EYE_VECTOR.w);	// 1/|E|
	R_EYE_VECTOR = (R_EYE_VECTOR) * (R_EYE_VECTOR.w);	// E'= E/|E|
	R_EYE_VECTOR = dot(R_EYE_VECTOR.rgb, R_NORMAL.rgb);	// N.E'
	// CLEAN: this is really __effect, but since __effect and __texanim overlap we can't use them both at once
	R_TINT_COLOR = (R_EYE_VECTOR) * (c_base_map_xform_y);	// perpendicular - parallel
	R_TEMP0 = R_TINT_COLOR + c_base_map_xform_x;	
	
	// (17) fog ------------------------------------------------------------------------------
	R_TEMP1 = Fog_Complex(R_POSITION);
	
	o.D0.xyz = R_TEMP0;	
	o.D0.w = (R_TEMP0.w) * (R_TEMP1);
	
	return o;
}

Technique transparent_plasma_m
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}