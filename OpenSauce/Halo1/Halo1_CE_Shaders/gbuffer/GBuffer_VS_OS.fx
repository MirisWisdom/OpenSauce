#define USE_ASM

struct VS_IN
{	
	float4	Position 		: POSITION;
	float4	Normal 			: NORMAL0;
	float4	BiNormal 		: BINORMAL0;
	float4	Tangent 		: TANGENT0;
	float4	TexCoord 		: TEXCOORD0;
	float2	BlendIndices 	: BLENDINDICES;
	float2	BlendWeights 	: BLENDWEIGHT;
};

struct VS_OUTPUT
{
	float4	Position 		: POSITION;
	float2	UV0    			: TEXCOORD0;
	float4	Position3D    	: TEXCOORD1;
	float4	PrevPosition3D  : TEXCOORD2;
    float3	Normal   		: TEXCOORD3;
    float3	BiNormal   		: TEXCOORD4;
    float3	Tangent   		: TEXCOORD5;
};

float4x4 c_world_view_projection				: register(c0);

float2 c_bump_scale 							: register(c10);
float4 c_base_map_xform_x 						: register(c11);
float4 c_base_map_xform_y 						: register(c12);

float4x3 c_node_matrices[64] 					: register(c29);

float4x4 c_prev_world_view_projection			: register(c222);
float4x3 c_normal_transform						: register(c226);
float c_linear_depth_multiplier 				: register(c229);

static const int k_normals_type_none = 0;
static const int k_normals_type_normal = 1;
static const int k_normals_type_tbn = 2;

VS_OUTPUT GBufferBSPVS(in VS_IN IN,
	uniform const int NormalType,
	uniform const bool bOutputVelocity)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;
	
	OUT.UV0.x = dot(IN.TexCoord, c_base_map_xform_x);
	OUT.UV0.y = dot(IN.TexCoord, c_base_map_xform_y);
	OUT.UV0 *= c_bump_scale;
	
	OUT.Position = mul(IN.Position, c_world_view_projection);
	
	OUT.Position3D = OUT.Position;
	OUT.Position3D.z *= c_linear_depth_multiplier;
	
	if(k_normals_type_none != NormalType)
	{
		OUT.Normal = mul(IN.Normal, c_normal_transform);
		if(k_normals_type_tbn == NormalType)
		{
			OUT.BiNormal = mul(IN.BiNormal, c_normal_transform);
			OUT.Tangent = mul(IN.Tangent, c_normal_transform);
		}
	}
	
	if(bOutputVelocity)
	{
		OUT.PrevPosition3D = mul(IN.Position, c_prev_world_view_projection);	
	}
	
	return OUT;
}

VS_OUTPUT GBufferObjectVS(in VS_IN IN,
	uniform const int NormalType,
	uniform const bool bOutputVelocity)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;
	
	OUT.UV0.x = dot(IN.TexCoord, c_base_map_xform_x);
	OUT.UV0.y = dot(IN.TexCoord, c_base_map_xform_y);	
	
	float4x3 WorldMatrix;	
	WorldMatrix = c_node_matrices[IN.BlendIndices.x] * IN.BlendWeights.x;
	WorldMatrix += c_node_matrices[IN.BlendIndices.y] * IN.BlendWeights.y;
		
	float4 WorldPosition;
	WorldPosition.xyz = mul(IN.Position, WorldMatrix);
	WorldPosition.w = IN.TexCoord.w;
	
	OUT.Position = mul(WorldPosition, c_world_view_projection);
	OUT.Position3D = OUT.Position;
	OUT.Position3D.z *= c_linear_depth_multiplier;
		
	if(k_normals_type_none != NormalType)
	{
		float3 Normal = mul(IN.Normal.xyz, WorldMatrix);
		OUT.Normal = mul(Normal, c_normal_transform);
		
		if(k_normals_type_tbn == NormalType)
		{
			float3 BiNormal = mul(IN.BiNormal.xyz, WorldMatrix);
			OUT.BiNormal = mul(BiNormal, c_normal_transform);
			
			float3 Tangent = mul(IN.Tangent.xyz, WorldMatrix);
			OUT.Tangent = mul(Tangent, c_normal_transform);
		}
	}
	
	if(bOutputVelocity)
	{
		//OUT.PrevPosition3D = mul(WorldPosition, c_prev_world_view_projection);	
	}
	
	return OUT;
}

technique VS_BSP_N
{
	pass P0
	{
		VertexShader = 
	#ifndef USE_ASM
		compile vs_2_0 GBufferBSPVS(k_normals_type_tbn, false);
	#else
		asm
		{
			#include "VS_BSP_N.inl"
		};
	#endif
	}
}

technique VS_BSP_N_V
{
	pass P0
	{
		VertexShader = 
	#ifndef USE_ASM
		compile vs_2_0 GBufferBSPVS(k_normals_type_tbn, true);
	#else
		asm
		{
			#include "VS_BSP_N_V.inl"
		};
	#endif
	}
}

technique VS_Object_N
{
	pass P0
	{
		VertexShader = 
	#ifndef USE_ASM
		compile vs_2_0 GBufferObjectVS(k_normals_type_normal, false);
	#else
		asm
		{
			#include "VS_Object_N.inl"
		};
	#endif
	}
}

technique VS_Object_TBN
{
	pass P0
	{
		VertexShader = 
	#ifndef USE_ASM
		compile vs_2_0 GBufferObjectVS(k_normals_type_tbn, false);
	#else
		asm
		{
			#include "VS_Object_TBN.inl"
		};
	#endif
	}
}

technique VS_Object_N_V
{
	pass P0
	{
		VertexShader = 
	#ifndef USE_ASM
		compile vs_2_0 GBufferObjectVS(k_normals_type_normal, false);
	#else
		asm
		{
			#include "VS_Object_N_V.inl"
		};
	#endif
	}
}

technique VS_Object_TBN_V
{
	pass P0
	{
		VertexShader = 
	#ifndef USE_ASM
		compile vs_2_0 GBufferObjectVS(true, false);
	#else
		asm
		{
			#include "VS_Object_TBN_V.inl"
		};
	#endif
	}
}

technique VS_Object_NONE
{
	pass P0
	{
		VertexShader = 
	#ifndef USE_ASM
		compile vs_2_0 GBufferObjectVS(k_normals_type_none, false);
	#else
		asm
		{
			#include "VS_Object_NONE.inl"
		};
	#endif
	}
}
