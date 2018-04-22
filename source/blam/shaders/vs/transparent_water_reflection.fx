#define VARS_VIEWPROJ
#define VARS_FOG
#define VARS_TEXSCALE
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"
#include "include/fog.fx"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float2 T0 : TEXCOORD0;
	float4 T1 : TEXCOORD1;
	float4 T2 : TEXCOORD2;
	float4 T3 : TEXCOORD3;
	float  Fog : FOG;
};

// PASS: matches asm
VS_OUTPUT main(float4 v4 : TEXCOORD0, float4 v0 : POSITION0, float3 v1 : NORMAL0, float3 v2 : BINORMAL0, float3 v3 : TANGENT0)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	half4 a0, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;
	
	// (1) eye vector ------------------------------------------------------------------------
	R_EYE_VECTOR.xyz = -V_POSITION.xyz + c_eye_position;
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(V_POSITION, c_world_view_projection);	
	
	// (13) output texcoords -----------------------------------------------------------------
	o.T0.xy = (V_TEXCOORD * c_primary_detail_scale) + c_secondary_detail_scale;	// bump map
	o.T1.x = V_TANGENT.x;	
	o.T1.y = V_BINORMAL.x;	
	o.T1.z = V_NORMAL.x;	
	o.T2.x = V_TANGENT.y;	
	o.T2.y = V_BINORMAL.y;	
	o.T2.z = V_NORMAL.y;	
	o.T3.x = V_TANGENT.z;	
	o.T3.y = V_BINORMAL.z;	
	o.T3.z = V_NORMAL.z;	
	o.T1.w = R_EYE_VECTOR.x;	
	o.T2.w = R_EYE_VECTOR.y;	
	o.T3.w = R_EYE_VECTOR.z;	
	
	// (2) atmospheric fog ----------------------------------------------------------------
	R_ATMOSPHERIC_FOG_DENSITY = dot(V_POSITION, c_atmospheric_fog_gradient);	
	o.Fog = V_ONE + R_ATMOSPHERIC_FOG_DENSITY;	

	return o;
}

Technique transparent_water_reflection
{
	Pass P0
	{
		VertexShader = compile TGT main();
		/*vertexshader = asm
		{
    vs_1_1
    dcl_position v0
    dcl_normal v1
    dcl_binormal v2
    dcl_tangent v3
    dcl_texcoord v4
    add r5.xyz, -v0, c4
    dp4 oPos.x, v0, c0
    dp4 oPos.y, v0, c1
    dp4 oPos.z, v0, c2
    dp4 oPos.w, v0, c3
    mad oT0.xy, v4, c10.xyyy, c10.zwww
    mov oT1.x, v3.x
    mov oT1.y, v2.x
    mov oT1.z, v1.x
    mov oT2.x, v3.y
    mov oT2.y, v2.y
    mov oT2.z, v1.y
    mov oT3.x, v3.z
    mov oT3.y, v2.z
    mov oT3.z, v1.z
    mov oT1.w, r5.x
    mov oT2.w, r5.y
    mov oT3.w, r5.z
    dp4 r8.z, v0, c6
    add oFog, v4.w, -r8.z

// approximately 20 instruction slots used
		};*/
	}
}