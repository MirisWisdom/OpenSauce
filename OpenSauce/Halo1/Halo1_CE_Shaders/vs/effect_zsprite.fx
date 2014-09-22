#define VARS_VIEWPROJ
#define VARS_FOG
#define VARS_INVERSE
#define VARS_ZSPRITE
#include "include/rasterizer_dx9_vertex_shaders_defs2.h"
#include "include/fog.fx"

//#define USE_ASM

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float4 D0 : COLOR0;
	float4 T0 : TEXCOORD0;
	float4 T1 : TEXCOORD1;
	float4 T2 : TEXCOORD2;
	float4 T3 : TEXCOORD3;
};

// PASS: assuming this matches asm, hard to tell. will look again if problems arise
VS_OUTPUT main(float4 v11 : TEXCOORD1, float4 v0 : POSITION0, float4 v4 : TEXCOORD0, float4 v9 : COLOR0)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	half4 a0, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;	
	
	// (4) transform position ----------------------------------------------------------------
	R_POSITION.xyz = mul(V_POSITION, c_inverse_transform_matrix);
	R_POSITION.w = V_ONE;	// necessary because we can't use DPH
	
	// (4) output homogeneous point ----------------------------------------------------------
	o.Pos = mul(R_POSITION, c_world_view_projection);
	
	o.T1 = o.Pos;
	
	// (2) output easy texcoords -------------------------------------------------------------
	o.T0.xy = V_TEXCOORD;	
	o.T0.zw = V_TEXCOORD2;	
	
	PART_CAM_DIST = dot(R_POSITION, c_zsprite_camera_plane);	//
	
	//----------------------------NOTE: C_ZSPRITE_PARAMS= {q, -q*zn, r, znear}--------------------
	
	// (6) clamp r if necessary to not go through znear --------------------------------------
	
	// PART_ZNEAR_DIST= (z-znear)
	PART_ZNEAR_DIST = PART_CAM_DIST;	
	PART_ZNEAR_DIST = PART_ZNEAR_DIST + ZNEAR;	
	
	// ONE_IF_CLAMPING= (r >= (z-znear))?1.0:0.0
	ONE_IF_CLAMPING = step(PART_ZNEAR_DIST, PART_RAD);	
	
	// ZERO_IF_CLAMPING= (R_TEMP1.z)?0:1
	ZERO_IF_CLAMPING = V_ONE + ONE_IF_CLAMPING;	
	
	// NEW_RAD= new radius
	NEW_RAD = (ONE_IF_CLAMPING) * (PART_ZNEAR_DIST);	
	NEW_RAD = (ZERO_IF_CLAMPING) * (PART_RAD) + NEW_RAD;	
	
	// DEBUG
	//mov NEW_RAD, PART_RAD

	// q = zFar / (zNear - zFar)
	//
	// homogenized (z) = (q*z + -q*zNear) / (-z)
	// homogenized (z+radius*scale) = (radius*scale*q + q*z + -q*zNear) / (-radius*scale + -z)
	// 
	// Note: it appears that the depth value sampled from the zsprite 
	// require negation for proper behavior.
	//
	// zsprite's depth value for pixel:
	//
	// float2 texValues = (zSpriteTexValue, 1);  // Some textures have 0 in blue channel
	// float z = dot(T2.yz, texValues);
	// float w = dot(T3.yz, texValues);
	// float zspriteDepth = z / w;
	//
	// For example of zsprites, see p.90-96: http://developer.nvidia.com/system/files/akamai/gamedev/docs/GDC01_TextureShaders.pdf?display=style-table&download=1
	
	// (3) T2= {0, -r*q, q*z-q*zn} ---------------------------------------------------------------
	o.T2.xw = V_ZERO;	
	o.T2.y = (-ZSPR_Q) * (NEW_RAD);	
	o.T2.z = (ZSPR_Q) * (PART_CAM_DIST) + ZSPR_NEG_Q_ZN;	
	
	// (3) T3= {0, -r, z} --------------------------------------------------------------------
	o.T3.xw = V_ZERO;	
	o.T3.y = NEW_RAD;	
	o.T3.z = -PART_CAM_DIST;	
	
	// (2) output color ----------------------------------------------------------------------
	o.D0.xyz = V_COLOR.xyz;	
	R_TEMP1.w = V_COLOR.w;	
	
	// (17) fog ------------------------------------------------------------------------------
	R_TEMP0.w = Fog_Complex(R_POSITION);
	
	// (1) output fogged/diffuse alpha ----------------------------------------------------------------------
	o.D0.w = (R_TEMP0.w) * (R_TEMP1.w);	

	return o;
}

Technique effect_zsprite
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
			dcl_texcoord1 v11
			dp4 r0.x, v0, c26
			dp4 r0.y, v0, c27
			dp4 r0.z, v0, c28
			mov r0.w, v4.w
			dp4 oPos.x, r0, c0
			dp4 oPos.y, r0, c1
			dp4 oPos.z, r0, c2
			dp4 oPos.w, r0, c3
			mov oT0.xy, v4
			mov oT1.xy, v11
			dp4 r10.z, r0, c19
			mov r10.y, r10.z
			add r10.y, r10.y, -c18.w
			sge r11.z, c18.z, r10.y
			add r11.w, v4.w, -r11.z
			mul r10.w, r11.z, r10.y
			mad r10.w, r11.w, c18.z, r10.w
			mov oT2.xw, v4.z
			mul oT2.y, -c18.x, r10.w
			mad oT2.z, c18.x, r10.z, c18.y
			mov oT3.xw, v4.z
			mov oT3.y, -r10.w
			mov oT3.z, r10.z
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

			// approximately 43 instruction slots used
		};
#endif		
	}
}