float4 c0 : register(c[0]);
float4 c1 : register(c[1]);
   
sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);
sampler TexS2 : register(s[2]);
sampler TexS3 : register(s[3]);

// PASS
half4 P0(half2 Tex0 : TEXCOORD0, half3 Tex1 : TEXCOORD1, half3 Tex2 : TEXCOORD2, half3 Tex3 : TEXCOORD3) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 t0 = tex2D(TexS0, Tex0);
	half4 t1 = texCUBE(TexS1, Tex1);// gel
	half4 t2 = tex3D(TexS2, Tex2);// distance attenuation
	half4 t3 = texCUBE(TexS3, Tex3);
	
	t3.rgb = 2 * (t3.rgb - 0.5f);

	r0.rgb = saturate(t1.rgb * t2.rgb);// gel color and spherical attenuation
	r0.a = (t3.b + t3.b) * 4;// self shadow mask
	r1 = saturate(dot(2 * (t0 - 0.5).rgb, t3.rgb));// bump attenuation
	r0 = saturate(r1 * r0);// light with gel, distance and bump
	r0 = saturate(r0.a * r0);// light with self shadow
	r0 = saturate(r0 * c0);// light with color
	r1 = saturate(dot(r0.rgb, c0.rgb));// active pixel mask
	r0.rgb = saturate(c1.rgb * r0);// final light
	r0.a = saturate(r1.b);// active pixel mask
	
	return r0;
};

Technique EnvironmentDiffuseLights
{
    Pass P0
	{
		PixelShader = compile TGT P0();
 	}
}
