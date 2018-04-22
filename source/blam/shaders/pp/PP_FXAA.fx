#include "includes\vertex_shaders.fx"

#define FXAA_GREEN_AS_LUMA 1
#define FXAA_PC 1
#define FXAA_HLSL_3 1
#define FXAA_QUALITY__PRESET 39
//#define FXAA_QUALITY__PRESET 12

#include "includes\Fxaa3_11.h"
uniform extern texture gScreenTexture	: TEXSCENE;
uniform extern texture gLumaTexture		: TEXSOURCE;

uniform extern float rcp_width = 0.001;
uniform extern float rcp_height = 0.001;

sampler screenSampler = sampler_state
{
    Texture = <gScreenTexture>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = POINT;
    AddressU = BORDER;
    AddressV = BORDER;
    SRGBTexture = FALSE;
};
sampler lumaSampler = sampler_state
{
    Texture = <gLumaTexture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
    AddressU = BORDER;
    AddressV = BORDER;
    SRGBTexture = FALSE;
};
float4 LumaShader( float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 c0 = tex2D(screenSampler,Tex);
    c0.w = dot(c0.xyz,float3(0.299, 0.587, 0.114)); //store luma in alpha
    //c0.w = sqrt(dot(c0.xyz,float3(0.299, 0.587, 0.114))); //store luma in alpha
    return c0;
}

float4 FXAA( float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 c0 = FxaaPixelShader(
		Tex, //pos
		0, //fxaaConsolePosPos (?)
		lumaSampler, //tex
		lumaSampler, //fxaaConsole360TexExpBiasNegOne
		lumaSampler, //fxaaConsole360TexExpBiasNegTwo
		float2(rcp_width, rcp_height), //fxaaQualityRcpFrame
		float4(-0.5*rcp_width,-0.5*rcp_height,0.5*rcp_width,0.5*rcp_height), //fxaaConsoleRcpFrameOpt
		float4(-2.0*rcp_width,-2.0*rcp_height,2.0*rcp_width,2.0*rcp_height), //fxaaConsoleRcpFrameOpt2
		float4(8.0*rcp_width,8.0*rcp_height,-4.0*rcp_width,-4.0*rcp_height), //fxaaConsole360RcpFrameOpt2
		0.50, //fxaaQualitySubpix (default: 0.75)
		0.166, //fxaaQualityEdgeThreshold
		0.0833, //fxaaQualityEdgeThresholdMin
		8.0, //fxaaConsoleEdgeSharpness
		0.125, //fxaaConsoleEdgeThreshold
		0.05, //fxaaConsoleEdgeThresholdMin
		0 //fxaaConsole360ConstDir
	);
    c0.w = 1;
    return c0;
}


technique PostProcess_sm3_0
{
    pass Luma
    {
		VertexShader = compile vs_3_0 PassThroughVS();
        PixelShader = compile ps_3_0 LumaShader();
	}
    pass FXAA
    {
		VertexShader = compile vs_3_0 PassThroughVS();
        PixelShader = compile ps_3_0 FXAA();
    }
}