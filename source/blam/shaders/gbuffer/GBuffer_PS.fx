struct PSIN {
	float2 		UV0	  			: TEXCOORD0;
	float4 		Position3D	 	: TEXCOORD1;
	float4 		PrevPosition3D	: TEXCOORD2;
    float3		Normal   		: TEXCOORD3;
    float3		BiNormal   		: TEXCOORD4;
    float3		Tangent   		: TEXCOORD5;
};

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

sampler sampler0 : register(s0);
sampler object_norm_sampler : register(s4);

float4 c_pixel_shader_input : register(c[0]);
#define c_mesh_type_index c_pixel_shader_input.x
#define c_owner_team_index c_pixel_shader_input.y
#define c_velocity_scale c_pixel_shader_input.z

//////Functions/////////////////
float GetDepth(float4 IN)
{
	return IN.z;
};

float2 GetVelocity(float4 IN_CURR, float4 IN_PREV)
{	
	float2 Velocity = (IN_CURR.xy / IN_CURR.w) - (IN_PREV.xy / IN_PREV.w);
	Velocity /= 2.0f;
	Velocity *= c_velocity_scale;
	
	return Velocity;
};

float2 GetNormals(const uniform bool bNormalMap, float3x3 TBN, float3 IN_NORMTEX)
{	
	float3 NormalOut = float3(0, 0, 1);
	
	if(bNormalMap)
		NormalOut = mul(IN_NORMTEX, TBN).xyz;
	else
		NormalOut = TBN[2];
	NormalOut = normalize(NormalOut);
	
	return (NormalOut.xy + 1) * 0.5f;
};

float2 GetIndex()
{
	return float2(c_mesh_type_index, c_owner_team_index);
};

//////Function
void DrawGBuffer_MRT3_P0(in PSIN IN, out PSOUT_MRT3_P0 OUT,
	const uniform bool bObject, 
	const uniform bool bVelocity,
	const uniform bool bNormalMap)
{
	OUT = (PSOUT_MRT3_P0)0;
	
	float4 tex_samp0 = tex2D(sampler0, IN.UV0);
	
	float3 normal_map = float3(0, 0, 1);
	if(bNormalMap)
	{
		if(bObject)
			normal_map = tex2D(object_norm_sampler, IN.UV0).rgb;
		else
			normal_map = tex_samp0.rgb;
		normal_map = (normal_map * 2.0f) - 1.0f;
	}
	
	OUT.Depth.a = tex_samp0.a;
	
	float3x3 TBN = float3x3(IN.Tangent, IN.BiNormal, IN.Normal);
	
	OUT.Depth.r 			= GetDepth(IN.Position3D);	
	if(bVelocity)
		OUT.Velocity.rg 	= GetVelocity(IN.Position3D, IN.PrevPosition3D);
	OUT.NormalIndex.rg 		= GetNormals(bNormalMap, TBN, normal_map);	
	OUT.NormalIndex.ba		= GetIndex();
}
void DrawGBuffer_MRT2_P0(in PSIN IN, out PSOUT_MRT2_P0 OUT)
{
	OUT = (PSOUT_MRT2_P0)0;
	
	float4 tex_samp0 = tex2D(sampler0, IN.UV0);
	OUT.Depth.a = tex_samp0.a;
	
	OUT.Depth.r 			= GetDepth(IN.Position3D);
}
void DrawGBuffer_MRT2_P1(in PSIN IN, out PSOUT_MRT2_P1 OUT,
	const uniform bool bObject, 
	const uniform bool bVelocity,
	const uniform bool bNormalMap)
{
	OUT = (PSOUT_MRT2_P1)0;
	
	float4 tex_samp0 = tex2D(sampler0, IN.UV0);
	
	float3 normal_map = float3(0, 0, 1);
	if(bNormalMap)
	{
		if(bObject)
			normal_map = tex2D(object_norm_sampler, IN.UV0).rgb;
		else
			normal_map = tex_samp0.rgb;
		normal_map = (normal_map * 2.0f) - 1.0f;
	}
	
	OUT.Velocity.a = tex_samp0.a;
	
	float3x3 TBN = float3x3(IN.Tangent, IN.BiNormal, IN.Normal);
	
	if(bVelocity)
		OUT.Velocity.rg 	= GetVelocity(IN.Position3D, IN.PrevPosition3D);
	OUT.NormalIndex.rg 		= GetNormals(bNormalMap, TBN, normal_map);	
	OUT.NormalIndex.ba		= GetIndex();
}

