struct vs_struct{
	float4 Position : POSITION0;
	float2 Tex0 : TEXCOORD0;
	float2 Tex1 : TEXCOORD1;
};

uniform extern matrix OrthoWVPMatrix 	: ORTHOWORLDVIEWPROJECTION;

vs_struct 	PassThroughVS(in vs_struct IN)
{
	vs_struct OUT = IN;
	OUT.Position = mul(OUT.Position, OrthoWVPMatrix);
	return OUT;
}

static const int shader_model_mask_1_0 = 1 << 0;
static const int shader_model_mask_2_0 = 1 << 1;
static const int shader_model_mask_3_0 = 1 << 2;

#ifdef VERTEX_SCALE_SHADER
vs_struct ScaleVS(in vs_struct IN, uniform float2 VertexScale, uniform float2 UVScale)
{
	vs_struct OUT = IN;
	OUT.Position.x = ((OUT.Position.x + 0.5f) * VertexScale) - 0.5f;
	OUT.Position.y = ((OUT.Position.y - 0.5f) * VertexScale) + 0.5f;
	OUT.Position = mul(OUT.Position, OrthoWVPMatrix);
	
	OUT.Tex0 *= UVScale;
	return OUT;	
}
#endif