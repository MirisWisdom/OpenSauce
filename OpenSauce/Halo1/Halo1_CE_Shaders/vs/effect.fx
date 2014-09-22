#define VARS_VIEWPROJ
#define VARS_FOG
#define VARS_INVERSE
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"
#include "include/fog.fx"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float4 D0 : COLOR0;
	float4 T0 : TEXCOORD0;
	float4 T1 : TEXCOORD1;
};

// PASS: matches asm
VS_OUTPUT main(float4 v9 : COLOR0, float4 v0 : POSITION0, float4 v4 : TEXCOORD0)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	half4 a0, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;
	
	// (4) transform position ----------------------------------------------------------------
	R_POSITION.xyz = mul(V_POSITION, c_inverse_transform_matrix);
	R_POSITION.w = V_ONE;	// necessary because we can't use DPH
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(R_POSITION, c_world_view_projection);
	
	o.T1 = o.Pos;
	
	// (1) output texcoords ------------------------------------------------------------------
	o.T0.xy = V_TEXCOORD;	
	
	// (2) output color ----------------------------------------------------------------------
	o.D0.xyz = V_COLOR.xyz;
	R_TEMP1.w = V_COLOR.w;
	
	// (17) fog ------------------------------------------------------------------------------
	R_TEMP0.w = Fog_Complex(R_POSITION);
	
	// (1) output fogged/diffuse alpha ----------------------------------------------------------------------
	o.D0.w = (R_TEMP0.w) * (R_TEMP1.w);	

	return o;
}

Technique effect
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}