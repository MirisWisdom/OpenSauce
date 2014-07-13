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
#define V_ZERO								v4.z
#define V_ONE								v4.w
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
#define C_VIEWPROJ_X						c[VIEWPROJ_OFFSET+0]
#define C_VIEWPROJ_Y						c[VIEWPROJ_OFFSET+1]
#define C_VIEWPROJ_Z						c[VIEWPROJ_OFFSET+2]
#define C_VIEWPROJ_W						c[VIEWPROJ_OFFSET+3]
#define C_EYE_POSITION						c[VIEWPROJ_OFFSET+4]
#define C_TWO								c[VIEWPROJ_OFFSET+4].w
#define C_EYE_FORWARD						c[VIEWPROJ_OFFSET+5]
#define C_ONE_HALF							c[VIEWPROJ_OFFSET+5].w
#define C_EYE_XFORM_X						c[EYEXFORM_OFFSET+0] // w is unused
#define C_EYE_XFORM_Y						c[EYEXFORM_OFFSET+1]

// ===================================================================================================
// fog (after viewproj)
#define C_ATMOSPHERIC_FOG_GRADIENT			c[FOG_OFFSET+0]	// atmospheric fog density
#define C_PLANAR_FOG_GRADIENT1				c[FOG_OFFSET+1]	// planar fog density
#define C_PLANAR_FOG_GRADIENT2				c[FOG_OFFSET+2]	// planar fog eye distance
#define C_ATMOSPHERIC_FOG_MAXIMUM_DENSITY	c[FOG_OFFSET+3].x	// used for vertex fog only
#define C_PLANAR_FOG_EYE_DENSITY			c[FOG_OFFSET+3].y	// used for vertex fog only
#define C_PLANAR_FOG_MAXIMUM_DENSITY		c[FOG_OFFSET+3].z	// used for vertex fog only
#define C_NODE_INDEX_MULTIPLIER				c[FOG_OFFSET+3].w

// ===================================================================================================
// texscale (after fog)
#define C_ACTIVE_CAMO_REFRACTION			c[TEXSCALE_OFFSET+0].x
#define C_ACTIVE_CAMO_FALLOFF				c[TEXSCALE_OFFSET+0].y
#define C_BUMP_SCALE						c[TEXSCALE_OFFSET+0].xy
#define C_PRIMARY_DETAIL_SCALE				c[TEXSCALE_OFFSET+0].xy
#define C_SECONDARY_DETAIL_SCALE			c[TEXSCALE_OFFSET+0].zw
#define C_MIRROR_SCALE						c[TEXSCALE_OFFSET+0].zw
#define C_SELF_ILLUMINATION_SCALE			c[TEXSCALE_OFFSET+0].z
#define C_MIRROR_SCALE_U					c[TEXSCALE_OFFSET+0].z
#define C_MIRROR_SCALE_V					c[TEXSCALE_OFFSET+0].w
#define C_NORMAL_SCALE						c[TEXSCALE_OFFSET+0].w
#define C_FOG_SCREEN_GRADIENT				c[TEXSCALE_OFFSET+0]
#define C_BASE_MAP_XFORM_X					c[TEXSCALE_OFFSET+1]
#define C_BASE_MAP_XFORM_Y					c[TEXSCALE_OFFSET+2]
#define C_MICRO_DETAIL_SCALE_X				c[TEXSCALE_OFFSET+1].z
#define C_MICRO_DETAIL_SCALE_Y				c[TEXSCALE_OFFSET+2].z
#define C_TRANSLUCENCY						c[TEXSCALE_OFFSET+2].z

#define C_GLASS_ZERO						c[TEXSCALE_OFFSET+2].x
#define C_GLASS_ONE							c[TEXSCALE_OFFSET+2].y

// ===================================================================================================
// fakelight (after texscale)
#define C_FAKE_LIGHT0_DIRECTION				c[FAKELIGHT_OFFSET+0]
#define C_FAKE_LIGHT0_INTENSITY				c[FAKELIGHT_OFFSET+0].w
#define C_FAKE_LIGHT1_DIRECTION				c[FAKELIGHT_OFFSET+1]
#define C_FAKE_LIGHT1_INTENSITY				c[FAKELIGHT_OFFSET+1].w
#define C_FAKE_LIGHT_COLOR					c[FAKELIGHT_OFFSET+2] // w is free
#define C_FAKE_LIGHT_AMBIENT				c[FAKELIGHT_OFFSET+3] // w is free

