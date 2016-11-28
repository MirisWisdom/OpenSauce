#ifdef GBUFFER_USE_DEPTH
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
#endif

#ifdef GBUFFER_USE_VELOCITY
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
#endif

#ifdef GBUFFER_USE_NORMALS
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
float3 GetNormals(float2 Tex)
{
	float4 Pixel = tex2D(GBuffer_NormalsSampler, Tex);

	float3 Normals = 0;
	Normals.rg = (float2(Pixel[Channel_Normals_X], Pixel[Channel_Normals_Y]) - 0.5) * 2;
	Normals.b = sqrt(1.0f - pow(Normals.r, 2) - pow(Normals.g, 2));
	return Normals;
}
#endif

#ifdef GBUFFER_USE_INDEX
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
int2 GetIndex(float2 Tex)
{
	float4 Pixel = tex2D(GBuffer_IndexSampler, Tex);
	return int2((int)(Pixel[Channel_Index_X] / (1.0f / 255.0f)), (int)(Pixel[Channel_Index_Y] / (1.0f / 255.0f)));
}
int GetType(in int2 Index)
{
	return Index.x % 240;
}
int GetTeam(in int2 Index)
{
	return Index.y % 224;
}
int GetIsEnemy(in int2 Index)
{
	int Value = Index.y - (Index.y % 32);
	Value /= 32;
	return Value % 2;
}
int GetIsDead(in float2 Index)
{
	int Value = Index.y - (Index.y % 64);
	Value /= 64;
	return Value % 2;
}
#endif