#include "include/environment/environment_lightmap.fx"

sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);
sampler TexS2 : register(s[2]);
sampler TexS3 : register(s[3]);
sampler TexS4 : register(s[4]);
sampler TexS5 : register(s[5]);

float4 		c_material_color				: register(c[0]);
float4 		c_plasma_animation				: register(c[1]);
float4 		c_primary_color					: register(c[2]);
float4 		c_secondary_color				: register(c[3]);
float4 		c_plasma_on_color				: register(c[4]);
float4 		c_plasma_off_color				: register(c[5]);
float4 		c_extension_vars				: register(c[6]);
#define 	c_normal_map_coefficient		c_extension_vars.x
#define 	c_normal_map_z_coefficient		c_extension_vars.y

// PASS
half4 PS_LightmapNormal(
	uniform const bool bDirectional,
	uniform const bool bBaseNormal,
	half4 Diff : COLOR0,
	half2 Tex0  : TEXCOORD0,
	half2 Tex1  : TEXCOORD1,
	half2 Tex2  : TEXCOORD2,
	half3 Tex3  : TEXCOORD3) : COLOR
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

		lightmap = CalculateDLM(BaseBump,
			lightmap_component_0,
			lightmap_component_1,
			lightmap_component_2);
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
		PixelShader = compile TGT PS_LightmapNormal(false, false);
	}
}

Technique EnvironmentLightmapNormalDir
{
	Pass P0
	{
		PixelShader = compile TGT PS_LightmapNormal(true, false);
	}
}

Technique EnvironmentLightmapNormalDirBaseNorm
{
	Pass P0
	{
		PixelShader = compile TGT PS_LightmapNormal(true, true);
	}
}