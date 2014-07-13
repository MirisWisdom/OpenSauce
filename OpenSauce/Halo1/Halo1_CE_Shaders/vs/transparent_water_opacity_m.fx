#define VARS_VIEWPROJ
#define VARS_FOG
#define VARS_NODEMATRIX
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"
#include "include/fog.fx"
#include "include/model/position.fx"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float2 T0 : TEXCOORD0;
	float3 T1 : TEXCOORD1;
	float  Fog : FOG;
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
	
	// (6) transform binormal ----------------------------------------------------------------
	R_BINORMAL.xyz = normalize(mul(V_BINORMAL.xyz, (float3x3)WorldMatrix));
	
	// (6) transform tangent -----------------------------------------------------------------
	R_TANGENT.xyz = normalize(mul(V_TANGENT.xyz, (float3x3)WorldMatrix));
	
	// (1) eye vector ------------------------------------------------------------------------
	R_EYE_VECTOR.xyz = -R_POSITION.xyz + c_eye_position;	
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(R_POSITION, c_world_view_projection);	
	
	// (4) output texcoords ------------------------------------------------------------------
	o.T0.xy = V_TEXCOORD;	// opacity map
	o.T1.x = dot(R_EYE_VECTOR.rgb, R_TANGENT.rgb);	// eye vector in bump map pace
	o.T1.y = dot(R_EYE_VECTOR.rgb, R_BINORMAL.rgb);	
	o.T1.z = dot(R_EYE_VECTOR.rgb, R_NORMAL.rgb);	

	// (2) atmospheric fog ----------------------------------------------------------------
	R_ATMOSPHERIC_FOG_DENSITY = dot(V_POSITION, c_atmospheric_fog_gradient);	
	o.Fog = V_ONE + R_ATMOSPHERIC_FOG_DENSITY;  

	return o;
}

Technique transparent_water_opacity_m
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}