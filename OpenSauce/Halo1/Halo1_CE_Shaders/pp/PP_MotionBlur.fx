#define GBUFFER_USE_VELOCITY
#define GBUFFER_USE_INDEX
#define VERTEX_SCALE_SHADER

#include "includes\vertex_shaders.fx"
#include "includes\gbuffer_functions.fx"

#define BLUR_SAMPLES 12

texture t_scene					: TEXSCENE;
texture t_source				: TEXSOURCE;

sampler2D s_scene =
sampler_state
{
    Texture = <t_scene>;
    AddressU = Clamp;
    AddressV = Clamp;
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Point;
};
sampler2D s_source =
sampler_state
{
    Texture = <t_source>;
    AddressU = Mirror;
    AddressV = Mirror;
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Point;
};

uniform extern float 		c_blur_amount;
uniform extern float 		c_vignette_amount;

float4 MaskPass(float2 Tex0 : TEXCOORD0) : COLOR0
{
	float3 color = tex2D(s_scene, Tex0).rgb;

	int2 index = GetIndex(Tex0);
	int type = GetType(index);
	
	//sky = 1
	//bsp = 2
	//scenery = 9	
	
	if(type == 1 || type == 2 || type == 9)
	{
		float2 origin_tex = Tex0 - 0.5f;
		origin_tex *= 1.5f;
		float vignette	= (1 - dot(origin_tex, origin_tex));
		vignette = 1.0f - (0.5f * pow(vignette, 4.0)) * c_vignette_amount;
		
		return float4(color, vignette);
	}
	else
		return float4(color, 0);
}

float4 MotionBlurPS(float2 Tex0 : TEXCOORD0) : COLOR0
{	
	float2 current_velocity = clamp(GetVelocity(Tex0), -1.0, 1.0) * float2(1.0f, -1.0f);	
	float radius = c_blur_amount / BLUR_SAMPLES;
	
	float2 uv_multiplier = radius * current_velocity;
	
	float4 base_color = tex2D(s_source, Tex0);
	
	if(base_color.w == 0.0f)
		return float4(base_color.rgb, 1);
		
	float3 blur_color = base_color.rgb;
	
	float half_samples = BLUR_SAMPLES / 2;
	float sample_count = 1;
	for( float i = -half_samples; i <= half_samples; i++)
	{
		float2 sample_coords = min(Tex0 + (i * uv_multiplier), 1.0f);
		
		float4 sample = tex2D(s_source, sample_coords);
		if(sample.w == 0.0f)
			continue;
			
		float multiplier = half_samples - abs(i);
		blur_color += sample.rgb * multiplier;
		sample_count += multiplier;
	}
	
    return float4(lerp(base_color, blur_color / sample_count, base_color.w), 1.0f);
}

technique PostProcess
{
	pass Mask
	{
		VertexShader = compile vs_3_0 PassThroughVS();
		PixelShader = compile ps_3_0 MaskPass();
	}
	pass Blur
	{
		VertexShader = compile vs_3_0 PassThroughVS();
		PixelShader = compile ps_3_0 MotionBlurPS();
	}
}