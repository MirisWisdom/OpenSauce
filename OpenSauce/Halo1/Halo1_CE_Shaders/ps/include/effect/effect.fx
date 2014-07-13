#ifndef EFFECT_EFFECT_FX
#define EFFECT_EFFECT_FX

//#define INCLUDE_SOFT

sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);
sampler TexS4 : register(s[4]);

static const int effect_tint_nonlinear = 0;
static const int effect_tint_normal = 1;

static const int effect_blend_none = 0;
static const int effect_blend_add = 1;
static const int effect_blend_alpha_blend = 2;
static const int effect_blend_double_multiply = 3;
static const int effect_blend_multiply = 4;
static const int effect_blend_multiply_add = 5;

float4 c_soft_particle_vars	: register(c[8]);

float4 PS_Effect(uniform const bool bMultiTexture,
	uniform const int nTintType,
	uniform const int nBlendType,
	uniform const bool bSoft,
	float4 v0 : COLOR0,
	float4 TextureCoords : TEXCOORD0,
	float3 Position : TEXCOORD1) : COLOR0
{
	half4 r0 = 0;
	
	half4 t0 = tex2D(TexS0, TextureCoords.xy);
	r0.a = t0.a;
	
	float Softness = 1.0f;	
	if(bSoft)
	{
		float ScreenDepth = tex2D(TexS4, Position.xy).r * c_soft_particle_vars.x;
		Softness = saturate((ScreenDepth - Position.z) * c_soft_particle_vars.y);
	}
	
	if(nTintType == effect_tint_nonlinear)
	{
		r0.rgb = t0.rgb * t0;
		r0.rgb = r0.rgb * r0;
		r0.rgb = r0 + v0.rgb * (t0.rgb - r0);		
	}
	else if(nTintType == effect_tint_normal)
	{
		r0.rgb = t0.rgb * v0;
	}
	
	if((nBlendType == effect_blend_none) && bSoft)
	{
		r0.a *= Softness;
	}
	else if(nBlendType == effect_blend_add)
	{
		r0.rgb = r0.rgb * v0.a * Softness;
	}
	else if(nBlendType == effect_blend_alpha_blend)
	{
		r0.a *= v0.a * Softness;
		
		if((nTintType == effect_tint_normal) && bMultiTexture)
		{				
			half4 t1 = tex2D(TexS1, TextureCoords.zw);
			r0.a *= t1.a;
		}
	}
	else if(nBlendType == effect_blend_double_multiply)
	{
		r0.rgb = 0.5f + (v0.a) * (r0.rgb - 0.5f) * Softness;
	}
	else if(nBlendType == effect_blend_multiply)
	{
		r0.rgb = r0.rgb * v0.a + 1-v0.a * Softness;
	}
	else if(nBlendType == effect_blend_multiply_add)
	{
		r0 = r0 * v0.a * Softness;
	}
	
	return r0;
}
#endif