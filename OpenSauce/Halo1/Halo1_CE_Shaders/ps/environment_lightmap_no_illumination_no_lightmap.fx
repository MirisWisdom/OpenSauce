sampler TexSampler0 : register(s[0]);
sampler TexSampler3 : register(s[3]);

float4 c_material_color : register(c[0]);

// PASS
half4 PS_LightmapNoIlluminationNoLightmap(
    half4 Diff : COLOR0,
    half2 Tex0  : TEXCOORD0,
    half2 Tex1  : TEXCOORD1,
    half2 Tex2  : TEXCOORD2,
    half3 Tex3  : TEXCOORD3) : COLOR
{
	half4 bump_color = tex2D(TexSampler0,Tex0);
	half4 normal_color = texCUBE(TexSampler3, Tex3);

	////////////////////////////////////////////////////////////
	// calculate bump attenuation
	////////////////////////////////////////////////////////////
	half bump_attenuation = dot((2 * bump_color) - 1, (2 * normal_color) - 1);
	bump_attenuation = (bump_attenuation * Diff.a) + (1 - Diff.a);
	
	float4 result;
	result.rgb = (bump_attenuation * c_material_color) + c_material_color;
	result.a = normal_color.a;
	
	return result;
}

Technique EnvironmentLightmapNoIlluminationNoLightmap
{
    Pass P0
	{
		PixelShader = compile TGT PS_LightmapNoIlluminationNoLightmap();
 	}
}
