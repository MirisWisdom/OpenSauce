float4 c0 : register(c[0]);
float4 c1 : register(c[1]);
float4 c2 : register(c[2]);
float4 c3 : register(c[3]);

sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);

half4 P0(half2 Tex0 : TEXCOORD0, half2 Tex1 : TEXCOORD1) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	/*
		c0 -	rgb = global_window_parameters.fog.atmospheric_maximum_density
			a = fog_blend_factor*global_window_parameters.fog.atmospheric_maximum_density
		c1 -	rgb = global_window_parameters.fog.planar_maximum_density
			a = planar_eye_density*global_window_parameters.fog.planar_maximum_density
		c2 -	rgb = global_window_parameters.fog.atmospheric_color
			a = fog_blend_factor
		c3 -	rgb = global_window_parameters.fog.planar_color
			a = 1.0f - fog_blend_factor
	*/

	float4 c4 = half4(1.0,1.0,1.0,1.0);
	half4 t0 = tex2D(TexS0, Tex0);// atmospheric fog density
	half4 t1 = tex2D(TexS1, Tex1);// planar fog density

	r0.a = (c1.a) * (t1.b);
	r0.a = saturate((c1.b) * (t1.a) + r0.a);// r0.a = Pf
	t0.rgb = (t0.a) * (c0);// t0.rgb = Af
	t0.a = saturate((t0.a) * (c0.b));// t0.a = blend * Af
	r1.rgb = (c3.rgb) * (r0.a);// r1.rgb = Pc*Pf
	r1.a = saturate((c3.a) * (r0.a));// r1.a = (1 - blend)*Pf
	r0.rgb = saturate((c2.rgb) * (t0));// r0.rgb = Ac*Af
	r0.a = saturate((1-t0.a) * (1-r0.a));// r0.a = (1 - Af)*(1 - Pf)
	r0.rgb = (r0.rgb) * (1-r1.a);// Ac*Af*(1 - (1 - blend)*Pf)
	r0.rgb = (r1.rgb) * (1-t0.a) + r0;// Ac*Af*(1 - (1 - blend)*Pf) + Pc*Pf*(1 - blend*Af)
	r0.a = c4.a - r0.a;// 1 - (1 - Af)*(1 - Pf)
	return r0;
};

Technique EnvironmentFog
{
    Pass P0
	{
		PixelShader = compile TGT P0();
 	}
}
