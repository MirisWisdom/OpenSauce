/*
RASTERIZER_DX8_VERTEX_SHADERS_DEFS.H
Tuesday January 23, 2001 6:26:39 PM
*/

//#include "cseries_windows.h"
//#ifdef pc
//
//#ifndef __RASTERIZER_DX9_VERTEX_SHADERS_DEFS_H__
//#define __RASTERIZER_DX9_VERTEX_SHADERS_DEFS_H__
//#pragma once
#include "include/rasterizer_dx9_vsh_constants.h"

#define dph									dp4

// ===================================================================================================
// vertex input registers
#define V_POSITION							v0
#define V_NORMAL							v1
#define V_BINORMAL							v2
#define V_TANGENT							v3
#define V_TEXCOORD							v4
#define V_ZERO								0
#define V_ONE								1
#define V_NODE_INDICES						v5
#define V_NODE0_INDEX						v5.x
#define V_NODE1_INDEX						v5.y
#define V_NODE2_INDEX						v5.z
#define V_NODE3_INDEX						v5.w
#define V_NODE_WEIGHTS						v6
#define V_NODE0_WEIGHT						v6.x
#define V_NODE1_WEIGHT						v6.y
#define V_NODE2_WEIGHT						v6.z
#define V_NODE3_WEIGHT						v6.w
#define V_INCIDENT_RADIOSITY				v7
#define V_LIGHTMAP_TEXCOORD					v8
#define V_COLOR								v9
#define V_TEXCOORD2							v11

// ===================================================================================================
// viewproj
#ifdef VARS_VIEWPROJ
float4x4	c_world_view_projection 			: register(c[VIEWPROJ_OFFSET + 0]);
float4		c_eye_position						: register(c[VIEWPROJ_OFFSET + 4]);
#define		c_two								c_eye_position.w
float4		c_eye_forward						: register(c[VIEWPROJ_OFFSET + 5]);
#define		c_one_half							c_eye_forward.w
float4 		c_eye_xform_x						: register(c[EYEXFORM_OFFSET + 0]);
float4		c_eye_xform_y						: register(c[EYEXFORM_OFFSET + 1]);
#endif

// ===================================================================================================
// fog (after viewproj)
#ifdef VARS_FOG
float4		c_atmospheric_fog_gradient			: register(c[FOG_OFFSET + 0]);	// atmospheric fog density
float4		c_planar_fog_gradient1 				: register(c[FOG_OFFSET + 1]);	// planar fog density
float4		c_planar_fog_gradient2				: register(c[FOG_OFFSET + 2]);	// planar fog eye distance
float4		c_fog_densities						: register(c[FOG_OFFSET + 3]);
#define		c_atmospheric_fog_max_density		c_fog_densities.x
#define		c_planar_fog_eye_density			c_fog_densities.y
#define		c_planar_fog_max_density			c_fog_densities.z
#define		c_node_index_multiplier				c_fog_densities.w
#endif

// ===================================================================================================
// texscale (after fog)
#ifdef VARS_TEXSCALE
float4		c_fog_screen_gradient				: register(c[TEXSCALE_OFFSET + 0]);
#define		c_active_camo_refraction			c_fog_screen_gradient.x
#define		c_active_camo_falloff				c_fog_screen_gradient.y
#define 	c_bump_scale						c_fog_screen_gradient.xy
#define 	c_primary_detail_scale				c_fog_screen_gradient.xy
#define 	c_secondary_detail_scale			c_fog_screen_gradient.zw
#define 	c_mirror_scale						c_fog_screen_gradient.zw
#define 	c_self_illumination_scale			c_fog_screen_gradient.z
#define 	c_mirror_scale_u					c_fog_screen_gradient.z
#define 	c_mirror_scale_v					c_fog_screen_gradient.w
#define 	c_normal_scale						c_fog_screen_gradient.w
float4 		c_base_map_xform_x					: register(c[TEXSCALE_OFFSET + 1]);
float4 		c_base_map_xform_y					: register(c[TEXSCALE_OFFSET + 2]);
#define 	c_micro_detail_scale_x				c_base_map_xform_x.z
#define 	c_micro_detail_scale_y				c_base_map_xform_y.z
#define 	c_translucency						c_base_map_xform_y.z

#define		c_glass_zero						c_base_map_xform_y.x
#define		c_glass_one							c_base_map_xform_y.y
#endif

