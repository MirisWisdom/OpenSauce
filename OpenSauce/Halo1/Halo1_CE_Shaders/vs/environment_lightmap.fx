#define VARS_VIEWPROJ
#define VARS_TEXSCALE
#define VARS_FAKELIGHT
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float4 D0 : COLOR0;
	float2 T0 : TEXCOORD0;
	float2 T1 : TEXCOORD1;
	float2 T2 : TEXCOORD2;
	float3 T3 : TEXCOORD3;
	float3 EyeVector : TEXCOORD4;
};

// PASS: matches asm
VS_OUTPUT main(float4 v8 : TEXCOORD1, float4 v0 : POSITION0, float3 v1 : NORMAL0, float3 v2 : BINORMAL0, float3 v3 : TANGENT0, float4 v4 : TEXCOORD0, float3 v7 : NORMAL1)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	half4 a0, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;
	
	// (5) transform incident radiosity ------------------------------------------------------
	R_TEMP0.rgb = V_INCIDENT_RADIOSITY;	// we can't read two v[] regs in one instruction
	R_INCIDENT_RADIOSITY.x = dot(R_TEMP0.rgb, V_TANGENT.rgb);	
	R_INCIDENT_RADIOSITY.y = dot(R_TEMP0.rgb, V_BINORMAL.rgb);	
	R_INCIDENT_RADIOSITY.z = dot(R_TEMP0.rgb, V_NORMAL.rgb);	
	R_INCIDENT_RADIOSITY.w = dot(R_TEMP0.rgb, R_TEMP0.rgb);
	
	float3x3 TBN = float3x3(V_TANGENT.rgb, V_BINORMAL.rgb, V_NORMAL.rgb);
	
	o.EyeVector = c_eye_position.xyz - V_POSITION.xyz;
	o.EyeVector = mul(TBN, o.EyeVector);
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(V_POSITION, c_world_view_projection);	
	
	// (1) output radiosity accuracy ---------------------------------------------------------
	o.D0.w = R_INCIDENT_RADIOSITY.w;	
	
	// (7) output texcoords ------------------------------------------------------------------
	R_TEMP0.x = dot(V_TEXCOORD, c_base_map_xform_x);	// base map
	R_TEMP0.y = dot(V_TEXCOORD, c_base_map_xform_y);	
	o.T0.xy = (R_TEMP0.xy) * (c_bump_scale);	// bump map
	o.T1.xy = (R_TEMP0.xy) * (c_self_illumination_scale);	
	o.T2.xy = V_LIGHTMAP_TEXCOORD;	
	R_INCIDENT_RADIOSITY.w = rsqrt(abs(R_INCIDENT_RADIOSITY.w));	
	o.T3.xyz = (R_INCIDENT_RADIOSITY.w) * (R_INCIDENT_RADIOSITY);	
	
	// (9) ouput fake lighting ---------------------------------------------------------------
	R_FAKE_LIGHT_SUM.xyz = c_fake_light_ambient;	
	R_FAKE_LIGHT_SUM.w = dot(V_NORMAL.rgb, c_fake_light0_direction.rgb);	
	R_FAKE_LIGHT_SUM.w = (R_FAKE_LIGHT_SUM.w) * (c_fake_light0_intensity);	
	R_FAKE_LIGHT_SUM.w = max(R_FAKE_LIGHT_SUM.w, V_ZERO);	
	R_FAKE_LIGHT_SUM.xyz = (R_FAKE_LIGHT_SUM.w) * (c_fake_light_color.xyz) + R_FAKE_LIGHT_SUM;	
	R_FAKE_LIGHT_SUM.w = dot(V_NORMAL.rgb, c_fake_light1_direction.rgb);	
	R_FAKE_LIGHT_SUM.w = (R_FAKE_LIGHT_SUM.w) * (c_fake_light1_intensity);	
	R_FAKE_LIGHT_SUM.w = max(R_FAKE_LIGHT_SUM.w, V_ZERO);	
	o.D0.xyz = (R_FAKE_LIGHT_SUM.w) * (c_fake_light_color.xyz) + R_FAKE_LIGHT_SUM;
	

	return o;
}

Technique environment_lightmap
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}