#define VARS_VIEWPROJ
#define VARS_POINTLIGHT
#define VARS_FOG
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float3 T0 : TEXCOORD0;
	float3 T1 : TEXCOORD1;
	float Fog : FOG;
};

// PASS: matches asm
VS_OUTPUT main(float4 v4 : TEXCOORD0, float4 v0 : POSITION0, float3 v1 : NORMAL0, float3 v2 : BINORMAL0, float3 v3 : TANGENT0)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	half4 a0, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;
	
	// (2) light vector ----------------------------------------------------------------------
	R_LIGHT_VECTOR.xyz = c_light_position.xyz + -V_POSITION;	
	R_LIGHT_VECTOR.w = c_one_half;	
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(V_POSITION, c_world_view_projection);	
	
	// (10) output texcoords -----------------------------------------------------------------
	o.T0.x = dot(R_LIGHT_VECTOR.rgb, c_light_gel_xform_x.rgb);	
	o.T0.y = dot(R_LIGHT_VECTOR.rgb, c_light_gel_xform_y.rgb);	
	o.T0.z = dot(R_LIGHT_VECTOR.rgb, c_light_gel_xform_z.rgb);	
	o.T1.xyz = (R_LIGHT_VECTOR.xyz) * (-c_light_half_inv_radius) + R_LIGHT_VECTOR.w;	
		
	// output fog
	R_ATMOSPHERIC_FOG_DENSITY = dot(V_POSITION, c_atmospheric_fog_gradient) * c_atmospheric_fog_max_density;
	o.Fog = V_ONE + -R_ATMOSPHERIC_FOG_DENSITY;
	
	return o;
}

Technique environment_diffuse_light_ff
{
	Pass P0
	{
		VertexShader = compile TGT main();
	}
}