// ===================================================================================================
// normalsscale (after texscale)
#ifdef VARS_NORMALSSCALE
float4		c_detail_normal_scales				: register(c[NORMALSSCALE_OFFSET + 0]);
#define		c_detail_normal0_scale				c_detail_normal_scales.xy
#define		c_detail_normal1_scale				c_detail_normal_scales.zw
#endif

// ===================================================================================================
// fakelight (after texscale)
#ifdef VARS_FAKELIGHT
float4 		c_fake_light0_direction				: register(c[FAKELIGHT_OFFSET + 0]);
#define 	c_fake_light0_intensity				c_fake_light0_direction.w
float4 		c_fake_light1_direction				: register(c[FAKELIGHT_OFFSET + 1]);
#define 	c_fake_light1_intensity				c_fake_light1_direction.w
float4 		c_fake_light_color					: register(c[FAKELIGHT_OFFSET + 2]); // w is free
float4 		c_fake_light_ambient				: register(c[FAKELIGHT_OFFSET + 3]); // w is free
#endif

// ===================================================================================================
// pointlight (after texscale)
#ifdef VARS_POINTLIGHT
float4 		c_light_position					: register(c[POINTLIGHT_OFFSET + 0]);
#define 	c_light_half_inv_radius				c_light_position.w
float4 		c_light_gel_xform_x					: register(c[POINTLIGHT_OFFSET + 1]);
float4 		c_light_gel_xform_y					: register(c[POINTLIGHT_OFFSET + 2]);
float4 		c_light_gel_xform_z					: register(c[POINTLIGHT_OFFSET + 3]);
float4 		c_light_gel_xform_w					: register(c[POINTLIGHT_OFFSET + 4]);
#endif

// ===================================================================================================
// texanim (after texscale)
#ifdef VARS_TEXANIM
float4	 	c_texture0_xform_x					: register(c[TEXANIM_OFFSET + 0]);
float4 		c_texture0_xform_y					: register(c[TEXANIM_OFFSET + 1]);
float4 		c_texture1_xform_x					: register(c[TEXANIM_OFFSET + 2]);
float4 		c_texture1_xform_y					: register(c[TEXANIM_OFFSET + 3]);
float4 		c_texture2_xform_x					: register(c[TEXANIM_OFFSET + 4]);
float4 		c_texture2_xform_y					: register(c[TEXANIM_OFFSET + 5]);
float4 		c_texture3_xform_x					: register(c[TEXANIM_OFFSET + 6]);
float4 		c_texture3_xform_y					: register(c[TEXANIM_OFFSET + 7]);
#endif

// ===================================================================================================
// shadow (after texscale)
#ifdef VARS_SHADOW
float4 		c_shadow_xform_x					: register(c[SHADOW_OFFSET+0]);
float4 		c_shadow_xform_y					: register(c[SHADOW_OFFSET+1]);
float4 		c_shadow_fade_forward				: register(c[SHADOW_OFFSET+2]);
float4 		c_shadow_fade_reverse				: register(c[SHADOW_OFFSET+3]);
float4 		c_shadow_axis						: register(c[SHADOW_OFFSET+4]);
#endif

// ===================================================================================================
// effect (after texscale)
#ifdef VARS_EFFECT
float4 		c_perpendicular_tint_color			: register(c[EFFECT_OFFSET + 0]);
float4 		c_parallel_tint_color				: register(c[EFFECT_OFFSET + 1]);
#endif

// ===================================================================================================
// vertexlight (after effect)
#ifdef VARS_VERTEXLIGHT
float4 		c_light0_position					: register(c[VERTEXLIGHT_OFFSET + 0]);
#define 	c_light0_inv_radius_sq				c_light0_position.w
float4 		c_light0_direction					: register(c[VERTEXLIGHT_OFFSET + 1]);
#define 	c_light0_spot_a						c_light0_direction.w
float4 		c_light0_color						: register(c[VERTEXLIGHT_OFFSET + 2]);
#define 	c_light0_spot_b						c_light0_color.w
// spot attenuation= (R_LIGHT_VECTOR dot C_LIGHT_DIRECTION)*SPOT_A + SPOT_B
// no spot attenuation if SPOT_A= 0 and SPOT_B= 1
float4 		c_light1_position					: register(c[VERTEXLIGHT_OFFSET + 3]);
#define 	c_light1_inv_radius_sq				c_light1_position.w
float4 		c_light1_direction					: register(c[VERTEXLIGHT_OFFSET + 4]);
#define 	c_light1_spot_a						c_light1_direction.w
float4 		c_light1_color						: register(c[VERTEXLIGHT_OFFSET + 5]);
#define 	c_light1_spot_b						c_light1_color.w
float4 		c_light2_direction					: register(c[VERTEXLIGHT_OFFSET + 6]);		// w is free
float4 		c_light2_color						: register(c[VERTEXLIGHT_OFFSET + 7]);		// w is free
float4 		c_light3_direction					: register(c[VERTEXLIGHT_OFFSET + 8]);		// w is free
float4 		c_light3_color						: register(c[VERTEXLIGHT_OFFSET + 9]);		// w is free
float4 		c_light_ambient						: register(c[VERTEXLIGHT_OFFSET + 10]);	// w is free
#endif

