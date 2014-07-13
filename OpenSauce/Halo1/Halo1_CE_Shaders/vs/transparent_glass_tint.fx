#define VARS_VIEWPROJ
#define VARS_FOG
#define VARS_TEXSCALE
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"
#include "include/fog.fx"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float4 D0 : COLOR0;
	float2 T0 : TEXCOORD0;
};

// PASS: matches asm
VS_OUTPUT main(float4 v4 : TEXCOORD0, float4 v0 : POSITION0, float3 v1 : NORMAL0, float3 v2 : BINORMAL0, float3 v3 : TANGENT0)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	half4 a0, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(V_POSITION, c_world_view_projection);	
	
	// (1) output texcoords ------------------------------------------------------------------
	o.T0.xy = (V_TEXCOORD.xy) * (c_primary_detail_scale);	// tint map
	
	// (17) fog ------------------------------------------------------------------------------
	o.D0.w = Fog_Complex(V_POSITION);
	o.D0.xyz = c_glass_zero;	

	return o;
}

Technique transparent_glass_tint
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}