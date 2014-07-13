#ifndef MODEL_SHARED_VARIABLES_FX
#define MODEL_SHARED_VARIABLES_FX

#define PS_CONSTANT_OFFSET 5

static const int detail_function_biased_multiply	= 0;
static const int detail_function_multiply			= 1;
static const int detail_function_biased_add			= 2;

sampler TexSampler0 								: register(s[0]);
sampler TexSampler1 								: register(s[1]);
sampler TexSampler2 								: register(s[2]);
sampler TexSampler3 								: register(s[3]);
sampler TexSampler4 								: register(s[4]);
sampler TexSampler5 								: register(s[5]);
sampler TexSampler6 								: register(s[6]);
sampler TexSampler7 								: register(s[7]);

float4 c_primary_change_color						: register(c[0]);
float4 c_fog_color_correction_0						: register(c[1]);
float4 c_fog_color_correction_E						: register(c[2]);
float4 c_fog_color_correction_1						: register(c[3]);
float4 c_self_illumination_color					: register(c[4]);

float4 c_base_normal_map_vars						: register(c[5]);
#define c_base_normal_map_coefficient				c_base_normal_map_vars.x
#define c_base_normal_map_z_coefficient				c_base_normal_map_vars.y
#define c_spec_color_exponent_mask_toggle			c_base_normal_map_vars.z

float4 c_detail_normal_map_vars						: register(c[6]);
#define c_detail_normal_map_1_interpolation			c_detail_normal_map_vars.x
#define c_detail_normal_map_2_interpolation			c_detail_normal_map_vars.y
#define c_detail_normal_map_1_coefficient			c_detail_normal_map_vars.z
#define c_detail_normal_map_2_coefficient			c_detail_normal_map_vars.w

float4 c_specular_vars								: register(c[7]);
#define c_specular_color_map_exponent				c_specular_vars.x
#define c_specular_color_map_coefficient			c_specular_vars.y
#define c_specular_lighting_exponent				c_specular_vars.z
#define c_specular_lighting_coefficient				c_specular_vars.w

float4 	c_eye_view_position							: register(c[9]);

float4 	c_object_parallel_colour					: register(c[10]);
float4 	c_object_perpendicular_colour				: register(c[11]);

float4 	c_object_point_light0_position				: register(c[12]);
float4 	c_object_point_light0_rotation				: register(c[13]);
float4 	c_object_point_light0_colour				: register(c[14]);

float4 	c_object_point_light1_position				: register(c[15]);
float4 	c_object_point_light1_rotation				: register(c[16]);
float4 	c_object_point_light1_colour				: register(c[17]);

float4 	c_object_distant_light0_vector				: register(c[18]);
float4 	c_object_distant_light0_colour				: register(c[19]);

float4 	c_object_distant_light1_vector				: register(c[20]);
float4 	c_object_distant_light1_colour				: register(c[21]);

float3 	c_object_ambient_colour						: register(c[22]);
#endif