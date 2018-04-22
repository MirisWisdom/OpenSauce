#ifndef ENVIRONMENT_ENVIRONMENT_TEXTURE_FX
#define ENVIRONMENT_ENVIRONMENT_TEXTURE_FX

sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);
sampler TexS2 : register(s[2]);
sampler TexS3 : register(s[3]);

static const int environment_texture_normal = 0;
static const int environment_texture_blended = 1;
static const int environment_texture_specular_mask = 2;

static const int environment_texture_blend_biased_add = 0;
static const int environment_texture_blend_biased_multiply = 1;
static const int environment_texture_blend_multiply = 2;

float4 EnvironmentTexture(
	uniform const int environment_type,
	uniform const int detail_blend_type,
	uniform const int micro_detail_blend_type,
	half2 Tex0 : TEXCOORD0,
	half2 Tex1 : TEXCOORD1,
	half2 Tex2 : TEXCOORD2,
	half2 Tex3 : TEXCOORD3) : COLOR
{
	half4 r0;

	half4 t0 = tex2D(TexS0, Tex0);
	half4 t1 = tex2D(TexS1, Tex1);
	half4 t2 = tex2D(TexS2, Tex2);
	half4 t3 = tex2D(TexS3, Tex3);
	
	// environment type
	if(environment_type == environment_texture_normal)
	{
		r0.rgb = (t0.a) * (t1);// detail map part 1
		r0.a = (t0.a) * (t1.a);// detail specular mask part 1
		r0.rgb = (1-t0.a) * (t2.rgb) + r0;// detail map part 2
		r0.a = (1-t0.a) * (t2.a) + r0.a;// detail specular mask part 2
	}
	else if(environment_type == environment_texture_blended)
	{
		r0.rgb = (t0.a) * (t1);// detail map part 1
		r0.a = (t0.a) * (t1.a);// detail specular mask part 1
		r0.rgb = (1-t0.a) * (t2.rgb) + r0;// detail map part 2
		r0.a = (1-t0.a) * (t2.a) + r0.a;// detail specular mask part 2
	}
	else if(environment_type == environment_texture_specular_mask)
	{
		r0 = (t0.a) * (t1);// detail map part 1
		r0.rgb = (1-t0.a) * (t2.rgb) + r0;// detail map part 2
		r0.a = t0.a;// detail specular mask
	}
	
	// detail map function
	if(detail_blend_type == environment_texture_blend_biased_add)
		r0.rgb = t0.rgb + (2 * (r0-0.5));
	else if(detail_blend_type == environment_texture_blend_biased_multiply)
		r0.rgb = ((t0.rgb) * (r0)) * 2;
	else if(detail_blend_type == environment_texture_blend_multiply)
		r0.rgb = (t0.rgb) * (r0);
		
	if(environment_type == environment_texture_normal)
		r0.a = (t0.a) * (r0.a);
	else
		r0.a = (t3.a) * (r0.a);
	
	// micro detail map function
	if(micro_detail_blend_type == environment_texture_blend_biased_add)
		r0.rgb = t3.rgb + (2 * (r0-0.5));
	else if(micro_detail_blend_type == environment_texture_blend_biased_multiply)
		r0.rgb = ((t3.rgb) * (r0)) * 2;
	else if(micro_detail_blend_type == environment_texture_blend_multiply)
		r0.rgb = (t3.rgb) * (r0);
	
	if(environment_type == environment_texture_normal)
		r0.a = (t3.a) * (r0.a);// Modulate specular mask by micro detail alpha
	
	return r0;
}

#endif