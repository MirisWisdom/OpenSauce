#ifndef SHARED_VARIABLES_FX
#define SHARED_VARIABLES_FX

#include "rasterizer_dx9_vsh_constants.h"

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
float4		c_atmospheric_fog_gradient			: register(c[FOG_OFFSET + 0]);
float4		c_planar_fog_gradient1 				: register(c[FOG_OFFSET + 1]);
float4		c_planar_fog_gradient2				: register(c[FOG_OFFSET + 2]);
float4		c_fog_densities						: register(c[FOG_OFFSET + 3]);
#define		c_atmospheric_fog_max_density		c_fog_densities.x
#define		c_planar_fog_eye_density			c_fog_densities.y
#define		c_planar_fog_max_density			c_fog_densities.z
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
float4 		c_light_position					: register(c[FAKELIGHT_OFFSET + 0]);
#define 	c_light_half_inv_radius				c_light_position.w
float4 		c_light_gel_xform_x					: register(c[FAKELIGHT_OFFSET + 1]);
float4 		c_light_gel_xform_y					: register(c[FAKELIGHT_OFFSET + 2]);
float4 		c_light_gel_xform_z					: register(c[FAKELIGHT_OFFSET + 3]);
float4 		c_light_gel_xform_w					: register(c[FAKELIGHT_OFFSET + 4]);
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

#endif