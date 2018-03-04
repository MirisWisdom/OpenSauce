texture GBuffer_Depth : TEXDEPTH;

sampler2D GBuffer0_DepthSampler =
sampler_state
{
	Texture = <GBuffer_Depth>;
	AddressU = Clamp;
	AddressV = Clamp;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = Point;
};

uniform extern int Channel_Depth_X : GBUFFER_DEPTH_X;
float GetDepth(float2 Tex)
{	
	return tex2D(GBuffer0_DepthSampler, Tex)[Channel_Depth_X];
}

texture GBuffer_Velocity : TEXVELOCITY;

sampler2D GBuffer0_VelocitySampler =
sampler_state
{
	Texture = <GBuffer_Velocity>;
	AddressU = Clamp;
	AddressV = Clamp;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = Point;
};

uniform extern int Channel_Velocity_X : GBUFFER_VELOCITY_X;
uniform extern int Channel_Velocity_Y : GBUFFER_VELOCITY_Y;
float2 GetVelocity(float2 Tex)
{	
	float4 Pixel = tex2D(GBuffer0_VelocitySampler, Tex);
	return float2(Pixel[Channel_Velocity_X], Pixel[Channel_Velocity_Y]);
}

texture GBuffer_Normals : TEXNORMALS;

sampler2D GBuffer_NormalsSampler =
sampler_state
{
	Texture = <GBuffer_Normals>;
	AddressU = Clamp;
	AddressV = Clamp;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = Point;
};

uniform extern int Channel_Normals_X : GBUFFER_NORMALS_X;
uniform extern int Channel_Normals_Y : GBUFFER_NORMALS_Y;
float2 GetNormals(float2 Tex)
{
	float4 Pixel = tex2D(GBuffer_NormalsSampler, Tex);
	return (float2(Pixel[Channel_Normals_X], Pixel[Channel_Normals_Y]) * 2.0f) - 1.0f;
}

texture GBuffer_Index : TEXINDEX;

sampler2D GBuffer_IndexSampler =
sampler_state
{
	Texture = <GBuffer_Index>;
	AddressU = Clamp;
	AddressV = Clamp;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = Point;
};

uniform extern int Channel_Index_X : GBUFFER_INDEX_X;
uniform extern int Channel_Index_Y : GBUFFER_INDEX_Y;

float2 GetIndex(float2 Tex)
{
	float4 Pixel = tex2D(GBuffer_IndexSampler, Tex);
	return float2(Pixel[Channel_Index_X], Pixel[Channel_Index_Y]);
}

uniform extern float FarClipDistance = 0;
uniform extern int RenderTarget = 0;

float4 GetDBGDepth(float2 Tex)
{	
	float Depth = GetDepth(Tex) * FarClipDistance;
	float dl = Depth;
    dl = min(dl,200.0);
    dl = max(dl,0.0);
	return dl * pow(200, -1.0f);
}
float4 GetDBGVelocity(float2 Tex)
{			
	return float4(GetVelocity(Tex), 0.0f, 1.0f);
}
float4 GetDBGNormals(float2 Tex, bool bShifted)
{			
	float3 Normals = 0;
	Normals.rg = GetNormals(Tex);
	Normals.b = sqrt(1.0f - pow(Normals.r, 2) - pow(Normals.g, 2));
	if(bShifted)	
		Normals = (Normals + 1.0f) * 0.5;
	return float4(Normals, 1.0f);	
}
float4 GetDBGIndex(float2 Tex)
{	
	float4 Index = float4(GetIndex(Tex), 0, 1);	
	return Index;
}
float4 DebugRTSingle_PS( float2 Tex : TEXCOORD0 ) : COLOR
{
	float4 OUT = 0;
	if			(RenderTarget == 1)
		OUT = GetDBGDepth(Tex);
	else if		(RenderTarget == 2)
		OUT = GetDBGVelocity(Tex);
	else if		(RenderTarget == 3)
		OUT = GetDBGNormals(Tex, false);
	else if		(RenderTarget == 4)
		OUT = GetDBGNormals(Tex, true);
	else if		(RenderTarget == 5)
		OUT = GetDBGIndex(Tex);
	return  OUT;
};
float4 DebugRTAll_PS( float2 Tex : TEXCOORD0 ) : COLOR
{
	float4 OUT = 0;
	Tex *= 2;
	if			(Tex.x < 1 && Tex.y < 1)
		OUT = GetDBGDepth(Tex);
	else if		(Tex.x > 1 && Tex.y < 1)
		OUT = GetDBGVelocity(Tex - float2(1,0));
	else if		(Tex.x < 1 && Tex.y > 1)
		OUT = GetDBGNormals(Tex - float2(0,1), true);
	else if		(Tex.x > 1 && Tex.y > 1)
		OUT = GetDBGIndex(Tex - float2(1,1));
	return  OUT;
};

technique DebugRTSingle
{
	pass p0 
	{
		PixelShader = compile ps_2_0 DebugRTSingle_PS();
		VertexShader = null;
	}
}
technique DebugRTAll
{
	pass p0 
	{
		PixelShader = compile ps_2_0 DebugRTAll_PS();
		VertexShader = null;
	}
}