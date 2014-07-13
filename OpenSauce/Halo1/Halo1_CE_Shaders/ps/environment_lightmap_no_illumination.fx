#include "include/environment/environment_lightmap.fx"

sampler TexSampler0 : register(s[0]);
sampler TexSampler1 : register(s[1]);
sampler TexSampler2 : register(s[2]);
sampler TexSampler3 : register(s[3]);
sampler TexSampler4 : register(s[4]);
sampler TexSampler5 : register(s[5]);

float4 		c_material_color 				: register(c[0]);
float4 		c_extension_vars				: register(c[6]);
#define 	c_normal_map_coefficient		c_extension_vars.x
#define 	c_normal_map_z_coefficient		c_extension_vars.y

// PASS
half4 PS_LightmapNoIllumination(uniform const bool bDirectional,
	uniform const bool bBaseNormal,
	half4 v0 : COLOR0,
	half2 Tex0 : TEXCOORD0,
	half2 Tex1 : TEXCOORD1,
	half2 Tex2 : TEXCOORD2,
	half3 Tex3 : TEXCOORD3) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 bump_color = tex2D(TexSampler0, Tex0);
	half4 t1 = tex2D(TexSampler1, Tex1);
	float3 lightmap = 0;
	half4 t3 = texCUBE(TexSampler3, Tex3);

	half3 normal_unbiased = normalize((bump_color.rgb * 2.0f) - 1.0f);
	if(!bDirectional)
	{
		lightmap = tex2D(TexSampler2, Tex2).rgb;
	}
	else
	{
		float3 BaseBump = float3(0, 0, 1);
			
		if(bBaseNormal)
		{			
			BaseBump = normal_unbiased;
			BaseBump.xy *= c_normal_map_coefficient;
			BaseBump.z *= c_normal_map_z_coefficient;
		}
		
		half3 lightmap_component_0 = tex2D(TexSampler2, Tex2).rgb;
		half3 lightmap_component_1 = tex2D(TexSampler4, Tex2).rgb;
		half3 lightmap_component_2 = tex2D(TexSampler5, Tex2).rgb;

		lightmap = CalculateDLM(BaseBump,
			lightmap_component_0,
			lightmap_component_1,
			lightmap_component_2);
	}
	
	r0 = saturate(dot(normal_unbiased, 2 * (t3 - 0.5).rgb));
	r0.rgb = lightmap * c_material_color;
	r0.a = bump_color.b;
	r0.a = (v0.a * r0.a) + (1 - v0.a);
	r1 = r0.a * c_material_color;
	r0.rgb = (lightmap * t1) + r0;
	r0.a = bump_color.a;
	
	return r0;
};

Technique EnvironmentLightmapNoIllumination
{
    Pass P0
	{
		PixelShader = compile TGT PS_LightmapNoIllumination(false, false);
 	}
}

Technique EnvironmentLightmapNoIlluminationDir
{
    Pass P0
	{
		PixelShader = compile TGT PS_LightmapNoIllumination(true, false);
 	}
}

Technique EnvironmentLightmapNoIlluminationDirBaseNorm
{
    Pass P0
	{
		PixelShader = compile TGT PS_LightmapNoIllumination(true, true);
 	}
}
