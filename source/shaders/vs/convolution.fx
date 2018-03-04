#define VARS_TEXANIM
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float2 T0 : TEXCOORD0;
	float2 T1 : TEXCOORD1;
	float2 T2 : TEXCOORD2;
	float2 T3 : TEXCOORD3;
};

// PASS: matches asm
VS_OUTPUT main(float4 v9 : COLOR0, float4 v0 : POSITION0, float4 v4 : TEXCOORD0)
{
	VS_OUTPUT o = (VS_OUTPUT)0;

	// (1) output homogeneous point ----------------------------------------------------------
	o.Pos = V_POSITION;	

	// (8) output texcoords ------------------------------------------------------------------
	o.T0.x = dot(V_TEXCOORD, c_texture0_xform_x);	
	o.T0.y = dot(V_TEXCOORD, c_texture0_xform_y);	
	o.T1.x = dot(V_TEXCOORD, c_texture1_xform_x);	
	o.T1.y = dot(V_TEXCOORD, c_texture1_xform_y);	
	o.T2.x = dot(V_TEXCOORD, c_texture2_xform_x);	
	o.T2.y = dot(V_TEXCOORD, c_texture2_xform_y);	
	o.T3.x = dot(V_TEXCOORD, c_texture3_xform_x);	
	o.T3.y = dot(V_TEXCOORD, c_texture3_xform_y);	

	return o;
}

Technique convolution
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}