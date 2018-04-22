struct VS_INPUT{
	float4 Position 			: POSITION0;
	float4 Normal 				: NORMAL0;
	float4 BiNormal 			: BINORMAL0;
	float4 Tangent 				: TANGENT0;
	float4 TexCoord0 			: TEXCOORD0;
	float4 BlendIndices 		: BLENDINDICES;
	float4 BlendWeights			: BLENDWEIGHT;
};

struct VS_OUTPUT{
 	float4 Position 			: POSITION0;
 	float  Fog					: FOG;
 	float3 Position3D			: TEXCOORD0;
 	float4 DiffuseMultiUV		: TEXCOORD1;
 	float4 DetailUV 			: TEXCOORD2;
	float3x3 TBNTranspose 		: TEXCOORD3;
	float4 NormalDetailUV		: TEXCOORD6; 
};

float CalculatePlanarFog(float3 world_position,
	half4 planar_fog_gradient1, half4 planar_fog_gradient2,
	half planar_fog_eye_density, half planar_fog_max_density)
{
	half PlanarFogVertexDensity = dot(world_position, planar_fog_gradient1.xyz) + planar_fog_gradient1.w;   // x
	half PlanarFogEyeDistance = dot(world_position, planar_fog_gradient2.xyz) + planar_fog_gradient2.w;   // y
	half2 FogDensity = 1.0f + half2(-PlanarFogVertexDensity, -PlanarFogEyeDistance);   // {1 - x, 1 - y}
	FogDensity.xy = max(FogDensity.xy, 0.0f);   // clamp to zero
	FogDensity.xy = pow(FogDensity, 2.0f);   // {(1 - x)^2, (1 - y)^2}
	FogDensity.xy = min(FogDensity.xy, 1.0f);   // clamp to one
	FogDensity.x = FogDensity.x + FogDensity.y;   // (1 - x)^2 + (1 - y)^2
	FogDensity.x = min(FogDensity.x, 1.0f);   // clamp to one
	FogDensity.xy = 1.0f + -FogDensity;   // {1 - (1 - x)^2 - (1 - y)^2, 1 - (1 - y)^2}
	FogDensity.xy = pow(FogDensity, 2.0f);   // {(1 - (1 - x)^2 - (1 - y)^2)^2, (1 - (1 - y)^2)^2}
	FogDensity.y = FogDensity.y + -FogDensity.x;  
	
	float PlanarFogDensity = planar_fog_eye_density * FogDensity.y + FogDensity.x;
	PlanarFogDensity *= planar_fog_max_density;
	
	return PlanarFogDensity;   // Pf
}


VS_OUTPUT ModelVS(in VS_INPUT IN,
	uniform bool bSingleBone,
	uniform bool bFogged)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;
	
	float2 Indices = IN.BlendIndices + c_one_half;

	// build the transform matrix for skinning
	float4x3 WorldMatrix;
	if(!bSingleBone)
	{
		WorldMatrix = c_node_matrices[Indices.x] * IN.BlendWeights.x;
		WorldMatrix += c_node_matrices[Indices.y] * IN.BlendWeights.y;
	}
	else
		WorldMatrix = c_node_matrices[0];
		
	// calculate the world space position of the vertex
	float4 WorldPosition;
	WorldPosition.xyz = mul(IN.Position, WorldMatrix);
	WorldPosition.w = IN.TexCoord0.w;
	OUT.Position3D =  WorldPosition.xyz;
	
	// transform normals
	float3x3 WorldToTangentSpace;
	WorldToTangentSpace[0] = mul(IN.Tangent,	(float3x3)WorldMatrix);
	WorldToTangentSpace[1] = mul(IN.BiNormal,	(float3x3)WorldMatrix);
	WorldToTangentSpace[2] = mul(IN.Normal,		(float3x3)WorldMatrix) * c_normal_scale;
	
	OUT.TBNTranspose = transpose(WorldToTangentSpace);
	
	// output the screen position of the vertex
	OUT.Position = mul(WorldPosition, c_world_view_projection);
	
	// calculate and output the texture coordinates of the vertex
	OUT.DiffuseMultiUV.x = dot(IN.TexCoord0, c_base_map_xform_x);
	OUT.DiffuseMultiUV.y = dot(IN.TexCoord0, c_base_map_xform_y);
	OUT.DetailUV.xy = OUT.DiffuseMultiUV.xy * c_primary_detail_scale;
	if(bSingleBone)
		OUT.DiffuseMultiUV.zw = IN.TexCoord0.z;
	else
		OUT.DiffuseMultiUV.zw = OUT.DiffuseMultiUV.xy;

	OUT.DetailUV.w = c_translucency;
	
	OUT.NormalDetailUV.xy = OUT.DiffuseMultiUV.xy * c_detail_normal0_scale;
	OUT.NormalDetailUV.zw = OUT.DiffuseMultiUV.xy * c_detail_normal1_scale;

	// output the fog value
	if(bFogged)
	{  
		float AtmosphericFogDensity = CalculatePlanarFog(WorldPosition.xyz, 
			c_planar_fog_gradient1, c_planar_fog_gradient2,
			c_planar_fog_eye_density, c_planar_fog_max_density);
		
		OUT.DetailUV.z = AtmosphericFogDensity;
		OUT.Fog = IN.TexCoord0.w - AtmosphericFogDensity;
	}
	
	return OUT;
}