#define VARS_VIEWPROJ
#define VARS_FOG
#define VARS_TEXSCALE
#define VARS_TEXANIM
#define VARS_NODEMATRIX
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"
#include "include/fog.fx"
#include "include/model/position.fx"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float4 D0 : COLOR0;
	float4 D1 : COLOR1;
	float3 T0 : TEXCOORD0;
	float2 T1 : TEXCOORD1;
	float2 T2 : TEXCOORD2;
	float2 T3 : TEXCOORD3;
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
	
	// (1) compute eye vector ----------------------------------------------------------------
	R_EYE_VECTOR.xyz = -R_POSITION.xyz + c_eye_position;	
	
	// (3) compute reflection vector ---------------------------------------------------------
	R_REFLECTION_VECTOR.x = dot(R_EYE_VECTOR.rgb, R_NORMAL.rgb);	
	R_REFLECTION_VECTOR.xyz = (R_REFLECTION_VECTOR.x) * (R_NORMAL);	
	R_REFLECTION_VECTOR.xyz = (R_REFLECTION_VECTOR.xyz) * (c_two) + -R_EYE_VECTOR;	
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(R_POSITION, c_world_view_projection);	
	
	// (12) output texcoords -----------------------------------------------------------------
	// we have to compute the z- axis by the cross product of the x- and y- axes
	R_TEMP1 = c_texture0_xform_y;	
	R_TEMP0 = (c_texture0_xform_x.yzxw) * (R_TEMP1.zxyw);	
	R_TEMP0 = (-R_TEMP1.yzxw) * (c_texture0_xform_x.zxyw) + R_TEMP0;	
	o.T0.x = dot(R_NORMAL.rgb, c_texture0_xform_x.rgb);	
	o.T0.y = dot(R_NORMAL.rgb, c_texture0_xform_y.rgb);	
	o.T0.z = dot(R_NORMAL.rgb, R_TEMP0.rgb);	
	o.T1.x = dot(V_TEXCOORD, c_texture1_xform_x);	
	o.T1.y = dot(V_TEXCOORD, c_texture1_xform_y);	
	o.T2.x = dot(V_TEXCOORD, c_texture2_xform_x);	
	o.T2.y = dot(V_TEXCOORD, c_texture2_xform_y);	
	o.T3.x = dot(V_TEXCOORD, c_texture3_xform_x);	
	o.T3.y = dot(V_TEXCOORD, c_texture3_xform_y);	
	
	// (17) fog ------------------------------------------------------------------------------
	R_PLANAR_FOG_DENSITY = Fog_Complex(R_POSITION);
	
	// (6) fade ------------------------------------------------------------------------------
	R_TEMP0.x = dot(R_NORMAL.rgb, -c_eye_forward.rgb);	
	R_TEMP0.x = max(R_TEMP0.x, -R_TEMP0.x);	
	R_TEMP0.y = V_ONE + -R_TEMP0.x;	
	R_TEMP0.xy = (R_TEMP0.xy) * (R_PLANAR_FOG_DENSITY);	
	o.D0.w = (R_PLANAR_FOG_DENSITY) * (c_translucency);	// no fade
	o.D1.xyzw = (R_TEMP0.xxxy) * (c_translucency);	// fade-when-perpendicular(w), parallel(xyz)

	return o;
}

Technique transparent_generic_object_centered_m
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}