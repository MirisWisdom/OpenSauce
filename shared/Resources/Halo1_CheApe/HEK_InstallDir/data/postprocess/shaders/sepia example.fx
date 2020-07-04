#include "includes\vertex_shaders.fx"

texture tex_Scene	: TEXSCENE;
texture tex_Source	: TEXSOURCE;

sampler2D sam_Source =
sampler_state
{
	Texture = <tex_Source>;
	AddressU = Clamp;
	AddressV = Clamp;
	MinFilter = Point;
	MagFilter = Linear;
	MipFilter = Linear;
};

float Desaturation = 0.5f;
float Tone = 1.0f;
float4 LightColor = { 1.0f, 0.9f, 0.5f, 1.0f };
float4 DarkColor = { 0.2f, 0.05f, 0.0f, 1.0f };

float4 SepiaPS( float2 Tex0 : TEXCOORD0 ) : COLOR0{
	float3 color = LightColor.rgb * tex2D(sam_Source, Tex0).xyz;
	float3 grey_ratio = float3(0.3,0.59,0.11);
	float color_interp = dot(grey_ratio, color);
	float3 muted = lerp(color, color_interp, Desaturation);
	float3 sepia = lerp(DarkColor.rgb, LightColor, color_interp);
	float3 result = lerp(muted, sepia, Tone);
	return float4(result, 1.0f);
}	   

technique Sepia
<
	int shader_model_mask = shader_model_mask_1_0 | shader_model_mask_2_0 | shader_model_mask_3_0;	
>
{
	pass Sepia
	{
		VertexShader = compile vs_1_1 PassThroughVS();
		PixelShader = compile ps_2_0 SepiaPS();
	}
}
