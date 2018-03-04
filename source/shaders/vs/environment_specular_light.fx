#define VARS_VIEWPROJ
#define VARS_TEXSCALE
#define VARS_POINTLIGHT
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float4 D0 : COLOR0;
	float4 D1 : COLOR1;
	float2 T0 : TEXCOORD0;
	float3 T1 : TEXCOORD1;
	float3 T2 : TEXCOORD2;
	float3 T3 : TEXCOORD3;
};

// PASS: matches asm
VS_OUTPUT main(float4 v4 : TEXCOORD0, float4 v0 : POSITION0, float3 v1 : NORMAL0, float3 v2 : BINORMAL0, float3 v3 : TANGENT0)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	half4 a0, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;
	
	// (2) light vector ----------------------------------------------------------------------
	R_LIGHT_VECTOR.xyz = c_light_position.xyz + -V_POSITION;	
	R_LIGHT_VECTOR.w = c_one_half;	
	
	// (1) eye vector ------------------------------------------------------------------------
	R_EYE_VECTOR.xyz = c_eye_position.xyz + -V_POSITION;	
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(V_POSITION, c_world_view_projection);	
	
	// (10) output texcoords -----------------------------------------------------------------
	R_TEMP0.x = dot(V_TEXCOORD, c_base_map_xform_x);	// base map
	R_TEMP0.y = dot(V_TEXCOORD, c_base_map_xform_y);	
	o.T0.xy = (R_TEMP0.xy) * (c_bump_scale);	// bump map
	o.T1.xyz = (-R_LIGHT_VECTOR.xyz) * (c_light_half_inv_radius) + R_LIGHT_VECTOR.w;	
	o.T2.x = dot(R_EYE_VECTOR.rgb, V_TANGENT.rgb);	
	o.T2.y = dot(R_EYE_VECTOR.rgb, V_BINORMAL.rgb);	
	o.T2.z = dot(R_EYE_VECTOR.rgb, V_NORMAL.rgb);	
	o.T3.x = dot(R_LIGHT_VECTOR.rgb, V_TANGENT.rgb);	
	o.T3.y = dot(R_LIGHT_VECTOR.rgb, V_BINORMAL.rgb);	
	o.T3.z = dot(R_LIGHT_VECTOR.rgb, V_NORMAL.rgb);	
	
	// (1) output distance attenuation -------------------------------------------------------
	o.D1.xyzw = V_ZERO;	
	

	return o;
}

Technique environment_specular_light
{
	Pass P0
	{
		//VertexShader = compile TGT main();
		
		vertexshader = asm
		{
    vs_1_1
    dcl_position v0
    dcl_normal v1
    dcl_binormal v2
    dcl_tangent v3
    dcl_texcoord v4
    add r4.xyz, c13, -v0
    mov r4.w, c5.w
    add r5.xyz, c4, -v0
    dp4 oPos.x, v0, c0
    dp4 oPos.y, v0, c1
    dp4 oPos.z, v0, c2
    dp4 oPos.w, v0, c3
    dp4 r10.x, v4, c11
    dp4 r10.y, v4, c12
    mul oT0.xy, r10, c10.xyyy
    mad oT1.xyz, -r4, c13.w, r4.w
    dp3 oT2.x, r5, v3
    dp3 oT2.y, r5, v2
    dp3 oT2.z, r5, v1
    dp3 oT3.x, r4, v3
    dp3 oT3.y, r4, v2
    dp3 oT3.z, r4, v1
    mov oD1, v4.z

// approximately 18 instruction slots used
		};
	}
}