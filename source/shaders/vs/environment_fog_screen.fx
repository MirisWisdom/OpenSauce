#define VARS_VIEWPROJ
#define VARS_TEXSCALE
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float2 T0 : TEXCOORD0;
};

// PASS: matches asm
VS_OUTPUT main(float4 v4 : TEXCOORD0, float4 v0 : POSITION0, float3 v1 : NORMAL0, float3 v2 : BINORMAL0, float3 v3 : TANGENT0)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	half4 a0, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(V_POSITION, c_world_view_projection);	
	
	// PERF: choose one
	//; (2) output fog density ----------------------------------------------------------------
	//dp4 oFog.xw, V_POSITION, C_FOG_SCREEN_GRADIENT
	o.T0.x = dot(V_POSITION, c_fog_screen_gradient);	

	return o;
}

Technique environment_fog_screen
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}