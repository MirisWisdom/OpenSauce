#define VARS_VIEWPROJ
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float4 D0 : COLOR0;
};

// PASS: Matches vs_1_1 asm
VS_OUTPUT main(float4 v9 : COLOR0, float4 v0 : POSITION0)
{
	VS_OUTPUT o = (VS_OUTPUT)0;	  

	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(V_POSITION, c_world_view_projection); 

	// (1) output color ----------------------------------------------------------------------
	o.D0.xyzw = V_COLOR;	

	return o;
}

Technique debug
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}