// ===================================================================================================
// pointlight (after texscale)
#define C_LIGHT_POSITION					c[FAKELIGHT_OFFSET+0]
#define C_LIGHT_HALF_INV_RADIUS				c[FAKELIGHT_OFFSET+0].w
#define C_LIGHT_GEL_XFORM_X					c[FAKELIGHT_OFFSET+1]
#define C_LIGHT_GEL_XFORM_Y					c[FAKELIGHT_OFFSET+2]
#define C_LIGHT_GEL_XFORM_Z					c[FAKELIGHT_OFFSET+3]
#define C_LIGHT_GEL_XFORM_W					c[FAKELIGHT_OFFSET+4]

// ===================================================================================================
// texanim (after texscale)
#define C_TEXTURE0_XFORM_X					c[TEXANIM_OFFSET+0]
#define C_TEXTURE0_XFORM_Y					c[TEXANIM_OFFSET+1]
#define C_TEXTURE1_XFORM_X					c[TEXANIM_OFFSET+2]
#define C_TEXTURE1_XFORM_Y					c[TEXANIM_OFFSET+3]
#define C_TEXTURE2_XFORM_X					c[TEXANIM_OFFSET+4]
#define C_TEXTURE2_XFORM_Y					c[TEXANIM_OFFSET+5]
#define C_TEXTURE3_XFORM_X					c[TEXANIM_OFFSET+6]
#define C_TEXTURE3_XFORM_Y					c[TEXANIM_OFFSET+7]

// ===================================================================================================
// shadow (after texscale)
#define C_SHADOW_XFORM_X					c[SHADOW_OFFSET+0]
#define C_SHADOW_XFORM_Y					c[SHADOW_OFFSET+1]
#define C_SHADOW_FADE_FORWARD				c[SHADOW_OFFSET+2]
#define C_SHADOW_FADE_REVERSE				c[SHADOW_OFFSET+3]
#define C_SHADOW_AXIS						c[SHADOW_OFFSET+4]

// ===================================================================================================
// effect (after texscale)
#define C_PERPENDICULAR_TINT_COLOR			c[EFFECT_OFFSET+0]
#define C_PARALLEL_TINT_COLOR				c[EFFECT_OFFSET+1]

// ===================================================================================================
// vertexlight (after effect)
#define C_LIGHT0_POSITION					c[VERTEXLIGHT_OFFSET+0]
#define C_LIGHT0_INV_RADIUS_SQ				c[VERTEXLIGHT_OFFSET+0].w
#define C_LIGHT0_DIRECTION					c[VERTEXLIGHT_OFFSET+1]
#define C_LIGHT0_SPOT_A						c[VERTEXLIGHT_OFFSET+1].w
#define C_LIGHT0_COLOR						c[VERTEXLIGHT_OFFSET+2]
#define C_LIGHT0_SPOT_B						c[VERTEXLIGHT_OFFSET+2].w
// spot attenuation= (R_LIGHT_VECTOR dot C_LIGHT_DIRECTION)*SPOT_A + SPOT_B
// no spot attenuation if SPOT_A= 0 and SPOT_B= 1
#define C_LIGHT1_POSITION					c[VERTEXLIGHT_OFFSET+3]
#define C_LIGHT1_INV_RADIUS_SQ				c[VERTEXLIGHT_OFFSET+3].w
#define C_LIGHT1_DIRECTION					c[VERTEXLIGHT_OFFSET+4]
#define C_LIGHT1_SPOT_A						c[VERTEXLIGHT_OFFSET+4].w
#define C_LIGHT1_COLOR						c[VERTEXLIGHT_OFFSET+5]
#define C_LIGHT1_SPOT_B						c[VERTEXLIGHT_OFFSET+5].w
#define C_LIGHT2_DIRECTION					c[VERTEXLIGHT_OFFSET+6]		// w is free
#define C_LIGHT2_COLOR						c[VERTEXLIGHT_OFFSET+7]		// w is free
#define C_LIGHT3_DIRECTION					c[VERTEXLIGHT_OFFSET+8]		// w is free
#define C_LIGHT3_COLOR						c[VERTEXLIGHT_OFFSET+9]		// w is free
#define C_LIGHT_AMBIENT						c[VERTEXLIGHT_OFFSET+10]	// w is free

