#define VARS_VIEWPROJ
#define VARS_NODEMATRIX
#define VARS_TEXSCALE
#define VARS_FOG
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"
#include "include/model/position.fx"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float2 T0 : TEXCOORD0;
	float2 T1 : TEXCOORD1;
	float Fog : FOG;
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
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(R_POSITION, c_world_view_projection);
	
	// (2) output fog density ----------------------------------------------------------------
	o.Fog = dot(R_POSITION, c_fog_screen_gradient);	
	o.T0.x = o.Fog;
	o.T0.y = V_ZERO;	
	
	// (2) output texcoords for alpha-kill ---------------------------------------------------
	o.T1.x = dot(V_TEXCOORD, c_base_map_xform_x);	// base map
	o.T1.y = dot(V_TEXCOORD, c_base_map_xform_y);	

	return o;
}

Technique model_fog_screen
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}