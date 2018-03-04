#define VARS_VIEWPROJ
#define VARS_DETAILOBJ
#define VARS_FOG
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"

#define USE_ASM

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float4 D0 : COLOR0;
	float2 T0 : TEXCOORD0;
	float Fog : FOG;
};

// PASS: matches asm
VS_OUTPUT main(float4 v8 : COLOR1, float4 v0 : POSITION0, float4 v9 : COLOR0)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	half4 a0, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;
	
	// (7) lookup tables --------------------------------------------------------------------
	R_TEMP0 = (v8) * (c_one_over_256);	
	a0.x = R_TEMP0.x;	
	R_DETAIL_OBJECT_FRAME_DATA = c_detail_object_frame_data[a0.x];	
	a0.x = R_TEMP0.y;	
	R_DETAIL_OBJECT_TYPE_DATA = c_detail_object_type_data[a0.x];	
	a0.x = R_TEMP0.z;	
	R_DETAIL_OBJECT_CORNER_DATA = c_detail_object_frame_data[a0.x];	
	
	// (1) position within cell --------------------------------------------------------------
	R_POSITION = V_POSITION;	
	
	// (4) offset position -------------------------------------------------------------------
	R_TEMP0.w = c_16;	
	R_TEMP0.w = max(-c_eye_forward.z, R_TEMP0.w);	
	R_TEMP0.w = (R_TEMP0.w) * (R_DETAIL_OBJECT_TYPE_DATA.w);	// bitmap height
	R_TEMP0.w = (R_TEMP0.w) * (R_DETAIL_OBJECT_FRAME_DATA.w);	// sprite height
	R_POSITION.z = (c_detail_object_offset) * (R_TEMP0.w) + R_POSITION;	
	
	// (3) fade ------------------------------------------------------------------------------
	// PERF: 1 instruction would suffice (dp4) if we had a whole 4d vector available
	R_EYE_VECTOR.xyz = R_POSITION.xyz + -c_eye_position;	
	R_TEMP0.w = dot(R_EYE_VECTOR.rgb, c_eye_forward.rgb);	
	
	// (4) vertex position -------------------------------------------------------------------
	// corner data .zw is {-1/2,0}, {1/2,0}, {1/2,1}, {-1/2,1}
	// PERF: we could remove a multiply if we premultiplied sprite bounds by type->size
	R_TEMP0.xy = (R_DETAIL_OBJECT_TYPE_DATA.zw) * (R_DETAIL_OBJECT_CORNER_DATA.zw);	// adjust for corner, size
	R_TEMP0.xy = (R_TEMP0.xy) * (R_DETAIL_OBJECT_FRAME_DATA.zw);	// adjust for sprite bounds
	R_POSITION.xyz = (c_eye_xform_x.xyz) * (R_TEMP0.x) + R_POSITION;	// horizontal
	R_POSITION.xyz = (c_eye_xform_y.xyz) * (R_TEMP0.y) + R_POSITION;	// vertical
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(R_POSITION, c_world_view_projection);	
	
	// (1) output texcoords ------------------------------------------------------------------
	o.T0.xy = (R_DETAIL_OBJECT_CORNER_DATA.xy) * (R_DETAIL_OBJECT_FRAME_DATA.zw) + R_DETAIL_OBJECT_FRAME_DATA.xy;	
	
	// (1) output color ----------------------------------------------------------------------
	o.D0.xyz = V_COLOR;	
	o.D0.w = (R_TEMP0.w) * (R_DETAIL_OBJECT_TYPE_DATA.y) + R_DETAIL_OBJECT_TYPE_DATA.x;	
	
	// output fog
	R_ATMOSPHERIC_FOG_DENSITY = dot(R_POSITION, c_atmospheric_fog_gradient) * c_atmospheric_fog_max_density;
	o.Fog = c_one_over_256 + -R_FOG_DENSITY;
	
	return o;
}

Technique detail_object_type0
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
			dcl_color v9
			dcl_color1 v8
			mul r10, v8, c13.x
			mov a0.x, r10.x
			mov r7, c29[a0.x]
			mov a0.x, r10.y
			mov r8, c19[a0.x]
			mov a0.x, r10.z
			mov r9, c15[a0.x]
			mov r0, v0
			mov r10.w, c13.y
			max r10.w, -c5.z, r10.w
			mul r10.w, r10.w, r8.w
			mul r10.w, r10.w, r7.w
			mad r0.z, c14.w, r10.w, r0
			add r5.xyz, r0, -c4
			dp3 r10.w, r5, c5
			mul r10.xy, r8.zwww, r9.zwww
			mul r10.xy, r10, r7.zwww
			mad r0.xyz, c27, r10.x, r0
			mad r0.xyz, c28, r10.y, r0
			dp4 oPos.x, r0, c0
			dp4 oPos.y, r0, c1
			dp4 oPos.z, r0, c2
			dp4 oPos.w, r0, c3
			mad oT0.xy, r9.xyyy, r7.zwww, r7.xyyy
			mov oD0.xyz, v9
			mad oD0.w, r10.w, r8.y, r8.x
			dp4 r8.z, r0, c6
			mul r8.z, r8.z, c9.x
			add oFog, c13.z, -r8.z

		// approximately 29 instruction slots used
		};
#endif
	}
}