// ===================================================================================================
// detailobj (after vertexlight)
#ifdef VARS_DETAILOBJ
// comes after vertexlight because i have this ridiculous idea of incorporating dynamic lighting...
float4		c_detailobj							: register(c[DETAILOBJ_OFFSET + 0]);
float4		c_detailobj_cell					: register(c[DETAILOBJ_OFFSET + 1]);
#define 	c_one_over_256						c_detailobj.x	
#define 	c_16								c_detailobj.y	
#define 	c_random							c_detailobj.zw	
#define 	c_detail_object_one					c_detailobj.z	
#define 	c_detail_object_cell_size			c_detailobj_cell.xyz	
#define 	c_detail_object_offset				c_detailobj_cell.w	// offsets screen-facing detail objects relative to N.E
float4	 	c_detail_object_corner_data			: register(c[DETAILOBJ_OFFSET + 2]);
// { 0, 0,-.5, 0}
// {+1, 0,+.5, 0}
// {+1,+1,+.5,+1}
// { 0,+1,-.5,+1}
float4		c_detail_object_type_data			: register(c[DETAILOBJ_TYPEDATA_OFFSET]);
// .x (1/(cutoff_distance - falloff_distance))*(cutoff_distance)
// .y (1/(cutoff_distance - falloff_distance))*(-1)
// .z size x
// .w size y
float4		c_detail_object_frame_data			: register(c[DETAILOBJ_FRAMEDATA_OFFSET]);
// .xy texcoord origin
// .zw texcoord extent
#endif

// ===================================================================================================
// nodematrix
#ifdef VARS_NODEMATRIX
float4x3	c_node_matrices[22] 				: register(c[NODEMATRIX_OFFSET + 0]);
#endif

// ===================================================================================================
// inverse
// (used for shaders that operate in something other than worldspace, eg. viewspace)
#ifdef VARS_INVERSE
float4x3	c_inverse_transform_matrix			: register(c[INVERSE_OFFSET + 0]);
#endif

// ===================================================================================================
// screenproj
#ifdef VARS_SCREENPROJ
float4x4	c_screenproj 						: register(c[SCREENPROJ_OFFSET + 0]);
float4		c_texture_scale						: register(c[SCREENPROJ_OFFSET + 4]);	// was c[95] // we should use C_TEXSCALE instead
#endif

// ===================================================================================================
// screenproj2
#ifdef VARS_SCREENPROJ2
float4		c_screen_texture_scales_1_2			: register(c[SCREENPROJ2_OFFSET + 0]);
float4 		c_screen_mask_0_1					: register(c[SCREENPROJ2_OFFSET + 1]);
float4 		c_screen_mask_2_off_0				: register(c[SCREENPROJ2_OFFSET + 2]);
float4 		c_screen_off_1_2					: register(c[SCREENPROJ2_OFFSET + 3]);
float4 		c_screen_scales_0_1					: register(c[SCREENPROJ2_OFFSET + 4]);
float4 		c_screen_scales_2					: register(c[SCREENPROJ2_OFFSET + 5]);
#endif

// ===================================================================================================
#ifdef VARS_ZSPRITE
float4		c_zsprite_params					: register (c[ZSPRITE_OFFSET + 0]);
float4 		c_zsprite_camera_plane				: register (c[ZSPRITE_OFFSET + 1]);
#endif

// ===================================================================================================
#ifdef VARS_TINT
float4 		c_tint								: register (c[TINTFACTOR_OFFSET]);
#define		c_tint_factor						c_tint.x
#endif