void DrawGBufferNone_MRT3_P0(out PSOUT_MRT3_P0 OUT)
{
	OUT = (PSOUT_MRT3_P0)0;
	
	OUT.Depth = 1.0f;
	OUT.Velocity = 0.0f;
	OUT.NormalIndex.rg = 0.5f;
	OUT.NormalIndex.ba = 0;
}
void DrawGBufferNone_MRT2_P0(out PSOUT_MRT2_P0 OUT)
{
	OUT = (PSOUT_MRT2_P0)0;
	
	OUT.Depth = 1.0f;
}
void DrawGBufferNone_MRT2_P1(out PSOUT_MRT2_P1 OUT)
{
	OUT = (PSOUT_MRT2_P1)0;
	
	OUT.Velocity = 0.0f;
	OUT.Velocity.a = 1.0f;
	OUT.NormalIndex.rg = 0.5f;
	OUT.NormalIndex.ba = 0;
}
//////Function

//////Techniques
technique PS_MRT3_BSP_N
{
	pass P0
	{
		PixelShader = compile ps_2_0 DrawGBuffer_MRT3_P0(false, false, true);
	}
};
technique PS_MRT3_BSP_N_V
{
	pass P0
	{
		PixelShader = compile ps_2_0 DrawGBuffer_MRT3_P0(false, true, true);
	}
};
technique PS_MRT3_Object_N
{
	pass P0
	{
		PixelShader = compile ps_2_0 DrawGBuffer_MRT3_P0(true, false, false);
	}
};
technique PS_MRT3_Object_TBN
{
	pass P0
	{
		PixelShader = compile ps_2_0 DrawGBuffer_MRT3_P0(true, false, true);
	}
};
technique PS_MRT3_Object_N_V
{
	pass P0
	{
		PixelShader = compile ps_2_0 DrawGBuffer_MRT3_P0(true, true, false);
	}
};
technique PS_MRT3_Object_TBN_V
{
	pass P0
	{
		PixelShader = compile ps_2_0 DrawGBuffer_MRT3_P0(true, true, true);
	}
};
technique PS_MRT3_Object_NONE
{
	pass P0
	{
		PixelShader = compile ps_2_0 DrawGBufferNone_MRT3_P0();
	}
};



technique PS_MRT2_BSP_N
{
	pass P0
	{
		PixelShader = compile ps_2_0 DrawGBuffer_MRT2_P0();
	}
	pass P1
	{
		PixelShader = compile ps_2_0 DrawGBuffer_MRT2_P1(false, false, true);
	}
};
technique PS_MRT2_BSP_N_V
{
	pass P0
	{
		PixelShader = compile ps_2_0 DrawGBuffer_MRT2_P0();
	}
	pass P1
	{
		PixelShader = compile ps_2_0 DrawGBuffer_MRT2_P1(false, true, true);
	}
};
technique PS_MRT2_Object_N
{
	pass P0
	{
		PixelShader = compile ps_2_0 DrawGBuffer_MRT2_P0();
	}
	pass P1
	{
		PixelShader = compile ps_2_0 DrawGBuffer_MRT2_P1(true, false, false);
	}
};
technique PS_MRT2_Object_TBN
{
	pass P0
	{
		PixelShader = compile ps_2_0 DrawGBuffer_MRT2_P0();
	}
	pass P1
	{
		PixelShader = compile ps_2_0 DrawGBuffer_MRT2_P1(true, false, true);
	}
};
technique PS_MRT2_Object_N_V
{
	pass P0
	{
		PixelShader = compile ps_2_0 DrawGBuffer_MRT2_P0();
	}
	pass P1
	{
		PixelShader = compile ps_2_0 DrawGBuffer_MRT2_P1(true, true, false);
	}
};
technique PS_MRT2_Object_TBN_V
{
	pass P0
	{
		PixelShader = compile ps_2_0 DrawGBuffer_MRT2_P0();
	}
	pass P1
	{
		PixelShader = compile ps_2_0 DrawGBuffer_MRT2_P1(true, true, true);
	}
};
technique PS_MRT2_Object_NONE
{
	pass P0
	{
		PixelShader = compile ps_2_0 DrawGBufferNone_MRT2_P0();
	}
	pass P1
	{
		PixelShader = compile ps_2_0 DrawGBufferNone_MRT2_P1();
	}
};
//////Techniques