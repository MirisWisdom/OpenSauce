#define VARS_VIEWPROJ
#define VARS_NODEMATRIX
#define VARS_FOG
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"
#include "include/model/position.fx"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float2 T0 : TEXCOORD0;
};

// PASS: matches asm
VS_OUTPUT main(float2 v6 : BLENDWEIGHT0, float4 v0 : POSITION0, float3 v1 : NORMAL0, float3 v2 : BINORMAL0, float3 v3 : TANGENT0, float4 v4 : TEXCOORD0, float2 v5 : BLENDINDICES0)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	half4 a0, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;
	
	// (10) build blended transform (2 nodes) ------------------------------------------------
	//TODO: node index multiplier necessary?
	
	// (4) transform position ----------------------------------------------------------------
	float4x3 WorldMatrix;
	R_POSITION.xyz = GetPosition(V_POSITION,
		V_NODE_INDICES,
		V_NODE_WEIGHTS,
		WorldMatrix,
		false);
	R_POSITION.w = V_ONE;	// necessary because we can't use DPH

	o.T0 = v4;	

	return o;
}

Technique model_ff
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}