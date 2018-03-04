#define VARS_VIEWPROJ
#define VARS_NODEMATRIX
#define VARS_TEXSCALE
#define VARS_FOG
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"
#include "include/model/position.fx"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float4 D0 : COLOR0;
	float2 T0 : TEXCOORD0;
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
	
	// (8) eye vector (incidence) ------------------------------------------------------------
	R_EYE_VECTOR.xyz = -R_POSITION.xyz + c_eye_position;	
	R_EYE_VECTOR.w = dot(R_EYE_VECTOR.rgb, c_eye_forward.rgb);	
	R_EYE_VECTOR.w = (R_EYE_VECTOR.w) * (R_EYE_VECTOR.w);	// squared distance
	R_EYE_VECTOR.w = 1.f / (R_EYE_VECTOR.w);	
	R_EYE_VECTOR.w = (R_EYE_VECTOR.w) * (c_active_camo_falloff);	// refraction scale
	R_EYE_VECTOR.w = min(R_EYE_VECTOR.w, V_ONE);	// clamp to one
	R_EYE_VECTOR.w = max(R_EYE_VECTOR.w, V_ZERO);	// clamp to zero
	o.D0.w = R_EYE_VECTOR.w;	// distance also affects edge-density
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(R_POSITION, c_world_view_projection);	
	
	// output texcoords ----------------------------------------------------------------------
	// (8) projection ------------------------------------------------------------------------
	R_TEMP0.xy = o.Pos.xy / o.Pos.w;
	R_TEMP0.xy = (R_TEMP0.xy + float2(1.0f, -1.0f)) * 0.5f;
	R_TEMP0.y *= -1.0f;
	o.T0.x = (c_mirror_scale_u) * (R_TEMP0.x);	// base u-texcoord
	o.T0.y = (c_mirror_scale_v) * (R_TEMP0.y);	// base v-texcoord
	
	// (1) output tint color -----------------------------------------------------------------
	o.D0.xyz = c_base_map_xform_y;	

	return o;
}

Technique model_active_camouflage_ff
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}