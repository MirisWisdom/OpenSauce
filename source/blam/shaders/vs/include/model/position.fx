float3 GetPosition(float4 position,
	float2 BlendIndices,
	float2 BlendWeights,
	out float4x3 world_matrix,
	uniform const bool bSingleBone)
{	
	// build the transform matrix for skinning
	if(!bSingleBone)
	{
		world_matrix = c_node_matrices[BlendIndices.x] * BlendWeights.x;
		world_matrix += c_node_matrices[BlendIndices.y] * BlendWeights.y;
	}
	else
		world_matrix = c_node_matrices[0];
		
	// calculate the world space position of the vertex
	return mul(position, world_matrix);
}