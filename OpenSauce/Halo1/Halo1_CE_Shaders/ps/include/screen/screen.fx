#ifndef SCREEN_SCREEN_FX
#define SCREEN_SCREEN_FX

float4 c0 : register(c0);
float4 c1 : register(c1);
float4 c2 : register(c2);

sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);
sampler TexS2 : register(s[2]);

static const int screen_blend_add = 0;
static const int screen_blend_dot = 1;
static const int screen_blend_multiply = 2;
static const int screen_blend_multiply2x = 3;
static const int screen_blend_subtract = 4;

float4 ScreenMultitexture(uniform const int BlendType1,
	uniform const int BlendType2,
	half4 v0 : COLOR0,
	half2 Tex0 : TEXCOORD0,
	half2 Tex1 : TEXCOORD1,
	half2 Tex2 : TEXCOORD2) : COLOR0
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 t0 = tex2D(TexS0, Tex0);
	half4 t1 = tex2D(TexS1, Tex1);
	half4 t2 = tex2D(TexS2, Tex2);

	t0 = (t0) * (c0);
	t1 = (t1) * (c1);
	t2 = (t2) * (c2);
	r0.rgb = (t0.rgb) * (v0);
	r0.a = t0.a;
	
	if(BlendType1 == screen_blend_add)
	{
		r0 = r0 + t1;	
	}
	else if(BlendType1 == screen_blend_dot)
	{
		r0 = saturate(dot(r0.rgb, t1.rgb));
		r0.a = (r0.a) * (t1.a);
	}
	else if(BlendType1 == screen_blend_multiply)
	{
		r0 = (r0) * (t1);
	}
	else if(BlendType1 == screen_blend_multiply2x)
	{
		r0 = ((r0) * (t1)) * 2;
	}
	else if(BlendType1 == screen_blend_subtract)
	{
		r0 = r0 - t1;
	}
	
	if(BlendType2 == screen_blend_add)
	{
		r0 = r0 + t2;
	}
	else if(BlendType2 == screen_blend_dot)
	{
		r0 = saturate(dot(r0.rgb, t2.rgb));
		r0.a = (r0.a) * (t2.a);
	}
	else if(BlendType2 == screen_blend_multiply)
	{
		r0 = (r0) * (t2);
	}
	else if(BlendType2 == screen_blend_multiply2x)
	{
		r0 = ((r0) * (t2)) * 2;
	}
	else if(BlendType2 == screen_blend_subtract)
	{
		r0 = r0 - t2;
	}
	
	return r0;
}

#endif