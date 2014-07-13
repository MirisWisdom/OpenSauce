#include "includes\vertex_shaders.fx"

texture t_result : TEXSCENE;

sampler2D s_result =
sampler_state
{
	Texture = <t_result>;
	AddressU = Clamp;
	AddressV = Clamp;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = Point;
};

uniform extern float 	c_fade_amount;

float4 		FadePS( float2 Tex0 : TEXCOORD0 ) : COLOR0
{
	return float4(tex2D(s_result, Tex0).xyz, c_fade_amount);
}

technique PostProcess
{
	pass EffectFade
	{		
		VertexShader = compile vs_1_1 PassThroughVS();
		PixelShader = compile ps_2_0 FadePS();
	}
}