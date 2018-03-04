#define VARS_VIEWPROJ
#define VARS_FOG
#define VARS_TEXSCALE
#define VARS_VERTEXLIGHT
//#define VARS_TEXANIM
#define VARS_NODEMATRIX
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"
#include "include/fog.fx"
#include "include/model/position.fx"

// have to hack this together as the registers overlap
float4 		c_texture0_xform_x					: register(c[TEXANIM_OFFSET + 0]);
float4 		c_texture0_xform_y					: register(c[TEXANIM_OFFSET + 1]);
#define		c_texture1_xform_x					c_light0_position
#define	 	c_texture1_xform_y					c_light0_direction
#define	 	c_texture2_xform_x					c_light0_color
#define	 	c_texture2_xform_y					c_light1_position
#define	 	c_texture3_xform_x					c_light1_direction
#define	 	c_texture3_xform_y					c_light1_color

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float4 D0 : COLOR0;
	float4 D1 : COLOR1;
	float2 T0 : TEXCOORD0;
	float2 T1 : TEXCOORD1;
	float2 T2 : TEXCOORD2;
	float2 T3 : TEXCOORD3;
};

// FAIL: original asm has overlapping tex coords!
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
	
	// (9) compute point light 0 attenuation -------------------------------------------------
	R_LIGHT_VECTOR.xyz = c_light0_position.xyz + -R_POSITION;	
	R_LIGHT_VECTOR.w = dot(R_LIGHT_VECTOR.rgb, R_LIGHT_VECTOR.rgb);	
	R_TEMP0.w = rsqrt(abs(R_LIGHT_VECTOR.w));	
	R_LIGHT_VECTOR.xyz = (R_LIGHT_VECTOR.xyz) * (R_TEMP0.w);	
	R_LIGHT0_DIST_ATTEN = (R_LIGHT_VECTOR.w) * (-c_light0_inv_radius_sq) + V_ONE;	
	R_LIGHT0_ANGL_ATTEN = dot(R_LIGHT_VECTOR.rgb, R_NORMAL.rgb);	// light hitting surface directly
	R_TEMP0.w = dot(R_LIGHT_VECTOR.rgb, -c_light0_direction.rgb);	
	R_TEMP0.w = (R_TEMP0.w) * (c_light0_spot_a);	
	R_LIGHT0_SPOT_ATTEN = R_TEMP0.w + c_light0_spot_b;	
	
	// (9) compute point light 1 attenuation -------------------------------------------------
	R_LIGHT_VECTOR.xyz = c_light1_position.xyz + -R_POSITION;	
	R_LIGHT_VECTOR.w = dot(R_LIGHT_VECTOR.rgb, R_LIGHT_VECTOR.rgb);	
	R_TEMP0.w = rsqrt(abs(R_LIGHT_VECTOR.w));	
	R_LIGHT_VECTOR.xyz = (R_LIGHT_VECTOR.xyz) * (R_TEMP0.w);	
	R_LIGHT1_DIST_ATTEN = (R_LIGHT_VECTOR.w) * (-c_light1_inv_radius_sq) + V_ONE;	
	R_LIGHT1_ANGL_ATTEN = dot(R_LIGHT_VECTOR.rgb, R_NORMAL.rgb);	// light hitting surface directly
	R_TEMP0.w = dot(R_LIGHT_VECTOR.rgb, -c_light1_direction.rgb);	
	R_TEMP0.w = (R_TEMP0.w) * (c_light1_spot_a);	
	R_LIGHT1_SPOT_ATTEN = R_TEMP0.w + c_light1_spot_b;	
	
	// (3) compute distant light 2 attenuation -----------------------------------------------
	R_LIGHT2_ATTEN = dot(R_NORMAL.rgb, -c_light2_direction.rgb);	// light hitting surface directly
	R_TEMP1.w = (-R_LIGHT2_ATTEN) * (c_translucency);	// light shining through surface
	R_LIGHT2_ATTEN = max(R_LIGHT2_ATTEN, R_TEMP1.w);	
	
	// (1) compute distant light 3 attenuation -----------------------------------------------
	R_LIGHT3_ATTEN = dot(R_NORMAL.rgb, -c_light3_direction.rgb);	// light hitting surface directly
	
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
	
	// (8) output texcoords ------------------------------------------------------------------
	o.T0.x = dot(V_TEXCOORD, c_texture0_xform_x);	
	o.T0.y = dot(V_TEXCOORD, c_texture0_xform_y);	
	o.T1.x = dot(V_TEXCOORD, c_texture1_xform_x);	
	o.T1.y = dot(V_TEXCOORD, c_texture1_xform_y);	
	o.T2.x = dot(V_TEXCOORD, c_texture2_xform_x);	
	o.T2.y = dot(V_TEXCOORD, c_texture2_xform_y);	
	o.T3.x = dot(V_TEXCOORD, c_texture3_xform_x);	
	o.T3.y = dot(V_TEXCOORD, c_texture3_xform_y);
	
	// (17) fog ------------------------------------------------------------------------------
	R_PLANAR_FOG_DENSITY = Fog_Complex(R_POSITION);
	
	// (6) fade ------------------------------------------------------------------------------
	// PERF: we don't always need all this crap
	R_TEMP0.x = dot(R_NORMAL.rgb, -c_eye_forward.rgb);	
	R_TEMP0.x = max(R_TEMP0.x, -R_TEMP0.x);	
	R_TEMP0.y = V_ONE + -R_TEMP0.x;	
	R_TEMP0.xy = (R_TEMP0.xy) * (R_PLANAR_FOG_DENSITY);	
	o.D0.w = (R_PLANAR_FOG_DENSITY) * (c_translucency);	// no fade
	o.D1.xyzw = (R_TEMP0.xxxy) * (c_translucency);	// fade-when-perpendicular(w), parallel(xyz)

	return o;
}

Technique transparent_generic_lit_m
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}