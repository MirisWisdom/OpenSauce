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

float gDesat = 0.5f;
float gToned = 1.0f;
float4 gLightColor = { 1.0f, 0.9f, 0.5f, 1.0f };
float4 gDarkColor = { 0.2f, 0.05f, 0.0f, 1.0f };

float4 SepiaPS( float2 Tex0 : TEXCOORD0 ) : COLOR0{
	float3 scnColor = gLightColor.rgb * tex2D(sam_Source, Tex0).xyz;
	float3 grayXfer = float3(0.3,0.59,0.11);
	float gray = dot(grayXfer,scnColor);
	float3 muted = lerp(scnColor,gray.xxx,gDesat);
	float3 sepia = lerp(gDarkColor.rgb,gLightColor,gray);
	float3 result = lerp(muted,sepia,gToned);
	return float4(result,1.0f);
}	   

technique PostProcess
{
	pass Sepia
	{
		VertexShader = compile vs_1_1 PassThroughVS();
		PixelShader = compile ps_2_0 SepiaPS();
	}
}
