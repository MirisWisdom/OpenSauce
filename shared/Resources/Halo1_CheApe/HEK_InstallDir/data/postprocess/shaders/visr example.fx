#define GBUFFER_USE_DEPTH
#define GBUFFER_USE_NORMALS
#define GBUFFER_USE_INDEX

#include "includes\vertex_shaders.fx"
#include "includes\gbuffer_functions.fx"

texture tex_Scene		: TEXSCENE;
texture tex_Source		: TEXSOURCE;

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

uniform extern float2 PixelSize			: PIXELSIZE;
uniform extern float FarClipDistance	: FARCLIPDISTANCE;

#define DIST 1.25f
float2 PixelKernel[4] =
{
	{0,  	DIST},
	{DIST,	0},
	{0, 	-DIST},
	{-DIST,	0},
};

float3 TeamColours[4] = 
{
	{0.4,0.6,0.35}, //Ally
	{0.8,0.0,0.1}, //Enemy
	{0.1,0.2,0.15}, //Ally
	{0.2,0.0,0.0}, //Enemy
};

float3 TypeColours[15] =
{
	{0.4,0.4,0.2}, //FP models
	{1.0,1.0,1.0}, //Sky
	{0.35,0.3,0.25}, //BSP
	{0.0,0.0,0.0}, //Biped (use team colour)
	{0.2,0.4,0.7}, //Vehicle
	{0.2,0.2,0.35}, //Weapon
	{0.1,0.1,0.4}, //Equipment
	{0.35,0.3,0.25}, //Garbage
	{0.8,0.2,0.1}, //Projectile
	{0.35,0.3,0.25}, //Scenery
	{0.35,0.3,0.25}, //Machine
	{0.35,0.3,0.25}, //Control
	{0.35,0.3,0.25}, //Light Fixture
	{0.35,0.3,0.25}, //Placeholder
	{0.35,0.3,0.25}, //Sound Scenery
};

uniform extern float Distance = 38.0f;

// Returns the pixels color depending on what mesh type and whether the object is an enemy
float3 GetColor(in int type, in int team, in int is_enemy, in int is_dead)
{
	float3 result = 0;
	
	// if the type is a vehicle
	if(type == 4)
	{
		// and the vehicle has a team, use the ally or enemy color, otherwise use the vehicle color
		if( team >= 1)
			result = TeamColours[is_enemy];
		else
			result = TypeColours[type];
	}
	// if the type is a biped, use the enemy or ally color
	else if(type == 3)
	{
		if(is_dead)
			result = TeamColours[is_enemy + 2];
		else
			result = TeamColours[is_enemy];
	}
	// otherwise just use the type color
	else
		result = TypeColours[type];		
		
	return result; 
}

float DetectEdge(float2 Tex0, int centre_type)
{	
	float3 centre_normal = GetNormals(Tex0);
	
	float pixel_depth = GetDepth(Tex0);	
	pixel_depth *= FarClipDistance;
	
	float falloff = pixel_depth / Distance;
	falloff = min(falloff, 1.0f);
	falloff = max(falloff, 0.0f);
			
	float result = 0;

	for( int i = 0; i < 4; i++ )
	{	
		float2 sample_coords = Tex0 + (PixelKernel[i] * PixelSize);
			
		float3 sample_normal = GetNormals(sample_coords);
			
		int2 index = GetIndex(sample_coords);
		int sample_type = GetType(index);
		
		if(sample_type == centre_type)
			result += saturate(1 - dot(centre_normal.xyz, sample_normal));
		else 
		{
			result = 1;
			break;
		}
	}
	
	return result * (1 - falloff);
}

float4 VISR_PS(float2 Tex0 : TEXCOORD0):COLOR0
{
	float3 scene = tex2D(sam_Source, Tex0).xyz;
	
	int2 index = GetIndex(Tex0);
	int type = GetType(index);
	int team = GetTeam(index);
	int is_enemy = GetIsEnemy(index);
	int is_dead = GetIsDead(index);
	
	if(type == 1)
		return float4(scene, 1);
	
	return clamp(0, 1, float4(lerp(scene, GetColor(type, team, is_enemy, is_dead), DetectEdge(Tex0, type)), 1));
}

technique VISR
<
	int shader_model_mask = shader_model_mask_3_0;
>
{
	pass p0
	{				
		VertexShader = compile vs_3_0 PassThroughVS();
		PixelShader = compile ps_3_0 VISR_PS();
	}
}
