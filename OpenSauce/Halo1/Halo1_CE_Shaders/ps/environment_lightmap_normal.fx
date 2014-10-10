#include "include/environment/environment_lightmap.fx"

sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);
sampler TexS2 : register(s[2]);
sampler TexS3 : register(s[3]);
sampler TexS4 : register(s[4]);
sampler TexS5 : register(s[5]);
sampler TexS6 : register(s[6]);

float4 		c_material_color					: register(c[0]);
float4 		c_plasma_animation					: register(c[1]);
float4 		c_primary_color						: register(c[2]);
float4 		c_secondary_color					: register(c[3]);
float4 		c_plasma_on_color					: register(c[4]);
float4 		c_plasma_off_color					: register(c[5]);

float4 		c_extension_vars1					: register(c[6]);
#define 	c_normal_map_coefficient			c_extension_vars1.x
#define 	c_normal_map_z_coefficient			c_extension_vars1.y
#define 	c_spec_color_exponent_mask_toggle	c_extension_vars1.z
float4 		c_extension_vars2					: register(c[7]);
#define 	c_specular_color_map_exponent		c_extension_vars2.x
#define 	c_specular_color_map_coefficient	c_extension_vars2.y
#define 	c_specular_lighting_exponent		c_extension_vars2.z
#define 	c_specular_lighting_coefficient		c_extension_vars2.w
float4 		c_perpendicular_specular_colour		: register(c[8]);
float4 		c_parallel_specular_colour			: register(c[9]);

// PASS
half4 PS_LightmapNormal(
	uniform const bool bDirectional,
	uniform const bool bBaseNormal,
	uniform const bool bSpecular,
	uniform const bool bSpecularMap,
	half4 Diff : COLOR0,
	half2 Tex0  : TEXCOORD0,
	half2 Tex1  : TEXCOORD1,
	half2 Tex2  : TEXCOORD2,
	half3 Tex3  : TEXCOORD3,
	half3 EyeVectorTS : TEXCOORD4) : COLOR
{
	half4 r0, r1, r2, r3, r4, r7, r11;
	
	half4 self_illumination_color = tex2D(TexS1, Tex1);
	half3 lightmap = 0;
	half4 bump_color = tex2D(TexS0, Tex0);
	
	if(!bDirectional)
	{
		lightmap = tex2D(TexS2, Tex2).rgb;
	}
	else
	{
		float3 BaseBump = float3(0, 0, 1);
			
		if(bBaseNormal)
		{
			BaseBump = (bump_color.rgb * 2) - 1.0f;
			BaseBump = normalize(BaseBump);
			BaseBump.xy *= c_normal_map_coefficient;
			BaseBump.z *= c_normal_map_z_coefficient;
		}
		
		half3 lightmap_component_0 = tex2D(TexS2, Tex2).rgb;
		half3 lightmap_component_1 = tex2D(TexS4, Tex2).rgb;
		half3 lightmap_component_2 = tex2D(TexS5, Tex2).rgb;

		if(!bSpecular)
		{
			lightmap = CalculateDLMDiff(BaseBump,
				lightmap_component_0,
				lightmap_component_1,
				lightmap_component_2);
		}
		else
		{
			float4 SpecularMap = 1;
			if(bSpecularMap)
			{
				SpecularMap = tex2D(TexS6, Tex0);
				SpecularMap.xyz = saturate(pow(SpecularMap.xyz, c_specular_color_map_exponent) * c_specular_color_map_coefficient);
				SpecularMap.w = lerp(1, SpecularMap.w, c_spec_color_exponent_mask_toggle);
			}
			
			float4 SpecularTint = 0;
			SpecularTint = GetSpecularColor(BaseBump, normalize(EyeVectorTS), SpecularMap, c_perpendicular_specular_colour, c_parallel_specular_colour);
			
			lightmap = CalculateDLMSpec(BaseBump,
				EyeVectorTS,
				lightmap_component_0,
				lightmap_component_1,
				lightmap_component_2,
				SpecularTint,
				c_specular_lighting_exponent,
				SpecularMap.w,
				c_specular_lighting_coefficient,
				SpecularTint.w);
		}
	}
	
	float plasma_intermed_1 = c_plasma_animation + 0.5 - max(0, self_illumination_color.a);
	float plasma_intermed_2 = self_illumination_color.a + 0.5 - max(0, c_plasma_animation);

	float plasma = (plasma_intermed_1 < 0.5f) ? plasma_intermed_1 : plasma_intermed_2;
	plasma = plasma * plasma * 4;                                     // (plasma ^ 2) * 4
	plasma = plasma * plasma;                                         // 16 * plasma ^ 4
	plasma = (2 * plasma - 1) < 0.5f ? 0 : (2 * plasma - 1) * (2 * plasma - 1);   // plasma ^ 8
	
	float3 plasma_color = ((plasma * c_plasma_on_color) + c_plasma_off_color) * self_illumination_color.z;
    float3 primary_secondary_color = (self_illumination_color.x * c_primary_color) + (self_illumination_color.y * c_secondary_color);
	float3 lightmap_color = (lightmap * c_material_color);
	
	float4 result;
    result.xyz = lightmap_color + plasma_color + primary_secondary_color;
    result.w = bump_color.w;
	
    return result;	
}

Technique EnvironmentLightmapNormal
{
	Pass P0
	{
		PixelShader = compile TGT PS_LightmapNormal(false, false, false, false);
	}
}

Technique EnvironmentLightmapNormalDirDiff
{
	Pass P0
	{
		PixelShader = compile TGT PS_LightmapNormal(true, false, false, false);
	}
}

Technique EnvironmentLightmapNormalDirDiffBaseNorm
{
	Pass P0
	{
		PixelShader = compile TGT PS_LightmapNormal(true, true, false, false);
	}
}

Technique EnvironmentLightmapNormalDirSpec
{
	Pass P0
	{
		PixelShader = compile TGT PS_LightmapNormal(true, false, true, false);
	}
}

Technique EnvironmentLightmapNormalDirSpecBaseNorm
{
	Pass P0
	{
		PixelShader = compile TGT PS_LightmapNormal(true, true, true, false);
	}
}

Technique EnvironmentLightmapNormalDirSpecMap
{
	Pass P0
	{
		PixelShader = compile TGT PS_LightmapNormal(true, false, true, true);
	}
}

Technique EnvironmentLightmapNormalDirSpecMapBaseNorm
{
	Pass P0
	{
		PixelShader = compile TGT PS_LightmapNormal(true, true, true, true);
	}
}