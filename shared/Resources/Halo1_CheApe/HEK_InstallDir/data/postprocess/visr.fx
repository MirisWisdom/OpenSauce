#define GBUFFER_USE_DEPTH
#define GBUFFER_USE_NORMALS
#define GBUFFER_USE_INDEX

#include "includes\vertex_shaders.fx"
#include "includes\gbuffer_functions.fx"

texture tex_Scene		: TEXSCENE;
texture tex_Source		: TEXSOURCE;

sampler2D sam_Scene = 
sampler_state
{
	Texture = <tex_Scene>;
	AddressU = Clamp;
	AddressV = Clamp;
	MinFilter = Point;
	MagFilter = Linear;
	MipFilter = Linear;
};
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

uniform extern float2 PixelSize		: PIXELSIZE;
uniform extern float FarClipDistance	: FARCLIPDISTANCE;

#define DIST 1.5f
float2 PixelKernel[4] =
{
	{0,  	DIST},
	{DIST,	0},
	{0, 	-DIST},
	{-DIST,	0},
};

float3 TeamColours[12] = 
{
	{0.1,0.3,0.1}, //NULL team (not an object)
	{0.5,0.1,0.1}, //No Team
	{0.1,0.2,0.1}, //Player
	{0.1,0.5,0.1}, //Human
	{0.5,0.0,0.1}, //Covenant
	{0.5,0.2,0.0}, //Flood
	{0.4,0.2,0.2}, //Sentinel
	{0.0,0.0,0.0}, //Not Used
	{0.0,0.0,0.0}, //Not Used
	{0.2,0.4,0.7}, //Multiplayer No Team
	{1.0,0.0,0.0}, //Multiplayer Red Team
	{0.0,0.0,1.0}, //Multiplayer Blue Team
	//List could go on up to 19 since in multiplayer
	//the team index is offset by 9 so that multiplayer 
	//team colours can be used
};

float3 TypeColours[15] =
{
	{0.4,0.4,0.2}, //FP models
	{1.0,1.0,1.0}, //Sky
	{0.3,0.3,0.05}, //BSP
	{0.0,0.0,0.0}, //Biped (use team colour)
	{0.2,0.4,0.7}, //Vehicle
	{0.2,0.2,0.35}, //Weapon
	{0.1,0.1,0.4}, //Equipment
	{0.2,0.2,0.05}, //Garbage
	{0.8,0.2,0.1}, //Projectile
	{0.2,0.2,0.05}, //Scenery
	{0.2,0.2,0.05}, //Machine
	{0.05,0.05,0.2}, //Control
	{0.2,0.2,0.05}, //Light Fixture
	{0.2,0.2,0.05}, //Placeholder
	{0.2,0.2,0.05}, //Sound Scenery
};

float3 GetColor(in int Type, in int Team)
{
	float3 OUTColor = 0;
	
	if(Type == 4)
	{
		if( Type == 4 && Team == 1 )
			OUTColor = TypeColours[Type];
		else if(Type == 4 && Team >= 1)
			OUTColor = TeamColours[Team];
	}
	else if(Type == 3)
		OUTColor = TeamColours[Team];
	else
		OUTColor = TypeColours[Type];		
		
	return OUTColor; 
}

float3 LuminanceConv = { 0.2125f, 0.7154f, 0.0721f };
float Hither = 0.0f;
float Yon = 28.0f;
float DetectEdge(float2 Tex0, int CentreType)
{	
	float3 Orig = GetNormals(Tex0);
	
	float sampDepth = GetDepth(Tex0);	
	sampDepth *= FarClipDistance;
	float sampdl = ((sampDepth - Hither) / (Yon - Hither));
	sampdl = min(sampdl,1);
	sampdl = max(sampdl,0.0);
	sampdl = pow(sampdl,1);
			
	float Sum = 0;

	for( int i = 0; i < 4; i++ )
	{	
		float2 sample_coords = Tex0 + (PixelKernel[i] * PixelSize);
			
		float3 samp = GetNormals(sample_coords);
			
		int Type;
		GetType(sample_coords, Type);
		
		if(Type == CentreType)
			Sum += saturate(1 - dot( Orig.xyz, samp ));
		else 
		{
			Sum = 1;
			break;
		}
	}
	
	return Sum * (1 - sampdl);
}

float4 VISR(float2 Tex0 : TEXCOORD0):COLOR0
{
	float3 scene = tex2D(sam_Scene, Tex0).xyz;
	float3 color = float3(0.6,0.5,0.1);
	int Type, Team;
	GetTypeTeam(Tex0, Type, Team);
	if(Type == 1)
		return float4(scene, 1);
	
	return clamp(0, 1, float4(lerp(scene,GetColor(Type, Team),DetectEdge(Tex0, Type)), 1));
}

technique PostProcess
{
	pass p0
	{				
		VertexShader = compile vs_3_0 PassThroughVS();
		PixelShader = compile ps_3_0 VISR();
	}
}
