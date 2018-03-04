#ifndef ENVIRONMENT_ENVIRONMENT_SPECULAR_FX
#define ENVIRONMENT_ENVIRONMENT_SPECULAR_FX
sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);
sampler TexS2 : register(s[2]);
sampler TexS3 : register(s[3]);

static const int environment_specular_type_light = 0;
static const int environment_specular_type_lightmap = 1;

static const int environment_specular_surface_bumped = 0;
static const int environment_specular_surface_flat = 1;

float4 EnvironmentSpecular(
	uniform const int specular_type,
	uniform const int surface_type,
	half2 Tex0 : TEXCOORD0,
	half2 Tex1 : TEXCOORD1,
	half3 Tex2 : TEXCOORD2,
	half3 Tex3 : TEXCOORD3) : COLOR
{
	half4 r0, r1, r2, r3, r3_unbias, r2_unbias r4, r5, r6;
	
	r2 = texCUBE(TexS2, Tex2);
	r3 = texCUBE(TexS3, Tex3);	
	r3_unbias.rgb = 2 * (r3.rgb - 0.5f);
	r2_unbias.rgb = 2 * (r2.rgb - 0.5f);
	
	if(specular_type == environment_specular_type_light)
	{		
		r0 = tex2D(TexS0, Tex0);
		r1 = texCUBE(TexS1, Tex1);
		
		if(surface_type == environment_specular_surface_bumped)
		{
			mov_x8_sat r1.w, r3_bx2.z
		  + dp3_sat r5.xyz, r0_bx2, r2_bx2
			mov_x8_sat r2.w, r2_bx2.z
		  + mad_x2_sat r4.xyz, r0_bx2, r5, -r2_bias
			dp3_sat r4.xyz, r4, r3_bx2
			lrp r5.xyz, r5.z, c1, c2
		  + mul r2.w, r1.w, r2.w
			mul r2.xyz, r2.w, c0
			dp3 r0.xyz, r1, c0
			phase
			texld r1, t1
			mul r4.w, r4.z, r4.z
		  + mul r5.xyz, r5, r1
			mul r4.w, r4.w, r4.w
		  + mul r5.xyz, r5, c3.z
			mul r4.xyz, r0, c3.z
			mul r4.w, r4.w, r4.w
		  + mul r5.xyz, r5, r2
			dp3_sat r4, r4, r4.w
			mul r5.xyz, r5, r4.w
			mul r0.xyz, r5, 1-v1.w
		  + mov r0.w, r4.z
		}
		else if(surface_type == environment_specular_surface_flat)
		{
			half4 c0 = half4(0, 0, 1, 0);
			
			r1.w = saturate(r3_unbias.z * 8.0f);
			mov_x8_sat r1.w, r3_bx2.z
		  + dp3_sat r0.xyz, c0, r2_bx2
			mov_x8_sat r2.w, r2_bx2.z
		  + mad_x2_sat r4.xyz, c0, r0, -r2_bias
			dp3_sat r4.xyz, r4, r3_bx2
			mul r1.xyz, c0, r1
			lrp r0.xyz, r0.z, c1, c2
		  + mul r2.w, r1.w, r2.w
			mul r2.xyz, r2.w, c0
			phase
			texld r1, t1
			mul r4.w, r4.z, r4.z
		  + mul r0.xyz, r0, r1
			mul r4.w, r4.w, r4.w
		  + mul r0.xyz, r0, c3.z
			mul r4.w, r4.w, r4.w
		  + mul r0.xyz, r0, r2
			dp3_sat r4, r4, r4.w
			mul r0.xyz, r0, r4.w
			mul r0.xyz, r0, 1-v1.w
		  + mov r0.w, r4.z
		}
	}
	else if(specular_type == environment_specular_type_lightmap)
	{		
		r1 = tex2D(TexS1, Tex1);
		
		if(surface_type == environment_specular_surface_bumped)
		{	
			r0 = tex2D(TexS0, Tex0);
			
			def c4, 0.501960814, 0.501960814, 0.501960814, 0.501960814
			texld r0, t0
			texld r1, t1
			texld r2, t2
			texld r3, t3
			mov_x8_sat r1.w, r3_bx2.z
		  + dp3_sat r5.xyz, r0_bx2, r2_bx2
			mov_x8_sat r2.w, r2_bx2.z
		  + mad_x2_sat r4.xyz, r0_bx2, r5, -r2_bias
			dp3_sat r4.xyz, r4, r3_bx2
			lrp r5.xyz, r5.z, c1, c2
		  + mul r2.w, r1.w, r2.w
			mul r2.xyz, r2.w, c0
			dp3_sat r0.xyz, r1, c0
			phase
			texld r1, t1
			mul r4.w, r4.z, r4.z
		  + mul r5.xyz, r5, r1
			mul r4.w, r4.w, r4.w
		  + mul r5.xyz, r5, c3.z
			mul r4.w, r4.w, r4.w
		  + mul r5.xyz, r5, r2
			dp3_sat r4, r4, r4.w
			mul r5.xyz, r5, r4.w
			mul r0.xyz, r5, c4
		  + mov r0.w, r4.z
		}
		else if(surface_type == environment_specular_surface_flat)
		{
			def c0, 0, 0, 1, 0
			def c4, 0.501960814, 0.501960814, 0.501960814, 0.501960814
			texld r1, t1
			texld r2, t2
			texld r3, t3
			mov_x8_sat r1.w, r3_bx2.z
		  + dp3_sat r0.xyz, c0, r2_bx2
			mov_x8_sat r2.w, r2_bx2.z
		  + mad_x2_sat r4.xyz, c0, r0, -r2_bias
			dp3_sat r4.xyz, r4, r3_bx2
			mul r1.xyz, c0, r1
			lrp r0.xyz, r0.z, c1, c2
		  + mul r2.w, r1.w, r2.w
			mul r2.xyz, r2.w, c0
			phase
			texld r1, t1
			mul r4.w, r4.z, r4.z
		  + mul r0.xyz, r0, r1
			mul r4.w, r4.w, r4.w
		  + mul r0.xyz, r0, c3.z
			mul r4.w, r4.w, r4.w
		  + mul r0.xyz, r0, r2
			dp3_sat r4, r4, r4.w
			mul r0.xyz, r0, r4.w
			mul r0.xyz, r0, c4
		  + mov r0.w, r4.z
		}
	}
	
	return r0;
}

#endif