// ===================================================================================================
// detailobj (after vertexlight)
// comes after vertexlight because i have this ridiculous idea of incorporating dynamic lighting...
#define C_ONE_OVER_256						c[DETAILOBJ_OFFSET + 0].x	
#define C_16								c[DETAILOBJ_OFFSET + 0].y	
#define C_RANDOM							c[DETAILOBJ_OFFSET + 0].zw	
#define C_DETAIL_OBJECT_ONE					c[DETAILOBJ_OFFSET + 0].z	
#define C_DETAIL_OBJECT_CELL_SIZE			c[DETAILOBJ_OFFSET + 1].xyz	
#define C_DETAIL_OBJECT_OFFSET				c[DETAILOBJ_OFFSET + 1].w	// offsets screen-facing detail objects relative to N.E
#define C_DETAIL_OBJECT_CORNER_DATA			DETAILOBJ_OFFSET + 2
// { 0, 0,-.5, 0}
// {+1, 0,+.5, 0}
// {+1,+1,+.5,+1}
// { 0,+1,-.5,+1}
#define C_DETAIL_OBJECT_TYPE_DATA			DETAILOBJ_TYPEDATA_OFFSET
// .x (1/(cutoff_distance - falloff_distance))*(cutoff_distance)
// .y (1/(cutoff_distance - falloff_distance))*(-1)
// .z size x
// .w size y
#define C_DETAIL_OBJECT_FRAME_DATA			DETAILOBJ_FRAMEDATA_OFFSET
// .xy texcoord origin
// .zw texcoord extent
// ===================================================================================================
// nodematrix
#define C_NODE_XFORM_X						NODEMATRIX_OFFSET+0
#define C_NODE_XFORM_Y						NODEMATRIX_OFFSET+1
#define C_NODE_XFORM_Z						NODEMATRIX_OFFSET+2

// ===================================================================================================
// inverse
// (used for shaders that operate in something other than worldspace, eg. viewspace)
#define C_INVERSE_XFORM_X					c[INVERSE_OFFSET+0]
#define C_INVERSE_XFORM_Y					c[INVERSE_OFFSET+1]
#define C_INVERSE_XFORM_Z					c[INVERSE_OFFSET+2]

// ===================================================================================================
// screenproj
#define C_SCREENPROJ_X						c[SCREENPROJ_OFFSET+0]	// was c[91]
#define C_SCREENPROJ_Y						c[SCREENPROJ_OFFSET+1]	// was c[92]
#define C_SCREENPROJ_Z						c[SCREENPROJ_OFFSET+2]	// was c[93]
#define C_SCREENPROJ_W						c[SCREENPROJ_OFFSET+3]	// was c[94]
#define C_TEXTURE_SCALE						c[SCREENPROJ_OFFSET+4]	// was c[95] // we should use C_TEXSCALE instead

// ===================================================================================================
// screenproj2
#define C_SCREEN_TEXTURE_SCALES_1_2			c[SCREENPROJ2_OFFSET+0]
#define C_SCREEN_MASK_0_1					c[SCREENPROJ2_OFFSET+1]
#define C_SCREEN_MASK_2_OFF_0				c[SCREENPROJ2_OFFSET+2]
#define C_SCREEN_OFF_1_2					c[SCREENPROJ2_OFFSET+3]
#define C_SCREEN_SCALES_0_1					c[SCREENPROJ2_OFFSET+4]
#define C_SCREEN_SCALES_2					c[SCREENPROJ2_OFFSET+5]

// ===================================================================================================
#define C_ZSPRITE_PARAMS					c[ZSPRITE_OFFSET+0]
#define C_ZSPRITE_CAMERA_PLANE				c[ZSPRITE_OFFSET+1]

// ===================================================================================================
#define C_TINT_FACTOR						c[TINTFACTOR_OFFSET].x

// ===================================================================================================
#define C_DETAIL_OBJECT_CELL_ORIGIN			c[DETAILOBJ_CELL_ORIGIN_OFFSET]
#define C_DETAIL_OBJECT_XFORM_Z				c[DETAILOBJ_XFORM_Z_OFFSET]

// ===================================================================================================
// decal color
#define C_DECAL_COLOR						c[DECALCOLOR_OFFSET]

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

#define ZSPR_Q              C_ZSPRITE_PARAMS.x
#define ZSPR_NEG_Q_ZN       C_ZSPRITE_PARAMS.y
#define ZNEAR               C_ZSPRITE_PARAMS.w
#define PART_RAD            C_ZSPRITE_PARAMS.z

//#endif // __RASTERIZER_DX9_VERTEX_SHADERS_DEFS_H__
//
//#endif // pc
