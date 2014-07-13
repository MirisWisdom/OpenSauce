#ifndef SHARED_VARIABLES_FX
#define SHARED_VARIABLES_FX

#include "rasterizer_dx9_vsh_constants.h"

// ===================================================================================================
// viewproj
#ifdef VARS_VIEWPROJ
float4x4	c_world_view_projection 			: WorldViewProjection;
float4		c_eye_position						: POSITION
<
	string Object = "Camera0";
>;
#define		c_two								2.0f
//float4		c_eye_forward						: register(c[VIEWPROJ_OFFSET + 5]);
#define		c_one_half							0.5f
//float4 		c_eye_xform_x						: register(c[EYEXFORM_OFFSET + 0]);
//float4		c_eye_xform_y						: register(c[EYEXFORM_OFFSET + 1]);
#endif

// ===================================================================================================
// fog (after viewproj)
#ifdef VARS_FOG
float4		c_atmospheric_fog_gradient;
float4		c_planar_fog_gradient1;
float4		c_planar_fog_gradient2;
float4		c_fog_densities;
#define		c_atmospheric_fog_max_density		c_fog_densities.x
#define		c_planar_fog_eye_density			c_fog_densities.y
#define		c_planar_fog_max_density			c_fog_densities.z
#endif

// ===================================================================================================
// texscale (after fog)
#ifdef VARS_TEXSCALE
float4		c_fog_screen_gradient				= float4(1, 1, 1, 1);
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
float4 		c_base_map_xform_x					= float4(1, 0, 0, 1);
float4 		c_base_map_xform_y					= float4(0, 1, 0, 1);
#define 	c_micro_detail_scale_x				c_base_map_xform_x.z
#define 	c_micro_detail_scale_y				c_base_map_xform_y.z
#define 	c_translucency						c_base_map_xform_y.z

#define		c_glass_zero						c_base_map_xform_y.x
#define		c_glass_one							c_base_map_xform_y.y
#endif

// ===================================================================================================
// normalsscale (after texscale)
#ifdef VARS_NORMALSSCALE
float4		c_detail_normal_scales;
#define		c_detail_normal0_scale				c_detail_normal_scales.xy
#define		c_detail_normal1_scale				c_detail_normal_scales.zw
#endif

// ===================================================================================================
// fakelight (after texscale)
#ifdef VARS_FAKELIGHT
float4 		c_fake_light0_direction				: DIRECTION
<
	string Object = "DirectionalLight0";
>;
float 		c_fake_light0_intensity				: CONSTANTATTENUATION
<
	string Object = "DirectionalLight0";
>;
float4 		c_fake_light1_direction				: DIRECTION
<
	string Object = "DirectionalLight1";
>;
float	 	c_fake_light1_intensity				: CONSTANTATTENUATION
<
	string Object = "DirectionalLight1";
>;
float4 		c_fake_light_color					: COLOR
<
	string Object = "DirectionalLight0";
>;
float4 		c_fake_light_ambient : AMBIENT <
    string UIName =  "Ambient Light";
    string UIWidget = "Color";
> = {0.07f,0.07f,0.07f, 0.07f};
#endif

// ===================================================================================================
// pointlight (after texscale)
#ifdef VARS_POINTLIGHT
float4 		c_light_position					: POSITION
<
	string Object = "PointLight0";
>;
#define 	c_light_half_inv_radius				c_light_position.w
float4 		c_light_gel_xform_x;
float4 		c_light_gel_xform_y;
float4 		c_light_gel_xform_z;
float4 		c_light_gel_xform_w;
#endif

// ===================================================================================================
// texanim (after texscale)
#ifdef VARS_TEXANIM
#endif

#ifdef VARS_EFFECT
// ===================================================================================================
// effect (after texscale)
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
float4 		c_light_ambient						: AMBIENT;
#endif

// ===================================================================================================
// nodematrix
#ifdef VARS_NODEMATRIX
float4x3	c_node_matrices[22];
#endif

// ===================================================================================================
// inverse
// (used for shaders that operate in something other than worldspace, eg. viewspace)
#ifdef VARS_INVERSE
float4x3	c_inverse_transform_matrix			: register(c[INVERSE_OFFSET + 0]);
#endif

#endif