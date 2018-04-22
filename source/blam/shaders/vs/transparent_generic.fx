#define VARS_VIEWPROJ
#define VARS_FOG
#define VARS_TEXANIM
#define VARS_TEXSCALE
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"
#include "include/fog.fx"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float4 D0 : COLOR0;
	float4 D1 : COLOR1;
	float2 T0 : TEXCOORD0;
	float2 T1 : TEXCOORD1;
	float2 T2 : TEXCOORD2;
	float2 T3 : TEXCOORD3;
};

// PASS: matches asm
VS_OUTPUT main(float4 v4 : TEXCOORD0, float4 v0 : POSITION0, float3 v1 : NORMAL0, float3 v2 : BINORMAL0, float3 v3 : TANGENT0)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	half4 a0, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(V_POSITION, c_world_view_projection);	
	
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
	R_PLANAR_FOG_DENSITY = Fog_Complex(V_POSITION);
		
	// (6) fade ------------------------------------------------------------------------------
	// PERF: we don't always need all this crap
	R_TEMP0.x = dot(V_NORMAL.rgb, -c_eye_forward.rgb);	
	R_TEMP0.x = max(R_TEMP0.x, -R_TEMP0.x);	
	R_TEMP0.y = V_ONE + -R_TEMP0.x;	
	R_TEMP0.xy = (R_TEMP0.xy) * (R_PLANAR_FOG_DENSITY);	
	o.D0.w = (R_PLANAR_FOG_DENSITY) * (c_translucency);	// no fade
	o.D1.xyzw = (R_TEMP0.xxxy) * (c_translucency);	// fade-when-perpendicular(w), parallel(xyz)

	return o;
}

Technique transparent_generic
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}