// ===================================================================================================
#ifdef VARS_DETAILOBJ
#define C_DETAIL_OBJECT_CELL_ORIGIN			c[DETAILOBJ_CELL_ORIGIN_OFFSET]
#define C_DETAIL_OBJECT_XFORM_Z				c[DETAILOBJ_XFORM_Z_OFFSET]
#endif

// ===================================================================================================
// decal color
#ifdef VARS_DECAL 
float4 c_decal_color							: register(c[DECALCOLOR_OFFSET]);
#endif

// ===================================================================================================
// geometry registers
#define R_POSITION							r0
#define R_NODE_INDICES						r0
#define R_NODE0_INDEX						r0.x
#define R_NODE1_INDEX						r0.y
#define R_NODE2_INDEX						r0.z
#define R_NODE3_INDEX						r0.w
#define R_NORMAL							r1
#define R_BINORMAL							r2
#define R_TANGENT							r3
#define R_XFORM_X							r4 // not used after position, normal, binormal and tangent are computed
#define R_XFORM_Y							r5 // not used after position, normal, binormal and tangent are computed
#define R_XFORM_Z							r6 // not used after position, normal, binormal and tangent are computed

#define R_LIGHT_VECTOR						r4
#define R_N_dot_E							r4.w
#define R_EYE_VECTOR						r5
#define R_ONE_MINUS_N_dot_E					r5.w
#define R_REFLECTION_VECTOR					r6
#define R_TWO_N_dot_E_MINUS_ONE				r6.w

// ===================================================================================================
// vertex light registers
#define R_LIGHT_SUM							r7
#define R_LIGHT_ATTENUATION0				r8
#define R_LIGHT_ATTENUATION1				r9
// we set up the attenuation factors like this
// so we can multiply them more efficiently...
#define R_LIGHT0_DIST_ATTEN					r8.x
#define R_LIGHT0_ANGL_ATTEN					r9.x
#define R_LIGHT0_SPOT_ATTEN					r8.z
#define R_LIGHT1_DIST_ATTEN					r8.y
#define R_LIGHT1_ANGL_ATTEN					r9.y
#define R_LIGHT1_SPOT_ATTEN					r9.z
#define R_LIGHT2_ATTEN						r8.w
#define R_LIGHT3_ATTEN						r9.w

// ===================================================================================================
// lightmap registers
#define R_INCIDENT_RADIOSITY				r7
#define R_FAKE_LIGHT_SUM					r7

// ===================================================================================================
// fog registers
#define R_FOG_DENSITY						r8
#define R_PLANAR_FOG_VERTEX_DENSITY			r8.x
#define R_PLANAR_FOG_EYE_DISTANCE			r8.y
#define R_ATMOSPHERIC_FOG_DENSITY			r8.z
#define R_PLANAR_FOG_DENSITY				r8.w

// ===================================================================================================
// tint registers
#define R_TINT_COLOR						r7

// ===================================================================================================
// detailobj registers
#define R_DETAIL_OBJECT_FRAME_DATA			r7
//	.xy texcoord origin
//	.zw texcoord extent
#define R_DETAIL_OBJECT_TYPE_DATA			r8
//	.x (1/(cutoff_distance - falloff_distance))*(cutoff_distance)
//	.y (1/(cutoff_distance - falloff_distance))*(-1)
//	.z size_min :: world units times map size over sprite size
//	.w size_max - size_min
#define R_DETAIL_OBJECT_CORNER_DATA			r9
//	.xy {-1/2,0}, {1/2,0}, {1/2,1}, {-1/2,1}
//	.zw {0,0}, {1,0}, {1,1}, {0,1}

// ===================================================================================================
// temp registers
#define R_TEMP0								r10
#define R_TEMP1								r11

// ===================================================================================================
// zsprite bullshit
#define PART_ZNEAR_DIST     R_TEMP0.y
#define NEW_RAD             R_TEMP0.w
#define PART_CAM_DIST       R_TEMP0.z

#define ONE_IF_CLAMPING     R_TEMP1.z
#define ZERO_IF_CLAMPING    R_TEMP1.w

#ifdef VARS_ZSPRITE
#define ZSPR_Q              c_zsprite_params.x
#define ZSPR_NEG_Q_ZN       c_zsprite_params.y
#define ZNEAR               c_zsprite_params.w
#define PART_RAD            c_zsprite_params.z
#endif

//#endif // __RASTERIZER_DX9_VERTEX_SHADERS_DEFS_H__
//
//#endif // pc
