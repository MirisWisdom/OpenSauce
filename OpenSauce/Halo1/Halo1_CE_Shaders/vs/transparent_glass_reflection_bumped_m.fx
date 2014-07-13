#define VARS_VIEWPROJ
#define VARS_FOG
#define VARS_TEXSCALE
#define VARS_NODEMATRIX
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"
#include "include/fog.fx"
#include "include/model/position.fx"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float4 D0 : COLOR0;
	float2 T0 : TEXCOORD0;
	float4 T1 : TEXCOORD1;
	float4 T2 : TEXCOORD2;
	float4 T3 : TEXCOORD3;
};

// PASS: matches asm
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
	
	// (6) transform binormal ----------------------------------------------------------------
	R_BINORMAL.xyz = normalize(mul(V_BINORMAL.xyz, (float3x3)WorldMatrix));
	
	// (6) transform tangent -----------------------------------------------------------------
	R_TANGENT.xyz = normalize(mul(V_TANGENT.xyz, (float3x3)WorldMatrix));	
	
	// (1) eye vector ------------------------------------------------------------------------
	R_EYE_VECTOR.xyz = -R_POSITION.xyz + c_eye_position;	
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(R_POSITION, c_world_view_projection);	
	
	// (13) output texcoords -----------------------------------------------------------------
	o.T0.xy = (V_TEXCOORD.xy) * (c_primary_detail_scale);	// bump map
	o.T1.x = R_TANGENT.x;	
	o.T1.y = R_BINORMAL.x;	
	o.T1.z = R_NORMAL.x;	
	o.T2.x = R_TANGENT.y;	
	o.T2.y = R_BINORMAL.y;	
	o.T2.z = R_NORMAL.y;	
	o.T3.x = R_TANGENT.z;	
	o.T3.y = R_BINORMAL.z;	
	o.T3.z = R_NORMAL.z;	
	o.T1.w = R_EYE_VECTOR.x;	
	o.T2.w = R_EYE_VECTOR.y;	
	o.T3.w = R_EYE_VECTOR.z;	
	
	// (17) fog ------------------------------------------------------------------------------
	o.D0.w = Fog_Complex(R_POSITION);
	o.D0.xyz = c_glass_one;	

	return o;
}

Technique transparent_glass_reflection_bumped_m
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}