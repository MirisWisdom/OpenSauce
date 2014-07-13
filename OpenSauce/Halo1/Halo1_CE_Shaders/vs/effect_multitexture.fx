#define VARS_VIEWPROJ
#define VARS_FOG
#define VARS_INVERSE
#define VARS_TEXANIM
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"
#include "include/fog.fx"

//#define USE_ASM

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float4 D0 : COLOR0;
	float4 T0 : TEXCOORD0;
	float3 T1 : TEXCOORD1;
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
	
	o.T1.z = o.Pos.z;
	o.T1.xy = ((o.Pos.xy / o.Pos.w) * 0.5f) - 0.5f;
	o.T1.y *= -1.0f;
	
	// (3) output texcoords ------------------------------------------------------------------
	o.T0.xy = V_TEXCOORD;	
	o.T0.z = dot(V_TEXCOORD, c_texture1_xform_x);	
	o.T0.w = dot(V_TEXCOORD, c_texture1_xform_y);	
	
	// (2) output color ----------------------------------------------------------------------
	o.D0.xyz = V_COLOR.xyz;	
	R_TEMP1.w = V_COLOR.w;	
	
	// (17) fog ------------------------------------------------------------------------------
	R_TEMP0.w = Fog_Complex(R_POSITION);	
	
	// (1) output fogged/diffuse alpha ----------------------------------------------------------------------
	o.D0.w = (R_TEMP0.w) * (R_TEMP1.w);	

	return o;
}

Technique effect_multitexture
{
	Pass P0
	{
#ifndef USE_ASM
		VertexShader = compile TGT main();
#else
		vertexshader = asm
		{
			vs_1_1
			dcl_position v0
			dcl_texcoord v4
			dcl_color v9
			dp4 r0.x, v0, c26
			dp4 r0.y, v0, c27
			dp4 r0.z, v0, c28
			mov r0.w, v4.w
			dp4 oPos.x, r0, c0
			dp4 oPos.y, r0, c1
			dp4 oPos.z, r0, c2
			dp4 oPos.w, r0, c3
			mov oT0.xy, v4
			dp4 oT1.x, v4, c15
			dp4 oT1.y, v4, c16
			mov oD0.xyz, v9
			mov r11.w, v9
			dp4 r8.x, r0, c7
			dp4 r8.y, r0, c8
			dp4 r8.z, r0, c6
			add r8.xy, v4.w, -r8
			max r8.xyz, r8, v4.z
			mul r8.xy, r8, r8
			min r8.xyz, r8, v4.w
			add r8.x, r8.x, r8.y
			min r8.x, r8, v4.w
			add r8.xy, v4.w, -r8
			mul r8.xy, r8, r8
			add r8.y, r8.y, -r8.x
			mad r8.w, c9.y, r8.y, r8.x
			mul r8.w, r8.w, c9.z
			mul r8.z, r8.z, c9.x
			add r8, -r8, v4.w
			mul r10.w, r8.z, r8.w
			mul oD0.w, r10.w, r11.w

			// approximately 31 instruction slots used
		};
#endif
	}
}