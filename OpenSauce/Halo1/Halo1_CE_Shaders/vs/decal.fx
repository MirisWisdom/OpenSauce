#define VARS_VIEWPROJ
#define VARS_FOG
#define VARS_DECAL
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"
#include "include/fog.fx"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float4 D0 : COLOR0;
	float4 D1 : COLOR1;
	float2 T0 : TEXCOORD0;
};

// PASS: matches asm
VS_OUTPUT main(float4 v4 : COLOR0, float4 v0 : POSITION0)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	half4 a0, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;	
		
	// this is actally a packed texture set of texture coordinates
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(V_POSITION, c_world_view_projection); 
	
	// (1) output texcoords ------------------------------------------------------------------
	o.T0.xy = V_TEXCOORD;	
	
	// (1) output color ----------------------------------------------------------------------
	o.D0.xyzw = c_decal_color;	
	
	// (4) fog hack --------------------------------------------------------------------------
	R_ATMOSPHERIC_FOG_DENSITY = dot(V_POSITION, c_atmospheric_fog_gradient);	
	R_ATMOSPHERIC_FOG_DENSITY = min(R_ATMOSPHERIC_FOG_DENSITY, V_ONE);	
	R_ATMOSPHERIC_FOG_DENSITY = R_ATMOSPHERIC_FOG_DENSITY * c_atmospheric_fog_max_density;	
	o.D1 = step(R_ATMOSPHERIC_FOG_DENSITY, V_ONE);	

	return o;
}

Technique decal
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}