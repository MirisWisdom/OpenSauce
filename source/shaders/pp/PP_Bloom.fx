#define VERTEX_SCALE_SHADER
#include "..\includes\vertex_shaders.fx"

texture t_source : TEXSOURCE;
texture t_scene : TEXSCENE;
texture t_buffer : TEXBUFFER;

sampler2D s_source =
sampler_state
{
    Texture = <t_source>;
    AddressU = Mirror;
    AddressV = Mirror;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};
sampler2D s_scene =
sampler_state
{
    Texture = <t_scene>;
    AddressU = Mirror;
    AddressV = Mirror;
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Point;
};

uniform extern float2 		c_pixelsize		: PIXELSIZE;

uniform extern float 			c_bloom_size 			= 3.5f;
uniform extern float 			c_bloom_exposure 		= 1.0f;
uniform extern float 			c_bloom_mix_amount 		= 0.5f;

uniform extern float3 		c_bloom_minimum_color  	= {0.85,0.85,0.85};
uniform extern float3 		c_bloom_maximum_color  	= {1.0,1.0,1.0};

	
float4 Squisher( float2 Tex0 : TEXCOORD0 ) : COLOR0
{
    float3 Range = 		c_bloom_maximum_color - c_bloom_minimum_color;
	
    float3 Color = 		tex2D(s_source, Tex0).rgb;
    Color = 			min(c_bloom_maximum_color, Color);
    Color = 			max(c_bloom_minimum_color, Color);
    Color = 			(Color - c_bloom_minimum_color) / Range;
	
    return float4(clamp(0, 1, Color), 1);
}


float2 c_poisson_array[12] = {	
	float2(-0.326212f, -0.40581f),
	float2(-0.840144f, -0.07358f),
	float2(-0.695914f, 0.457137f),
	float2(-0.203345f, 0.620716f),
	float2(0.96234f, -0.194983f),
	float2(0.473434f, -0.480026f),
	float2(0.519456f, 0.767022f),
	float2(0.185461f, -0.893124f),
	float2(0.507431f, 0.064425f),
	float2(0.89642f, 0.412458f),
	float2(-0.32194f, -0.932615f),
	float2(-0.791559f, -0.59771f)
};
float4 PoissonBlur(float2 Tex0 : TEXCOORD0) : COLOR0
{
	float2 Size = (c_pixelsize * c_bloom_size);
	float3 Color = tex2D(s_source, Tex0).rgb;
	for(int i = 0; i < 12; i++)
	{
		float2 tex = Tex0 + ((c_poisson_array[i]) * Size);
		Color += tex2D(s_source, tex).rgb;
	}
	return float4((Color / 13.0f), 1);
}


static const float2 k_gaussian_weights[13] = 
{
    {-6.0, 0.002216},
    {-5.0, 0.008764},
    {-4.0, 0.026995},
    {-3.0, 0.064759},
    {-2.0, 0.120985},
    {-1.0, 0.176033},
    { 0.0, 0.199471},
    { 1.0, 0.176033},
    { 2.0, 0.120985},
    { 3.0, 0.064759},
    { 4.0, 0.026995},
    { 5.0, 0.008764},
    { 6.0, 0.002216},
};
float4 GaussianBlur(float2 Tex0 : TEXCOORD0, uniform const float2 Direction) : COLOR0
{
	float3 Color = 0;
	for(int i = 0; i < 13; i++)
	{
		float2 offset = ((k_gaussian_weights[i].x * 0.5f) * Direction * c_bloom_size) * c_pixelsize;
		float2 tex = Tex0 + offset;
		Color += tex2D(s_source, tex).rgb * k_gaussian_weights[i].y;
	}
	return float4(Color, 1);
}


float4 CompositePS(float2 Tex0 : TEXCOORD0, float2 Tex1 : TEXCOORD1) : COLOR0
{
	float3 Scene = 		tex2D(s_scene, Tex1).rgb;
    float3 Bloom = 		tex2D(s_source, Tex0).rgb * 0.5f;

    return float4(lerp(Scene, Scene + (Bloom * c_bloom_exposure), c_bloom_mix_amount), 1.0f);
}

#define OFFSET 0.01f
#define SHRINKSIZE 8.0f
technique PostProcess_sm3_0
{
    pass BrightPass
    {
		VertexShader = compile vs_3_0 ScaleVS((1.0f / SHRINKSIZE) + OFFSET, 1.0f + (SHRINKSIZE * OFFSET));
        PixelShader = compile ps_3_0 Squisher();
    }
    pass BlurPass1
    {
		VertexShader = compile vs_3_0 ScaleVS((1.0f / SHRINKSIZE) + OFFSET, (1.0f + (SHRINKSIZE * OFFSET)) / SHRINKSIZE );
		PixelShader = compile ps_3_0 PoissonBlur();
    }
    pass BlurPass2
    {
		VertexShader = compile vs_3_0 ScaleVS((1.0f / SHRINKSIZE) + OFFSET, (1.0f + (SHRINKSIZE * OFFSET)) / SHRINKSIZE );
		PixelShader = compile ps_3_0 PoissonBlur();
    }
    pass Composite
    {
		VertexShader = compile vs_3_0 ScaleVS(1.0f , 1.0f / SHRINKSIZE);
        PixelShader = compile ps_3_0 CompositePS();
    }
}

technique PostProcess_sm2_0
{
    pass BrightPass
    {
		VertexShader = compile vs_2_0 ScaleVS((1.0f / SHRINKSIZE) + OFFSET, 1.0f + (SHRINKSIZE * OFFSET));
        PixelShader = compile ps_2_0 Squisher();
    }
    pass BlurPass1
    {
		VertexShader = compile vs_2_0 ScaleVS((1.0f / SHRINKSIZE) + OFFSET, (1.0f + (SHRINKSIZE * OFFSET)) / SHRINKSIZE );
		PixelShader = compile ps_2_0 GaussianBlur(float2(0,1));
    }
    pass BlurPass2
    {
		VertexShader = compile vs_2_0 ScaleVS((1.0f / SHRINKSIZE) + OFFSET, (1.0f + (SHRINKSIZE * OFFSET)) / SHRINKSIZE );
		PixelShader = compile ps_2_0 GaussianBlur(float2(1,0));
    }
    pass Composite
    {
		VertexShader = compile vs_2_0 ScaleVS(1.0f , 1.0f / SHRINKSIZE);
        PixelShader = compile ps_2_0 CompositePS();
    }
}