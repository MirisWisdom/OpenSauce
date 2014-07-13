#define VARS_VIEWPROJ
#define VARS_FOG
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float2 T0 : TEXCOORD0;
	float Fog : FOG;
};

// PASS: matches asm
VS_OUTPUT main(float4 v4 : TEXCOORD0, float4 v0 : POSITION0, float3 v1 : NORMAL0, float3 v2 : BINORMAL0, float3 v3 : TANGENT0)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	half4 a0, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(V_POSITION, c_world_view_projection);	
	
	// (1) base map --------------------------------------------------------------------------
	o.T0.xy = V_TEXCOORD;
	
	// (2) atmospheric fog ----------------------------------------------------------------
	R_ATMOSPHERIC_FOG_DENSITY = dot(V_POSITION, c_atmospheric_fog_gradient);	
	o.Fog = V_ONE + R_ATMOSPHERIC_FOG_DENSITY;	
	
	return o;
}

Technique transparent_meter
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}