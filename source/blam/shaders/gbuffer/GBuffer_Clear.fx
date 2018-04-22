struct PSOUT_MRT3_P0 {
	float4		Depth			: COLOR0;
	float4		Velocity		: COLOR1;
	float4		NormalIndex		: COLOR2;
};

struct PSOUT_MRT2_P0 {
	float4		Depth			: COLOR0;
};

struct PSOUT_MRT2_P1 {
	float4		Velocity		: COLOR0;
	float4		NormalIndex		: COLOR1;
};

void DrawGBufferNoneMRT3_P0(out PSOUT_MRT3_P0 OUT)
{
	OUT.Depth = 1.0f;
	OUT.Velocity = 0.0f;
	OUT.NormalIndex.rg = 0.5f;
	OUT.NormalIndex.ba = 0;
}
void DrawGBufferNoneMRT2_P0(out PSOUT_MRT2_P0 OUT)
{
	OUT.Depth = 1.0f;
}
void DrawGBufferNoneMRT2_P1(out PSOUT_MRT2_P1 OUT)
{
	OUT.Velocity = 0.0f;
	OUT.Velocity.a = 1.0f;
	OUT.NormalIndex.rg = 0.5f;
	OUT.NormalIndex.ba = 0;
}

technique PS_MRT3_Clear_ALL
{
	pass P0
	{
		PixelShader = compile ps_3_0 DrawGBufferNoneMRT3_P0();
		VertexShader = null;
	}
};

technique PS_MRT2_Clear_ALL
{
	pass P0
	{
		PixelShader = compile ps_3_0 DrawGBufferNoneMRT2_P0();
		VertexShader = null;
	}
	pass P1
	{
		PixelShader = compile ps_3_0 DrawGBufferNoneMRT2_P1();
		VertexShader = null;
	}
};