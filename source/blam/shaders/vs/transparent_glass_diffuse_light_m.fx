#define VARS_VIEWPROJ
#define VARS_FOG
#define VARS_TEXSCALE
#define VARS_TEXANIM
#define VARS_VERTEXLIGHT
#define VARS_NODEMATRIX
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"
#include "include/fog.fx"
#include "include/model/position.fx"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float4 D0 : COLOR0;
	float2 T0 : TEXCOORD0;
	float2 T1 : TEXCOORD1;
	float2 T2 : TEXCOORD2;
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
	
	// (1) eye vector ------------------------------------------------------------------------
	R_EYE_VECTOR.xyz = -R_POSITION.xyz + c_eye_position;	
	
	// (9) compute point light 0 attenuation -------------------------------------------------
	R_LIGHT_VECTOR.xyz = c_light0_position.xyz + -R_POSITION;	
	R_LIGHT_VECTOR.w = dot(R_LIGHT_VECTOR.rgb, R_LIGHT_VECTOR.rgb);	
	R_TEMP0.w = rsqrt(abs(R_LIGHT_VECTOR.w));	
	R_LIGHT_VECTOR.xyz = (R_LIGHT_VECTOR.xyz) * (R_TEMP0.w);	
	R_LIGHT0_DIST_ATTEN = (R_LIGHT_VECTOR.w) * (-c_light0_inv_radius_sq) + V_ONE;	
	R_LIGHT0_ANGL_ATTEN = dot(R_LIGHT_VECTOR.rgb, R_NORMAL.rgb);	
	R_TEMP0.w = dot(R_LIGHT_VECTOR.rgb, c_light0_direction.rgb);	
	R_TEMP0.w = (R_TEMP0.w) * (c_light0_spot_a);	
	R_LIGHT0_SPOT_ATTEN = R_TEMP0.w + c_light0_spot_b;	
	
	// (9) compute point light 1 attenuation -------------------------------------------------
	R_LIGHT_VECTOR.xyz = c_light1_position.xyz + -R_POSITION;	
	R_LIGHT_VECTOR.w = dot(R_LIGHT_VECTOR.rgb, R_LIGHT_VECTOR.rgb);	
	R_TEMP0.w = rsqrt(abs(R_LIGHT_VECTOR.w));	
	R_LIGHT_VECTOR.xyz = (R_LIGHT_VECTOR.xyz) * (R_TEMP0.w);	
	R_LIGHT1_DIST_ATTEN = (R_LIGHT_VECTOR.w) * (-c_light1_inv_radius_sq) + V_ONE;	
	R_LIGHT1_ANGL_ATTEN = dot(R_LIGHT_VECTOR.rgb, R_NORMAL.rgb);	
	R_TEMP0.w = dot(R_LIGHT_VECTOR.rgb, c_light0_direction.rgb);	
	R_TEMP0.w = (R_TEMP0.w) * (c_light1_spot_a);	
	R_LIGHT1_SPOT_ATTEN = R_TEMP0.w + c_light1_spot_b;	
	
	// (1) compute distant light 2 attenuation -----------------------------------------------
	R_LIGHT2_ATTEN = dot(R_NORMAL.rgb, -c_light2_direction.rgb);	
	
	// (1) compute distant light 3 attenuation -----------------------------------------------
	R_LIGHT3_ATTEN = dot(R_NORMAL.rgb, -c_light3_direction.rgb);	
	
	// (4) clamp all light attenuations ------------------------------------------------------
	R_LIGHT_ATTENUATION0 = max(R_LIGHT_ATTENUATION0, V_ZERO);	
	R_LIGHT_ATTENUATION1 = max(R_LIGHT_ATTENUATION1, V_ZERO);	
	R_LIGHT_ATTENUATION0 = min(R_LIGHT_ATTENUATION0, V_ONE);	
	R_LIGHT_ATTENUATION1 = min(R_LIGHT_ATTENUATION1, V_ONE);	
	
	// (8) output light contributions --------------------------------------------------------
	R_LIGHT_SUM.xyz = c_light_ambient;	
	R_TEMP0.xy = (R_LIGHT_ATTENUATION0.xy) * (R_LIGHT_ATTENUATION1);	
	R_TEMP0.x = (R_TEMP0.x) * (R_LIGHT0_SPOT_ATTEN);	
	R_TEMP0.y = (R_TEMP0.y) * (R_LIGHT1_SPOT_ATTEN);	
	R_LIGHT_SUM.xyz = (R_TEMP0.x) * (c_light0_color.xyz) + R_LIGHT_SUM;	
	R_LIGHT_SUM.xyz = (R_TEMP0.y) * (c_light1_color.xyz) + R_LIGHT_SUM;	
	R_LIGHT_SUM.xyz = (R_LIGHT2_ATTEN) * (c_light2_color.xyz) + R_LIGHT_SUM;	
	o.D0.xyz = (R_LIGHT3_ATTEN) * (c_light3_color.xyz) + R_LIGHT_SUM;	
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(R_POSITION, c_world_view_projection);	
	
	// (2) output texcoords ------------------------------------------------------------------
	o.T0.xy = (V_TEXCOORD.xy) * (c_primary_detail_scale);	// diffuse map
	o.T1.xy = (V_TEXCOORD.xy) * (c_secondary_detail_scale);	// diffuse detail map
	o.T2.xy = V_ZERO;	
	
	// (17) fog ------------------------------------------------------------------------------
	o.D0.w = Fog_Complex(R_POSITION);
	return o;
}

Technique transparent_glass_diffuse_light_m
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}